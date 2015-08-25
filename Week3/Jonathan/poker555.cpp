#include <stdint.h>
#include <stdio.h>

#include "sha256.h"

#define POKER_PLAYERS (5)
#define POKER_CARDS_PER_HAND (5)
#define POKER_DECKS (5)
#define POKER_CARDS_PER_DECK (52)
#define POKER_TOTAL_CARDS (POKER_DECKS * POKER_CARDS_PER_DECK)

/* specific to dealer */
class PokerDealer
{
private:
	uint64_t state;
	uint8_t  hands[POKER_PLAYERS][POKER_CARDS_PER_HAND];

public:
			PokerDealer(void);
	void	Deal(void);
	void	GetHand(int player, uint8_t* cards) {
		for(int c=0; c < POKER_CARDS_PER_HAND; c++)
			cards[c] = hands[player][c];
	}
	void	PrintHand(int player);
};

PokerDealer::PokerDealer(void)
{
	/* this is an excellent source of randomness, bruce says so */
	FILE* fp = fopen("/dev/random", "rb");
	fread(&state, sizeof(state), 1, fp);
	fclose(fp);

	for(int p=0; p < POKER_PLAYERS; p++)
		for(int c=0; c < POKER_CARDS_PER_HAND; c++)
			hands[p][c] = 0;
}

void PokerDealer::Deal(void)
{
	uint32_t rng=0, out=0;
	uint8_t hash[SHA256::HashBytes];
	SHA256 sha256;
	uint8_t deck[POKER_TOTAL_CARDS];
	int deckSize = 0, index = 0;

	for(int d=0; d < POKER_DECKS; d++)
		for(int c=0; c < POKER_CARDS_PER_DECK; c++)
			deck[deckSize++] = c;

	/* millions of players at once... */
	/* that /dev/random is too slow to use every spin */
	/* cryptographic hash is effectively a random number, bruce says so */
	sha256.add(&state, sizeof(state));
	sha256.getHash(hash);

	for(int h=POKER_CARDS_PER_HAND-1; h >= 0; h--) {
		for(int p=POKER_PLAYERS-1; p >= 0; p--) {
			if(deckSize > 255) {
				/* need more than one byte of entropy per card */
				/* otherwise cards at end of deck never get picked */
				rng = (rng << 16) + (hash[index] << 8) + hash[index+1];
				index += 2;
			} else {
				rng = (rng << 8) + hash[index++];
			}

			hands[p][h] = out = deck[rng % deckSize];
			deck[rng % deckSize] = deck[deckSize-1];
			rng /= deckSize;
			deckSize--;
		}

		/* initial state is totally random, so output sequence is different every time */
		/* all bytes of the hash contribute to new state */
		state += state + out;
		state ^= (uint64_t) hash[30] << 56;
		state ^= (uint64_t) hash[31] << 48;
	}
}

#define HEARTS   (1ULL << 0)
#define CLUBS    (1ULL << 13)
#define SPADES   (1ULL << 26)
#define DIAMONDS (1ULL << 39)

#define TWO   (1 << 0)
#define THREE (1 << 1)
#define FOUR  (1 << 2)
#define FIVE  (1 << 3)
#define SIX   (1 << 4)
#define SEVEN (1 << 5)
#define EIGHT (1 << 6)
#define NINE  (1 << 7)
#define TEN   (1 << 8)
#define JACK  (1 << 9)
#define QUEEN (1 << 10)
#define KING  (1 << 11)
#define ACE   (1 << 12)

#define ALLCARDS ((1 << 13) - 1)

static const char* const cardName[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };

static inline void printCard(uint8_t x)
{
	static const char value[13] = {'2','3','4','5','6','7','8','9','X','J','Q','K','A'};
	static const char suit[4] = {'H','C','S','D'};
	printf("%c%c ", value[x%13], suit[x/13]);
}

static inline int maskEqual(uint64_t hand, uint64_t mask)
{
	return ((hand & mask) == mask);
}

