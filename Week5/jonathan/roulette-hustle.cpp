#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <unistd.h>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "sha256.h"

#define ROULETTE_HISTORY (64)

/* specific to player */
class RoulettePlayerHistory
{
public:
	uint8_t history[ROULETTE_HISTORY];
	uint64_t state;

public:
			RoulettePlayerHistory(void);
	uint8_t Spin(void);
	void	Predict(uint8_t uncertainty, uint32_t* out);
	void	Update(uint8_t slot);
};

RoulettePlayerHistory::RoulettePlayerHistory(void)
{
	/* this is an excellent source of randomness, bruce says so */
	FILE* fp = fopen("/dev/random", "rb");
	fread(history, sizeof(history[0]), ROULETTE_HISTORY, fp);
	fread(&state, sizeof(state), 1, fp);
	fclose(fp);
}

uint8_t RoulettePlayerHistory::Spin(void)
{
	uint16_t rng;
	uint8_t out, chance, age, index = 0;
	uint8_t hash[SHA256::HashBytes];
	SHA256 sha256;

	/* millions of players at once... */
	/* that /dev/random is too slow to use every spin */
	/* cryptographic hash is effectively a random number, bruce says so */
	sha256.add(&state, sizeof(state));
	sha256.getHash(hash);

	while(1) {
		rng = (hash[index] << 8) + hash[index+1];
		index += 2;
		out = rng % 38;

		/* not a run */
		if(out != history[0])
			break;

		/* prevent runs of three */
		if(out == history[0] && out == history[1])
			continue;

		/* make chance of each number less if appeared recently */
		for(age=0; age < ROULETTE_HISTORY; age++)
			if(history[age] == out)
				break;
		chance = rng * (age+2) / 65536;
		if(chance)
			break;
	}

	/* 37 == 00 */
	return out;
}

void RoulettePlayerHistory::Update(uint8_t slot)
{
	state += state + slot;
	memmove(history+1, history, ROULETTE_HISTORY-1);
	history[0] = slot;
}

void RoulettePlayerHistory::Predict(uint8_t uncertainty, uint32_t* out)
{
	uint32_t bucket[38] = {0};
	uint8_t  shift = 64 - (uncertainty > 16 ? 16 : uncertainty);
	uint32_t iters = 1 << (uncertainty > 16 ? 16 : uncertainty);
	uint64_t stateBackup = state;

	while(iters--) {
		state = stateBackup ^ (((uint64_t) iters) << shift);
		bucket[Spin()]++;
	}

	state = stateBackup;
	memcpy(out, bucket, sizeof(bucket));
}

class RouletteCroupier
{
public:
	/* central repository of bet types, payouts and win criteria */
	typedef struct {
		uint64_t mask;
		char* name;
		uint8_t payout, probability;
	} BetType;
	std::vector<BetType> betTypes;

	int numOutside, numInside;

	typedef struct {
		uint32_t betType, betAmount;
	} Bet;
	std::vector<Bet> bets;

	RouletteCroupier(void);

	void PlaceBet(uint32_t type, uint32_t amount) {
		Bet b = {type, amount};
		bets.push_back(b);
	};

	uint32_t PlaceFullCompleteBet(uint8_t number, uint32_t scale) {
		uint32_t stake = 0;

		for(int i=numOutside; i < betTypes.size(); i++) {
			if((betTypes[i].mask >> number) & 1) {
				PlaceBet(i, betTypes[i].probability * scale);
				stake += betTypes[i].probability * scale;
			}
		}

		return stake;
	};

	uint32_t PayBets(uint8_t spin) {
		uint32_t pay = 0;

		while(!bets.empty()) {
			Bet b = bets.back();
			bets.pop_back();

			if((betTypes[b.betType].mask >> spin) & 1)
				pay += b.betAmount * (betTypes[b.betType].payout + 1);
		}

		return pay;
	};

