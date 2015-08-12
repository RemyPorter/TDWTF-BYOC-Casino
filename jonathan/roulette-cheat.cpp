#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

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
};

RoulettePlayerHistory::RoulettePlayerHistory(void)
{
	memset(history, 0, ROULETTE_HISTORY);
	state = 0;
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

int main(void)
{
	RoulettePlayerHistory player;
	uint8_t uncertainty = 64;

	while(1) {
		uint32_t bucket[38] = {0};
		uint8_t  shift = 64 - (uncertainty > 20 ? 20 : uncertainty);
		uint32_t iters = 1 << (uncertainty > 20 ? 20 : uncertainty);
		uint64_t stateBackup = player.state;
		double denominator = 100.0 / iters;

		while(iters--) {
			player.state = stateBackup ^ (((uint64_t) iters) << shift);
			bucket[player.Spin()]++;
		}

		uint32_t evens=0, odds=0, col1=0, col2=0, col3=0, lows=0, highs=0, tw1=0, tw2=0, tw3=0, snake=0, reds=0, blacks=0;

		for(int x=1; x < 37; x += 2) {
			odds  += bucket[x];
			evens += bucket[x+1];
		}
		for(int x=1; x < 37; x += 3) {
			col1 += bucket[x];
			col2 += bucket[x+1];
			col3 += bucket[x+2];
		}
		for(int x=1; x < 13; x++)
			tw1 += bucket[x];
		for(int x=13; x < 25; x++)
			tw2 += bucket[x];
		for(int x=25; x < 37; x++)
			tw3 += bucket[x];
		for(int x=1; x < 19; x++)
			lows += bucket[x];
		for(int x=19; x < 37; x++)
			highs += bucket[x];
		snake = bucket[ 1] + bucket[ 5] + bucket[ 9] +
		        bucket[12] + bucket[14] + bucket[16] +
		        bucket[19] + bucket[23] + bucket[27] +
		        bucket[30] + bucket[32] + bucket[34];
		reds = bucket[ 1] + bucket[ 3] + bucket[ 5] +
		       bucket[ 7] + bucket[ 9] + bucket[12] +
		       bucket[14] + bucket[16] + bucket[18] +
		       bucket[19] + bucket[21] + bucket[23] +
		       bucket[25] + bucket[27] + bucket[30] +
		       bucket[32] + bucket[34] + bucket[36];
		blacks = bucket[ 2] + bucket[ 4] + bucket[ 6] +
		         bucket[ 8] + bucket[10] + bucket[11] +
		         bucket[13] + bucket[15] + bucket[17] +
		         bucket[20] + bucket[22] + bucket[24] +
		         bucket[26] + bucket[28] + bucket[29] +
		         bucket[31] + bucket[33] + bucket[35];

		printf("Even: %3.1f  Odd: %3.1f  Manque: %3.1f  Passe: %3.1f  Red: %3.1f  Black: %3.1f  "
			   "P-12: %3.1f  M-12: %3.1f  D-12: %3.1f  C1: %3.1f  C2: %3.1f  C3: %3.1f  Snake: %3.1f\n",
				evens * denominator, odds * denominator, lows * denominator, highs * denominator, reds * denominator, blacks * denominator,
				tw1 * denominator, tw2 * denominator, tw3 * denominator, col1 * denominator, col2 * denominator, col3 * denominator, snake * denominator);

again:
		char* input = readline(": ");
		uint8_t spin = 0;

		if(!strcmp(input, "00")) {
			spin = 37;
		} else {
			char* e;
			spin = strtoul(input, &e, 10);
			if(spin > 36 || !e || *e || !*input) {
				printf("Mitä vittu?!?\n");
				goto again;
			}
		}

		player.state = stateBackup + stateBackup + spin;
		memmove(player.history+1, player.history, ROULETTE_HISTORY-1);
		player.history[0] = spin;
		uncertainty = (uncertainty ? uncertainty-1 : 0);

		free(input);
	}

	return 0;
}
