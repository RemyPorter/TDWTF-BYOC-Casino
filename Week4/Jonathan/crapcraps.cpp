#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "sha256.h"

#define CRAPS_MIN_SIDES (4)
#define CRAPS_MAX_SIDES (250)
#define CRAPS_MIN_DICE (2)
#define CRAPS_MAX_DICE (32)
#define CRAPS_MAX_MAXVAL (1024)
#define CRAPS_MAX_COMBINATIONS (UINT64_MAX)

/* specific to craps table */
class CrapsTable
{
private:
	uint8_t  numSides, numDice;
	uint16_t minVal, maxVal, minPoint, maxPoint, minNatural, maxNatural, numPoints;
	uint64_t stats[CRAPS_MAX_MAXVAL+1];

	uint64_t state;
	uint8_t  dice[CRAPS_MAX_DICE];
	uint16_t result;
	uint16_t onPoint;

public:
			CrapsTable(int sides, int dice);

	static bool IsLegalCombination(int sides, int dice) {
		if(sides < CRAPS_MIN_SIDES || sides > CRAPS_MAX_SIDES)
			return false;
		if(dice < CRAPS_MIN_DICE || dice > CRAPS_MAX_DICE)
			return false;
		if(sides * dice > CRAPS_MAX_MAXVAL)
			return false;

		return pow(sides, dice) < CRAPS_MAX_COMBINATIONS;
	};

	bool	IsPoint(void) const {
		if(result >= minNatural && result <= maxNatural)
			return false;
		return result >= minPoint && result <= maxPoint;
	};

	bool	IsHardWay(void) const {
		uint8_t hard = dice[0];
		for(int i=1; i < numDice; i++)
			if(dice[i] != hard)
				return false;
		return true;
	};

	bool	IsNatural(void) const {
		return result >= minNatural && result <= maxNatural;
	};

	bool	IsPassWin(void) const {
		if(result == onPoint)
			return true;
		if(IsNatural())
			return !onPoint;
		if(result == maxVal)
			return false;
		if(result < minPoint)
			return false;
		if(result > maxPoint)
			return true;
		return false;
	};

	bool	IsPassLoss(void) const {
		if(result == onPoint)
			return false;
		if(IsNatural())
			return !!onPoint;
		if(result == maxVal)
			return true;
		if(result < minPoint)
			return true;
		if(result > maxPoint)
			return false;
		return false;
	};

	bool	IsDontPassWin(void) const {
		if(result == onPoint)
			return false;
		if(IsNatural())
			return !!onPoint;
		if(result == maxVal)
			return false;
		if(result < minPoint)
			return true;
		if(result > maxPoint)
			return false;
		return false;
	};

	bool	IsDontPassLoss(void) const {
		if(result == onPoint)
			return true;
		if(IsNatural())
			return !onPoint;
		if(result == maxVal)
			return false;
		if(result < minPoint)
			return false;
		if(result > maxPoint)
			return true;
		return false;
	};

	bool	IsDontPassPush(void) const {
		return result == maxVal;
	};

	uint16_t OnPoint(void) const { /* 0 means we're in come-out round */
		return onPoint;
	};

	uint16_t Roll(void);
	void PrintDice(void);
	void PrintStats(void);
};

CrapsTable::CrapsTable(int sides, int dice)
{
	if(!IsLegalCombination(sides, dice))
		throw;

	numSides = sides;
	numDice = dice;
	result = 0;
	onPoint = 0;

	/* set up configuration-specific game parameters */
	minVal = dice;
	maxVal = sides * dice;
	minPoint = minVal + dice;
	maxPoint = maxVal - dice;
	minNatural = (minVal + maxVal) / 2;
	maxNatural = minNatural + (maxVal + minVal) % 2;
	numPoints = (maxPoint - minPoint) - (maxNatural - minNatural);

	/* this is an excellent source of randomness, bruce says so */
	FILE* fp = fopen("/dev/random", "rb");
	fread(&state, sizeof(state), 1, fp);
	fclose(fp);

	for(int i=0; i < CRAPS_MAX_DICE; i++)
		this->dice[i] = 0;
	for(int i=0; i <= CRAPS_MAX_MAXVAL; i++)
		stats[i] = 0;

	/* calculate odds of each result arising */
	uint64_t combinations = stats[0] = 1;
	for(int i=0; i < numDice; i++) {
		for(int j=maxVal; j >= 0; j--) {
			stats[j] = 0;
			for(int k=j-1; k >= 0 && j-k <= sides; k--)
				stats[j] += stats[k];
		}
		combinations *= numSides;
	}
	stats[0] = combinations;
}