	uint32_t BestInsideBet(uint32_t* wheelStats) {
		uint32_t bestBet = 0;
		double bestExpectedPayout = -999999999;

		for(uint32_t i=numOutside; i < numInside + numOutside; i++) {
			double expected = 0;

			for(int j=0; j < 38; j++) {
				if((betTypes[i].mask >> j) & 1)
					expected += betTypes[i].payout * wheelStats[j];
				else
					expected -= wheelStats[j];
			}

			if(expected > bestExpectedPayout) {
				bestExpectedPayout = expected;
				bestBet = i;
			}
		}

		return bestBet;
	};

	uint32_t BestOutsideBet(uint32_t* wheelStats) {
		uint32_t bestBet = 0;
		double bestExpectedPayout = -999999999;

		for(uint32_t i=0; i < numOutside; i++) {
			double expected = 0;

			for(int j=0; j < 38; j++) {
				if((betTypes[i].mask >> j) & 1)
					expected += betTypes[i].payout * wheelStats[j];
				else
					expected -= wheelStats[j];
			}

			if(expected > bestExpectedPayout) {
				bestExpectedPayout = expected;
				bestBet = i;
			}
		}

		return bestBet;
	};

	uint8_t BestFullCompleteBet(uint32_t* wheelStats) {
		uint8_t bestBet = 0;
		double bestExpectedPayout = -99999999999;

		for(int i=0; i < 38; i++) {
			double expected = 0;

			for(int j=0; j < 38; j++) {
				double wager = PlaceFullCompleteBet(i, 1);
				double payout = PayBets(j) - wager;
				expected += payout * wheelStats[j];
			}

			if(expected > bestExpectedPayout) {
				bestExpectedPayout = expected;
				bestBet = i;
			}
		}

		return bestBet;
	};
};

