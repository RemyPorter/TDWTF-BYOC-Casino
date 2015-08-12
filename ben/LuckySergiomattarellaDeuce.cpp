/*
 * My entry for the 'Lucky Deuce' contest on thedailywtf.com.
 * Author: Ben Wiederhake (Ben.Wiederhake+Lucky.Deuce@gmail.com)
 * Do whatever you want with these files and algorithms, but I'd be
 * happy to see my name still on it.
 *
 * For an explanation why this is "broken", see the accompanying README.md.
 *
 * Running on Linux with gcc version 4.9.3 (Debian 4.9.3-3),
 * but should be portable to anything with <random> on the include path.
 *
 * Compile for French roulette (no '00' field):
 * g++ -std=c++11 -o lsd LuckySergiomattarellaDeuce.cpp
 * Compile for American roulette (has a '00' field, acting like a second '0'):
 * g++ -std=c++11 -DEUCE_DOUBLE_ZEROS -o lsd LuckySergiomattarellaDeuce.cpp
 *
 * Run: ./lsd
 */

#include <iostream>
#include <list>
#include <random>

/* UPPER_LIMIT is required to define the range of the uniform distribution.
 * It is the highest legal number. In other words: [0, UPPER_LIMIT], inclusive. */
#ifdef EUCE_DOUBLE_ZEROS
const uint UPPER_LIMIT = 37;
#else
const uint UPPER_LIMIT = 36;
#endif

/* Encapsulated for easy unit testing.
 * Note that this is either true random or the Mersenne Twister,
 * and passes the Diehard tests. */
class FairWheel {
    std::random_device prng;
    /* Ugh. Explicit constructor in the way. */
    typedef std::uniform_int_distribution<uint> distrib_t;
    distrib_t wheel = distrib_t(0, UPPER_LIMIT);

public:
    uint operator()() {
        return wheel(prng);
    }
};

/* Encapsulated for easy replacement by FairWheel in main().
 * Note that it's pointless to run entropy tests on this,
 * as this is *by specification* not truly random. */
class FewRunsWheel {
    FairWheel backing;
    std::list<uint> recentResults = {999, 999, 999};
    bool isRun = false;

    /* Update 'isRun' field. */
    void digest(const uint result) {
        isRun = recentResults.back() == result;

        recentResults.pop_front();
        recentResults.push_back(result);
    }

public:
    /* Poll the next number, taking care to avoid runs. */
    uint operator()() {
        uint generated = backing();

        if (isRun) {
            /* Oh my goodness! Avoid a third occurrence at all cost! */
            const uint avoided = ~recentResults.back();
            /* Make sure the fourth-least significant bit
             * is different from the last result. All other bits are random,
             * but we know it'll definitely not be identical *again*.
             * Why the fourth? I expect "professional" roulette players to
             * closely observe some properties, including even/odd (lsb),
             * and let's assume they even observe "value mod 4" (second-lsb)
             * and "value mod 8" (third-lsb). */
            const uint mask = 1 << 3; /* Fourth bit, so shift '1' by three. */
            const uint a = avoided & ~mask; /* Take the opposite bit. */
            const uint b = generated & mask; /* Take from randomness. */
            generated = a | b; /* Merge. */

            /* If we switched a 0 to a 1, it's possible that we left
             * the valid range. If so, wrap around. */
            generated %= UPPER_LIMIT;

            /* See README, section "Why this is secure" for more information. */
        } else if (recentResults.back() == generated) {
            /* We would trigger a second occurrence.
             * Make it unlikely *but possible* by rolling again. */
            generated = backing();
        }

        /* Remember the result. */
        digest(generated);

        return generated;
    }
};

void printWheelResult(const uint result) {
#ifdef EUCE_DOUBLE_ZEROS
    if (UPPER_LIMIT == result) {
        std::cout << "00" << std::endl;
        return;
    }
#endif
    std::cout << result << std::endl;
}

int main() {
    FewRunsWheel wheel;

    while (true) {
        printWheelResult(wheel());
    }

    return 0;
}