uint16_t CrapsTable::Roll(void)
{
	uint32_t rng=0, index=0;
	uint16_t out=0;
	uint8_t hash[SHA256::HashBytes];
	SHA256 sha256;

	/* millions of players at once... */
	/* that /dev/random is too slow to use every spin */
	/* cryptographic hash is effectively a random number, bruce says so */
	sha256.add(&state, sizeof(state));
	sha256.getHash(hash);

	for(int i=0; i < numDice; i++) {
		while(index < SHA256::HashBytes && index < i+sizeof(rng))
			rng = (rng << 8) + hash[index++];
		result = out += dice[i] = rng % numSides + 1;
		rng /= numSides;
	}

	/* initial state is totally random, so output sequence is different every time */
	state += state + out;
	return out;
}

void CrapsTable::PrintDice(void)
{
	for(int i=0; i < numDice; i++) {
		if(i)
			printf(" + ");
		printf("%u", dice[i]);
	}
	printf(" ==> %u", result);

	if(IsNatural() || IsPoint()) {
		printf("  (");
		if(IsHardWay())
			printf("Hard ");
		else if(result % numDice == 0) /* ie. if hard-way possible for this result */
			printf("Easy ");
		if(IsNatural())
			printf("Natural)");
		if(IsPoint())
			printf("%u)", result);
	}

	if(IsPassWin())
		printf("  (Pass Wins, ");
	if(IsPassLoss())
		printf("  (Pass Loses, ");

	if(IsDontPassWin())
		printf("Don't Pass Wins)");
	if(IsDontPassLoss())
		printf("Don't Pass Loses)");
	if(IsDontPassPush())
		printf("Don't Pass Pushes)");
	printf("\n");

	if(!onPoint) {
		/* come-out round */
		if(IsPoint()) {
			onPoint = result;
			printf("  *** Come Out - Point Established @ %u ***\n", onPoint);
		}
	} else {
		/* on-point round */
		if(IsNatural()) {
			printf("  *** Point @ %u Missed - Next Shooter ***\n", onPoint);
			onPoint = 0;
		} else if(result == onPoint) {
			printf("  *** Point @ %u Hit - Come Out Again ***\n", onPoint);
			onPoint = 0;
		}
	}
}

static uint64_t gcd(uint64_t a, uint64_t b)
{
	if(!b) return a;
	return gcd(b, a % b);
}

static void PrintRatio(uint64_t a, uint64_t b)
{
	double p = 100 * (double)(a) / (double)(b);
	b -= a;
	uint64_t d = gcd(a,b);
	printf("%llu:%llu (%.4f%%)", b/d, a/d, p);
}

static int Log2(uint64_t x)
{
	int l = 0;
	uint64_t y = 1;

	while(y && x > y) {
		l++;
		y <<= 1;
	}

	return l;
}

static void MultiplyReduceRatio(uint64_t aa, uint64_t ab, uint64_t ba, uint64_t bb, uint64_t& ca, uint64_t& cb)
{
	/* try to keep as much ratio precision as possible, by avoiding floating-point */
	uint64_t ad, bd, cd;

repeat:
	ad = gcd(aa,ab);
	bd = gcd(ba,bb);
	aa /= ad;
	ab /= ad;
	ba /= bd;
	bb /= bd;

	ad = gcd(aa,bb);
	bd = gcd(ba,ab);
	aa /= ad;
	bb /= ad;
	ba /= bd;
	ab /= bd;

	if(Log2(aa) + Log2(ba) > 64 || Log2(ab) + Log2(bb) > 64) {
		/* not enough integer precision */
		if(aa > ba)
			aa /= 2;
		else
			ba /= 2;
		if(ab > bb)
			ab /= 2;
		else
			bb /= 2;
		goto repeat;
	}

	ca = aa * ba;
	cb = ab * bb;

	/* final reduction */
	cd = gcd(ca,cb);
	ca /= cd;
	cb /= cd;
}