RouletteCroupier::RouletteCroupier(void)
{
	// outside bets first
	const uint64_t allNumbers = 0x3FFFFFFFFF;
	const uint64_t allNoZeros = 0x1FFFFFFFFE;

	BetType odds   = {  0xAAAAAAAAA, "Odds",   1, 18 };
	BetType evens  = { 0x1555555554, "Evens",  1, 18 };
	BetType manque = {      0x7FFFE, "Manque", 1, 18 };
	BetType passe  = { 0x1FFFF80000, "Passe",  1, 18 };
	BetType reds   = { 0x154AAD52AA, "Reds",   1, 18 };
	BetType blacks = {  0xAB552AD54, "Blacks", 1, 18 };

//	assert(odds  ^  evens == allNoZeros && !(odds  &  evens));
//	assert(manque ^ passe == allNoZeros && !(manque & passe));
//	assert(reds  ^ blacks == allNoZeros && !(reds  & blacks));

	betTypes.push_back(odds);
	betTypes.push_back(evens);
	betTypes.push_back(manque);
	betTypes.push_back(passe);
	betTypes.push_back(reds);
	betTypes.push_back(blacks);

	BetType p12   = {       0x1FFE, "P-12", 2, 12 };
	BetType m12   = {    0x1FFE000, "M-12", 2, 12 };
	BetType d12   = { 0x1FFE000000, "D-12", 2, 12 };
	BetType col1  = { 0x0492492492, "Col1", 2, 12 };
	BetType col2  = { 0x0924924924, "Col2", 2, 12 };
	BetType col3  = { 0x1249249248, "Col3", 2, 12 };
	BetType snake = { 0x0548895222, "Snake", 2, 12 };

//	assert(p12 | m12 | d12 == allNoZeros && !(p12 & m12) && !(m12 & d12) && !(p12 & d12));
//	assert(col1 | col2 | col3 == allNoZeros && !(col1 & col2) && !(col2 & col3) && !(col1 & col3));

	betTypes.push_back(p12);
	betTypes.push_back(m12);
	betTypes.push_back(d12);
	betTypes.push_back(col1);
	betTypes.push_back(col2);
	betTypes.push_back(col3);
	betTypes.push_back(snake);

	numOutside = betTypes.size();

	// now the inside bets

	for(int i=0; i < 38; i++) {
		BetType straight = { 1ULL << i, (char*) malloc(3), 35, 1 };
		if(i == 37)
			strcpy(straight.name, "00");
		else
			sprintf(straight.name, "%d", i);

		betTypes.push_back(straight);
	}

	BetType row     = { 0x2000000001, "Row 0+00",      17, 2 };
	BetType basket1 = { 0x0000000007, "Basket 0+1+2",  11, 3 };
	BetType basket2 = { 0x200000000C, "Basket 00+2+3", 11, 3 };
	BetType basket3 = { 0x2000000005, "Basket 0+00+2", 11, 3 };
	BetType topline = { 0x200000000F, "Top Line 0+00+1+2+3", 6, 5 };

	betTypes.push_back(row);
	betTypes.push_back(basket1);
	betTypes.push_back(basket2);
	betTypes.push_back(basket3);
	betTypes.push_back(topline);

	for(int i=1; i < 37; i += 3) {
		BetType split1 = { 3ULL << i, (char*) malloc(12), 17, 2 };
		BetType split2 = { 6ULL << i, (char*) malloc(12), 17, 2 };
		BetType street = { 7ULL << i, (char*) malloc(16), 11, 3 };

		sprintf(split1.name, "Split %d+%d", i, i+1);
		sprintf(split2.name, "Split %d+%d", i+1, i+2);
		sprintf(street.name, "Street %d+%d+%d", i, i+1, i+2);

		if(i > 3) {
			BetType split3  = {  9ULL << (i-3), (char*) malloc(12), 17, 2 };
			BetType split4  = { 18ULL << (i-3), (char*) malloc(12), 17, 2 };
			BetType split5  = { 36ULL << (i-3), (char*) malloc(12), 17, 2 };

			BetType corner1 = { 27ULL << (i-3), (char*) malloc(20), 8, 4 };
			BetType corner2 = { 54ULL << (i-3), (char*) malloc(20), 8, 4 };

			BetType sixline = { 63ULL << (i-3), (char*) malloc(16),  5, 6 };

			sprintf(split3.name,  "Split %d+%d", i-3, i);
			sprintf(split4.name,  "Split %d+%d", i-2, i+1);
			sprintf(split5.name,  "Split %d+%d", i-1, i+2);

			sprintf(corner1.name, "Corner %d+%d+%d+%d", i-3, i-2, i  , i+1);
			sprintf(corner2.name, "Corner %d+%d+%d+%d", i-2, i-1, i+1, i+2);

			sprintf(sixline.name, "Six-line %d-%d", i-3, i+2);

			betTypes.push_back(split3);
			betTypes.push_back(corner1);
			betTypes.push_back(split4);
			betTypes.push_back(corner2);
			betTypes.push_back(split5);
			betTypes.push_back(sixline);
		}

		betTypes.push_back(split1);
		betTypes.push_back(split2);
		betTypes.push_back(street);
	}

	numInside = betTypes.size() - numOutside;

	// also wheel-based bets - sod that for a lark
}

// Game parameters
#define FREE_BALANCE (25)
#define TABLE_MIN (1)
#define TABLE_MAX (100)
#define HIGHROLLER_FLOAT (100)
#define SPIN_TIME (1 * 1000000)
#define NUM_ACCOUNTS (100)
#define NUM_LOSERS (NUM_ACCOUNTS / 10.0)
#define NUM_WINNERS (NUM_ACCOUNTS / 10.0)

class DummyPlayer
{
public:
	static std::mutex cageMutex;
	static std::condition_variable cageQueue;
	static uint32_t cageBank;

	uint32_t bet, target, balance, compsEarned, uncertainty;
	int32_t  increment;
	const char* name;

	RouletteCroupier cr;

