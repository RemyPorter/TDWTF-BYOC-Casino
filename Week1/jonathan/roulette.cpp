#include <stdint.h>
#include <stdio.h>

#include "sha256.h"

#define ROULETTE_HISTORY (64)

/* specific to player */
class RoulettePlayerHistory
{
private:
	uint8_t history[ROULETTE_HISTORY];
	uint64_t state;

public:
			RoulettePlayerHistory(void);
	uint8_t Spin(void);
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

	/* initial state is totally random, so output sequence is different every time */
	state += state + out;
	memmove(history+1, history, ROULETTE_HISTORY-1);
	history[0] = out;

	/* 37 == 00 */
	return out;
}

#ifdef TESTING
int main(void)
{
	RoulettePlayerHistory player;

	printf("Press Return to spin!\n");

	while(1) {
		while(getchar() != '\n')
			;

		uint8_t spin = player.Spin();
		if(spin == 37)
			printf("00\n");
		else
			printf("%2u\n", spin);
	}

	return 0;
}
#endif