void PokerDealer::PrintHand(int player)
{
	uint64_t hand = 0;
	int values[13] = {0};
	int suits[4] = {0};

	for(int c=0; c < POKER_CARDS_PER_HAND; c++) {
		uint8_t x = hands[player][c];
		values[x%13]++;
		suits[x/13]++;
		hand |= 1ULL << x;
		printCard(x);
	}

	printf(" - ");

	if(maskEqual(hand, (ACE | KING | QUEEN | JACK | TEN) * HEARTS)
	 | maskEqual(hand, (ACE | KING | QUEEN | JACK | TEN) * CLUBS)
	 | maskEqual(hand, (ACE | KING | QUEEN | JACK | TEN) * SPADES)
	 | maskEqual(hand, (ACE | KING | QUEEN | JACK | TEN) * DIAMONDS) ) {
		printf("Royal Flush");
		return;
	}

	for(int shift=0; shift < 9; shift++) {
		uint64_t mask = (KING | QUEEN | JACK | TEN | NINE) >> shift;
		if(shift == 8)
			mask |= ACE;

		for(uint64_t suit = HEARTS; suit <= DIAMONDS; suit *= CLUBS) {
			if(maskEqual(hand, mask*suit)) {
				printf("Straight Flush (%s high)", cardName[11-shift]);
				return;
			}
		}
	}

	for(int card=12; card >= 0; card--) {
		if(values[card] >= 5) {
			printf("Five of a Kind (%s)", cardName[card]);
			return;
		}
	}

	for(int card=12; card >= 0; card--) {
		if(values[card] == 4) {
			int tie1 = 12;
			while(tie1 >= 0 && (!values[tie1] || tie1 == card))
				tie1--;
			printf("Four of a Kind (%s) + %s", cardName[card], cardName[tie1]);
			return;
		}
	}

	for(int card1=12; card1 >= 0; card1--) {
		if(values[card1] == 3) {
			int card2 = 12;
			while(card2 >= 0 && (values[card2] < 2 || card2 == card1))
				card2--;
			if(card2 >= 0) {
				printf("Full House (%s + %s)", cardName[card1], cardName[card2]);
				return;
			}
		}
	}

	for(int suit=0; suit < 4; suit++) {
		if(suits[suit] >= 5) {
			printf("Flush (");
			for(int c=0, cnt=0; c < POKER_CARDS_PER_HAND; c++) {
				uint8_t x = hands[player][c];
				if(x/13 != suit)
					continue;
				if(cnt++)
					printf(" + ");
				printf("%s", cardName[x%13]);
				/* FIXME: really should sort these in descending order of value */
			}
			printf(")");
			return;
		}
	}

	for(int shift=0; shift < 10; shift++) {
		uint32_t mask = (ACE | KING | QUEEN | JACK | TEN) >> shift;
		if(shift == 9)
			mask |= ACE;
		if(maskEqual((hand / HEARTS) | (hand / CLUBS) | (hand / SPADES) | (hand / DIAMONDS), mask)) {
			printf("Straight (%s high)", cardName[12-shift]);
			return;
		}
	}

	for(int card=12; card >= 0; card--) {
		if(values[card] == 3) {
			int tie1 = 12;
			while(tie1 >= 0 && (!values[tie1] || tie1 == card))
				tie1--;
			int tie2 = tie1-1;
			while(tie2 >= 0 && (!values[tie2] || tie2 == card))
				tie2--;
			printf("Three of a Kind (%s) + %s + %s", cardName[card], cardName[tie1], cardName[tie2]);
			return;
		}
	}

	for(int card1=12; card1 >= 0; card1--) {
		if(values[card1] == 2) {
			int card2 = card1-1;
			while(card2 >= 0 && (values[card2] < 2 || card2 == card1))
				card2--;
			int tie1 = 12;
			while(tie1 >= 0 && (!values[tie1] || tie1 == card1 || tie1 == card2))
				tie1--;
			if(card2 >= 0) {
				printf("Two Pair (%s + %s) + %s", cardName[card1], cardName[card2], cardName[tie1]);
				return;
			}
		}
	}

	for(int card=12; card >= 0; card--) {
		if(values[card] == 2) {
			int tie1 = 12;
			while(tie1 >= 0 && (!values[tie1] || tie1 == card))
				tie1--;
			int tie2 = tie1-1;
			while(tie2 >= 0 && (!values[tie2] || tie2 == card))
				tie2--;
			int tie3 = tie2-1;
			while(tie3 >= 0 && (!values[tie3] || tie3 == card))
				tie3--;
			printf("Pair (%s) + %s + %s + %s", cardName[card], cardName[tie1], cardName[tie2], cardName[tie3]);
			return;
		}
	}

	int cnt=0;
	for(int card=12; card >= 0; card--) {
		if(values[card]) {
			if(!(cnt++))
				printf("%s High", cardName[card]);
			else
				printf(" + %s", cardName[card]);
		}
	}
}

#ifdef TESTING
int main(void)
{
	PokerDealer dealer;

	printf("Press Return to deal!\n");

	while(1) {
		while(getchar() != '\n')
			;

		dealer.Deal();

		printf("Dealer:   ");
		dealer.PrintHand(0);
		printf("\n");

		for(int o=1; o < POKER_PLAYERS; o++) {
			printf("Player %d: ", o);
			dealer.PrintHand(o);
			printf("\n");
		}
	}

	return 0;
}
#endif