	DummyPlayer(const char* myName, uint32_t betSize, uint32_t targetFloat, int32_t targetIncrement)
	: name(myName),
	  bet(betSize),
	  target(targetFloat),
	  increment(targetIncrement)
	{
	};

	void operator() () {
		// If increment is positive, we're a "winner" and use inside bets.
		// If increment is negative, we're a "loser" and use outside bets.
		// If increment is zero, we're a "high roller":
		//		use inside bets until RNG state is known and float is adequate,
		//		then use "full complete" bets for maximum returns.
		balance = FREE_BALANCE;
		compsEarned = 0;
		uncertainty = 64;

		std::unique_lock<std::mutex> lock(cageMutex, std::defer_lock);

		// simulated connection to Lucky Deuce casino
		char* buffer = (char*) malloc(257);
		FILE* server = popen("yes \"\" | ./roulette", "r");
		assert(server);
		fgets(buffer, 256, server); // initial prompt

		// rigged RNG
		RoulettePlayerHistory wheel;

		// proper RNG
		FILE* rngSrc = fopen("/dev/random", "rb");

		printf("%s: joining the table.\n", name);

		while(1) {
			while(balance < bet) {
				// replenish float from the cage
				uint32_t wanted = bet;

				if(FREE_BALANCE > wanted)
					wanted = FREE_BALANCE;

				if(!increment && bet * 40 > wanted)
					wanted = bet * 40;

				wanted -= balance;

				lock.lock();
				if(cageBank > wanted) {
					printf("%s: taking $%u from the cage.\n", name, wanted);
					balance += wanted;
					target += wanted;
					cageBank -= wanted;
				} else {
					printf("%s: taking $%u from the cage.\n", name, cageBank);
					balance += cageBank;
					target += cageBank;
					cageBank = 0;
					cageQueue.wait(lock);
				}
				lock.unlock();
			}

			if(increment > 0 && balance > target && target > FREE_BALANCE) {
				uint32_t surplus = target - FREE_BALANCE;
				lock.lock();
				printf("%s: donating $%u to the cage.\n", name, surplus);
				cageBank += surplus;
				balance -= surplus;
				target -= surplus;
				lock.unlock();
				cageQueue.notify_all();
			}

			if(increment > 0 || (-increment) > target)
				target += increment;

			uint32_t threshold = bet;
			uint32_t effectiveUncertainty = uncertainty;

			if(balance > target)
				effectiveUncertainty += balance - target;
			if(increment < 0)
				effectiveUncertainty = 16;

			uint32_t stats[38] = {0};
			wheel.Predict(effectiveUncertainty, stats);

			// choose a bet to place
			if(increment > 0) {
				uint32_t betIndex = cr.BestOutsideBet(stats);

				printf("%s: betting $%u on %s.\n", name, bet, cr.betTypes[betIndex].name);

				cr.PlaceBet(betIndex, bet);
				balance -= bet;
			} else if(increment < 0) {
				uint32_t betIndex = cr.BestInsideBet(stats);

				printf("%s: betting $%u on %s.\n", name, bet, cr.betTypes[betIndex].name);

				cr.PlaceBet(betIndex, bet);
				balance -= bet;
			} else {
				if(!uncertainty && balance > 40*bet) {
					uint8_t betNumber = cr.BestFullCompleteBet(stats);

					printf("%s: betting %u to the *MAX*!\n", name, betNumber);

					balance -= cr.PlaceFullCompleteBet(betNumber, bet);
				} else {
					uint32_t betIndex = cr.BestInsideBet(stats);

					printf("%s: betting $%u on %s.\n", name, bet, cr.betTypes[betIndex].name);

					cr.PlaceBet(betIndex, bet);
					balance -= bet;
				}
			}

		again:
			// spin the wheel
			usleep(SPIN_TIME);
			fgets(buffer, 256, server);

			uint8_t spin = 0;

			if(!strcmp(buffer, "00\n")) {
				spin = 37;
			} else {
				char* e;
				spin = strtoul(buffer, &e, 10);
				if(spin > 36 || !e || *e != '\n' || !*buffer) {
					printf("%s: Mitä vittu?!?  '%s'\n", name, buffer);
					goto again;
				}
			}

			uint32_t payout = cr.PayBets(spin);
			if(payout)
				printf("%s: Spun %u, won $%u!  :D\n", name, spin, payout);
			else
				printf("%s: Spun %u, didn't win.  :(\n", name, spin);

			balance += payout;
			wheel.Update(spin);
			uncertainty = (uncertainty ? uncertainty-1 : 0);
		}
	};
};

