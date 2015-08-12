The code delivered is everything except roulette-cheat.cpp.  Splitting roulette.cpp into header and implementation is left as an exercise for the integration team (which, incidentally, also serves as plausible deniability - the code running in a complete gambling system is unlikely to be *precisely* what I delivered).

To compile, use a Linux or MacOS X machine:

        g++ -O -DTESTING roulette.cpp sha256.cpp -o roulette

This executable takes no arguments, and you simply press Return (as prompted) for every spin of the wheel.

The implementation is essentially a PRNG based around a 64-bit state variable which is seeded from /dev/random, then fed through SHA256.  The intent is that this looks like a professional implementation with at least some attempt at best practice.  The output is statistically random (within the limits imposed by the spec), and not at all a-priori predictable.  There are some small biases towards low numbers, but these are swamped by the house edge and the compromises in the spec.  As far as the house and ordinary gamblers are concerned, this is a “fair wheel” within the tolerances of the traditional mechanical version.

The exploitable flaw is that the value of the internal state variable can eventually be deduced from the output, and is completely known after exactly 64 spins.  The cheat program takes the pseudo-random output as input, and performs this deduction.  When the state is as yet incompletely known, it iterates over (a subset of) the possible values of the state, at first showing only the inherent biases of the system as specified, but eventually giving probabilities that swamp the house edge and then become certainty.

        g++ -O roulette-cheat.cpp sha256.cpp -o roulette-cheat -lreadline

Winning a little over 33% or 50% of the time is not obviously suspicious with outside bets, as it would be with single or line bets.  The output of the cheat program is not a precise prediction of which number will be next (even though it could be).  Instead, it offers a guide to which of the standard “outside bets” is strongest.

It is only necessary to beat the house edge, so I would be wise to avoid taking the cheat program’s advice on *every* spin, but to roll a D20 and deliberately make an “off” bet some of the time.  The losses associated with this can be mitigated by following the “martingale” or some other well-known system which increases payoff on an eventual win, in the certainty that I can *choose* to win at any moment before running out of money.

Other methods of evading suspicion are left as an exercise to the reader.