static void AddReduceRatio(uint64_t aa, uint64_t ab, uint64_t ba, uint64_t bb, uint64_t& ca, uint64_t& cb)
{
	/* try to keep as much ratio precision as possible, by avoiding floating-point */
	uint64_t ad, bd, cd;

	if(ab == bb && (aa + ba) >= aa)
		goto skip; /* correction not required */

repeat:
	ad = gcd(aa,ab);
	bd = gcd(ba,bb);
	aa /= ad;
	ab /= ad;
	ba /= bd;
	bb /= bd;

	if(Log2(aa) + Log2(bb) > 64 || Log2(ba) + Log2(ab) > 64 || Log2(ab) + Log2(bb) > 64) {
		/* not enough integer precision */
		if(aa > ba) {
			aa /= 2;
			ab /= 2;
		} else {
			ba /= 2;
			bb /= 2;
		}
		goto repeat;
	}

	if(ab != bb) {
		aa *= bb;
		ba *= ab;
		ab = bb = ab * bb;
	}

skip:
	ca = aa + ba;
	cb = ab;

	/* final reduction */
	cd = gcd(ca,cb);
	ca /= cd;
	cb /= cd;
}

void CrapsTable::PrintStats(void)
{
	printf("Statistics for %ud%u craps:\n", numDice, numSides);
	printf("Total dice combinations: %llu\n", stats[0]);

	if(minNatural == maxNatural)
		printf("Natural is %u\n", minNatural);
	else
		printf("Naturals are %u, %u\n", minNatural, maxNatural);

	if(numDice == 2) {
		printf("Pass wins on natural%s and %u; loses on %u, %u, and %u\n",
			minNatural == maxNatural ? "" : "s", maxPoint+1, minVal, minVal+1, maxVal);
		printf("Don't Pass wins on %u, %u; loses on natural%s and %u; pushes on %u\n",
			minVal, minVal+1, minNatural == maxNatural ? "" : "s", maxPoint+1, maxVal);
	} else {
		printf("Pass wins on natural%s and %u-%u; loses on %u-%u and %u\n",
			minNatural == maxNatural ? "" : "s", maxPoint+1, maxVal-1, minVal, minPoint-1, maxVal);
		printf("Don't Pass wins on %u-%u; loses on natural%s and %u-%u; pushes on %u\n",
			minVal, minPoint-1, minNatural == maxNatural ? "" : "s", maxPoint+1, maxVal-1, maxVal);
	}

	printf("Point values are: %u", minPoint);
	for(int i=minPoint+1; i <= maxPoint; i++) {
		if(i >= minNatural && i <= maxNatural)
			continue;
		printf(", %u", i);
	}
	printf("  (%u in all)\n", numPoints);

	printf("\nIndividual results:");
	for(int i=minVal; i <= minNatural; i++) {
		printf("\n\t%4u | %4u:\t", i, (maxVal - i) + minVal);
		PrintRatio(stats[i], stats[0]);

		if(!(i % numDice)) {
			printf("\thard way: ");
			PrintRatio(1, stats[0]);
		}
	}
	printf("\n");

	uint64_t cdp=0, cp=0, cn=0, co=0, pp=0, pdp=0, tpa=0, tdpa=0, tpb=1, tdpb=1, push=stats[maxVal];
	uint64_t ppa[CRAPS_MAX_MAXVAL/2] = {0};
	uint64_t ppb[CRAPS_MAX_MAXVAL/2] = {0};
	uint64_t ppc[CRAPS_MAX_MAXVAL/2] = {0};
	uint64_t ppd[CRAPS_MAX_MAXVAL/2] = {0};

	printf("\nNatural (on each roll): ");
	for(int i=minNatural; i <= maxNatural; i++) {
		cn += stats[i];
		cp += stats[i];
		pdp += stats[i];
	}
	PrintRatio(cn, stats[0]);

	printf("\nPass (on each come-out roll): ");
	for(int i=maxPoint+1; i < maxVal; i++) {
		cp += stats[i];
		pp += stats[i];
	}
	PrintRatio(cp, stats[0]);

	printf("\nDon't Pass (on each come-out roll): ");
	for(int i=minVal; i < minPoint; i++) {
		cdp += stats[i];
		pdp += stats[i];
	}
	PrintRatio(cdp, stats[0]);

	printf("\nPass Rate (for each point established):");
	for(int i=minPoint; i < minNatural; i++) {
		co += stats[i] * 2;

		MultiplyReduceRatio(stats[i]*2, stats[0], stats[i] + pp, pp + pdp + stats[i], ppa[i], ppb[i]);
		MultiplyReduceRatio(stats[i]*2, stats[0] - push,    pdp, pp + pdp + stats[i], ppc[i], ppd[i]);
		AddReduceRatio(ppa[i], ppb[i], tpa,  tpb,  tpa,  tpb);
		AddReduceRatio(ppc[i], ppd[i], tdpa, tdpb, tdpa, tdpb);

		printf("\n\t%4u | %4u:\t", i, (maxVal - i) + minVal);
		PrintRatio(stats[i], cn + stats[i]);
	}

	printf("\nCome Out (on each come-out roll): ");
	PrintRatio(co, stats[0]);

	printf("\nTotal Pass Rate: ");
	AddReduceRatio(cp,  stats[0], tpa,  tpb,  tpa,  tpb);
	PrintRatio(tpa, tpb);

	printf("\nTotal Don't Pass Rate: ");
	AddReduceRatio(cdp, stats[0] - stats[maxVal], tdpa, tdpb, tdpa, tdpb);
	PrintRatio(tdpa, tdpb);

	uint64_t hea=0, heb=1;
	printf("\nHouse Edge (where Pass & Don't Pass both lose): ");
	AddReduceRatio(tpa, tpb, tdpa, tdpb, hea, heb);
	hea = heb - hea;
	PrintRatio(hea, heb);

	printf("\n\nMulti-roll bets:");

	printf("\nPlace:");
	for(int i=minPoint; i < minNatural; i++) {
		printf("\n\t%4u | %4u:\t", i, (maxVal - i) + minVal);
		PrintRatio(stats[i], stats[i] + cn);
	}

	printf("\nLay (any): ");
	PrintRatio(cn, co + cn);

	printf("\nHard Ways:");
	for(int i=minPoint; i < minNatural; i += numDice) {
		printf("\n\t%4u | %4u:\t", i, (maxVal - i) + minVal);
		PrintRatio(1, stats[i] + cn);
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	int dice, sides;

	if(argc != 3) {
		fprintf(stderr, "Please specify number of dice and number of sides.\n");
		return 1;
	}

	dice = atoi(argv[1]);
	sides = atoi(argv[2]);

	if(!CrapsTable::IsLegalCombination(sides, dice)) {
		fprintf(stderr, "Sorry, can't cope with that combination.  Here's what I *can* do:\n");
		fprintf(stderr, "There must be at least 2 dice with at least 4 wides each...\n");
		int oldSides = 250;
		for(dice=CRAPS_MIN_DICE; dice <= CRAPS_MAX_DICE; dice++) {
			for(sides=CRAPS_MIN_SIDES; CrapsTable::IsLegalCombination(sides, dice); sides++)
				;
			if(sides-1 != oldSides) {
				fprintf(stderr, "\tUp to %d dice with up to %d sides\n", dice-1, oldSides);
				oldSides = sides-1;
			}
		}
		return 1;
	}

	CrapsTable table(sides, dice);

	table.PrintStats();

	printf("Press Return to roll the dice!\n");

	while(1) {
		while(getchar() != '\n')
			;

		table.Roll();
		table.PrintDice();
	}

	return 0;
}