std::mutex DummyPlayer::cageMutex;
std::condition_variable DummyPlayer::cageQueue;
uint32_t DummyPlayer::cageBank;

#ifdef TESTING

int main(void)
{
	RouletteCroupier cr;

	// calculate statistics of the full-complete bets
	int32_t wager[38] = {0}, balance[38][38] = {{0}};

	for(int i=0; i < 38; i++) {
		for(int j=0; j < 38; j++) {
			wager[i] = cr.PlaceFullCompleteBet(i, 1);
			balance[i][j] = cr.PayBets(j) - wager[i];
			printf("%8d ", balance[i][j]);
		}
		printf("\n");
	}

	uint8_t round[1000000];

	for(int i=999999; i >= 0; i--)
		round[i] = 0;

	RoulettePlayerHistory wheel;
	RoulettePlayerHistory better;

	for(int p=0; p < 1000000; p++) {
		int r=0;
		double b = 1000;

		fprintf(stderr, "\rplayer %d", p+1);

		while(b > 0 && r <= 64) {
			r++;

			uint8_t bet = better.Spin();
			better.Update(bet);

			uint8_t spin = wheel.Spin();
			wheel.Update(spin);

			b += balance[bet][spin];

		//	printf("\nbet=%u spin=%u return=%d balance=%g", bet, spin, balance[bet][spin], b);
		}

		round[p] = r;
	}
	fprintf(stderr, "\n");

	for(int r=1; r <= 64; r++) {
		int rr=0;

		for(int i=0; i < 1000000; i++)
			if(round[i] > r)
				rr++;

		printf("%d players lasted at least %d rounds\n", rr, r);
	}

	return 0;
}

#else

int main(void)
{
	std::vector<DummyPlayer> players;
	std::vector<std::thread> threads;
	int winners=0, losers=0, highrollers=0;

	players.reserve(NUM_ACCOUNTS);
	threads.reserve(NUM_ACCOUNTS);
	DummyPlayer::cageBank = 0;

	do {
		char* name = (char*) malloc(64);
		sprintf(name, "LuckyWinner%04u", winners+1);
		players.emplace_back(name, TABLE_MIN, FREE_BALANCE, TABLE_MIN);
		threads.emplace_back(players.back());
		usleep(SPIN_TIME);
	} while(++winners < NUM_WINNERS);

	while(++losers < NUM_LOSERS) {
		char* name = (char*) malloc(64);
		sprintf(name, "LuckyLoser%04u", losers);
		players.emplace_back(name, TABLE_MIN, FREE_BALANCE, -TABLE_MIN);
		threads.emplace_back(players.back());
		usleep(SPIN_TIME);
	}

	while(winners + losers + ++highrollers < NUM_ACCOUNTS) {
		do {
			usleep(SPIN_TIME * 10);
		} while(DummyPlayer::cageBank < TABLE_MAX * HIGHROLLER_FLOAT);

		char* name = (char*) malloc(64);
		sprintf(name, "HighRoller%04u", highrollers);
		players.emplace_back(name, TABLE_MAX, TABLE_MAX * HIGHROLLER_FLOAT, 0);
		threads.emplace_back(players.back());
	}

	return 0;
}

#endif
