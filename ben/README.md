<!-- Markdown, version 1.0.1. -->

All about LuckySergiomattarellaDeuce
====================================

## Name and identity ##

Once upon a time, I read your great "Lucky Deuce" challenge, and would 
like to participate. I wanted to give it a "unique" name, so naturally 
UUIDs would have been a good fit. But alas, UUIDs are also ugly. So I 
used a highly sophisticated algorithm to come up with a somewhat unique 
name: "Umm, hey Google, tell me an Italian mafia name starting with S 
so I can abbreviate the name of my program to LSD because I'm feeling 
funny."
  And that's why the program is now named after an Italian politician, 
lawyer, and judge. All coincidences are due to Google.

## Truth and deceit ##

Obviously, at least some parts of the comments in the code must be 
written in-universe, that is: They must pretend that there is no 
cheat. But just as obviously, I have to break this illusion at some 
point, and explain what the cheat is and why I can exploit it, with 
barely anyone else noticing.
  I think I found a simple way to draw the line:
The complete file "LuckySergiomattarellaDeuce.cpp" is written 
in-universe (and thus contains explanation that may or may not be 
deceiving). The sections "The sales pitch" and "Why this is secure" in 
this file are also in-universe. The rest of this file is actually 
veritable, especially the sections "Why this is NOT 
secure" and "Why nobody stumbles upon the secret".

## The sales pitch ##

The given implementation is ideal to run as an embedded program. It 
outputs a steady stream of roulette spins. The output can be piped into 
the next module, letting the OS take care of all buffering and 
whatsoever.
  The core random generator is `std::random_device`. This is always at 
least as secure as the Mersenne Twister. The generated numbers are then 
filtered, so that a duplicate result is very unlikely (in fact: as 
likely of getting the same result on a fair wheel three times in a 
row), and a run of length three is made impossible by diagonalization.
  A simple compilation flag allows you to specify whether a French
wheel or an American wheel is desired.

## Why this is secure ##

The implementation follows the "Not My Fault" pattern: Every single 
component is small and well-documented. If anything goes wrong it 
"can't be my fault", as most functionality is provided by the 
well-tested standard library.

Since the core random generator is always at least as secure as the 
Mersenne Twister (and MT19937 can be easily enforced using another 
compilation flag), this analysis is going to assume that MT19937 is 
being used.

The following attack vectors exist:

1. Guess the initial state of MT19937.
2. Attack on MT19937.
3. Access hardware/software directly (e.g. read program memory).
4. Guess the state of MT19937 by observing the outputs.
5. Exploit shift in probabilities caused by removal of duplicates.

Points 1 through 3 are outside the scope of this module, and would 
require additional hardware (or at least significant help from the 
Operation System) to avoid.

#### "Guess the state of MT19937" - avoided! ####

As Wikipedia tells us, 624 observed 32-bit results are sufficient to 
compute the state of MT19937.  
However, this attack vector is thwarted, because:

- A single spin has at most 38 (American) or 37 (French) possible 
  outcomes. This means the attacker learns at each spin a bit less than 
  5.25 bits about the state. Now the attacker has to observe 3804 
  spins! This alone would require significant amounts of work, and 
  would still not lead to success because:
- Removal of duplicates obscures the true results of MT19937. Let's 
  look at the sequence `(12, 23)`. Even if the attacker has access to 
  the source code and knows how it works, it is impossible to say from 
  the output alone whether MT19937 generated precisely that sequence, 
  or `(12, 12, 23)`, and the duplicate 12 was swallowed by the 
  run-removal.

Thus the attacker not only has to acquire a very large amount of 
samples, but also has to solve the probabilistic problem of determining 
where the run-removal might have kicked in.

#### "Exploit shift in probabilities" - avoided! ####

An attacker might learn that this roulette wheel is not, in fact, fair. 
The worst-case manifestation of this is: The attacker reads and 
understands the source code perfectly, and knows precisely how 
duplicates are avoided.

The first difference is that a second occurrence of a spin result is 
made unlikely. Let `n=37` in case of French wheels and `n=38` for 
American wheels. Then a second occurrence of a number requires that the 
fair wheel produces the first number twice. The probability of this is 
`1 / (n*n)`. This boosts the probability of the other results. Let's 
assume (for the advantage of the attacker) that the probability of a 
duplicate is decreased to 0; boosting the probability of each other 
result to `1 / (n-1)`. Then the house edge is either reduced (American 
wheel) or precisely annihilated (French wheel).  
  So in the actual scenario (where duplicates are still possible), the 
house edge is at least partially preserved. So the attacker can, at 
best, reduce his losses by an insignificant amount, but (still) can't 
possibly win any money in the long run.

The second difference arises from the impossibility of a third 
occurrence. So if the attacker observes a duplicate (which only happens 
with probability `1 / (n*n)`), he knows that this number will not 
appear again. Due to the diagonalization (the third-least-significant 
bit is precisely opposite to the previously repeated number; all other 
bits are randomly generated by the MT19937), the attacker knows even 
more: Only roughly `n/2` of all possible results are then possible for
the third spin!  
  This means that in this very rare scenario, the attacker can guess 
any number from the remaining possibilities and bet a single unit of 
money. Then his probability of winning is `1/(n/2) = 2/n`. His average 
payout for this scenario is `36*2/n + (-1)*(n-2)/n = 74/n - 1`. On a 
French wheel this is precisely 1, on an American wheel slightly less.  
  However, this scenario presents itself only `1/(n*n)` of the time. In
all other scenarios the attacker (which should participate and again 
bet one unit of money, otherwise he can be detected and be detained) 
has an average payout of `(36-n)/n`. Using this strategy yields a 
long-term profit of roughly:

    ((n*n-1)/(n*n)) * (36-n)/n + (1/(n*n)) * (74/n - 1)

For a French wheel, this evaluates to roughly -0.0263, for an American 
wheel -0.0519. Note that this is not only negative, but also very close 
to the average payout of a fair (ignorant) player: -0.027 and -0.053, 
respectively.

So in summary: Exploiting the first property is not profitable. 
Exploiting the second property is only profitable if the attacker makes 
it really, really obvious by placing bets at least 40 (French) or 85 
(American) times their usual bet. Even if he exploits the "wrapping" 
really well (reducing the result space to `n/3`, which doesn't happen), 
the factors are at least 25 (French) and 45 (American). Please note 
that this "flaw" is due to the very requirement, and not a bug.

## Why this is NOT secure ##

**THIS SECTION IS A SPOILER!**

I strongly recommend that you *first* read the source code and try to 
find the "cheat" on your own.

Let's look at the following lines again:

    const uint mask = 1 << 3;
    const uint a = avoided & ~mask;
    const uint b = generated & mask;
    generated = a | b;

The comments (intentionally omitted) talk about how this takes the 
negated fourth bit of the number-to-avoid (= the fourth bit of 
`avoided`) and merges it with the "other" bits from the truly random, 
freshly generated value. Here is the code that actually would do that 
(note the `~` operator):

    const uint mask = 1 << 3;
    const uint a = avoided & mask;
    const uint b = generated & ~mask;
    generated = a | b;

Now instead of taking *one* bit of `avoided`, it actually ends 
up using the opposite *all except one* bit of `avoided`, restricting the 
result space to 2!

But didn't the last section clearly show that this can't be exploited? 
No. Not at all actually. In fact, the "Exploit shift in probabilities" 
section only looks at the first exploit XOR the second exploit, but not 
what happens if both are combined. The difference is not too important: 
The factors (special bet divided by normal bet) to reach an average 
payout of 101% is still 16 (French) and 61 (American) for `n/2` result 
space, and 8 (French) and 30 (American) for `n/3` result space. That 
would still obvious enough to get caught ... ignoring the fact that this 
exploit is impossible because the algorithm doesn't actually work that 
way.

Now that you know what the code actually does, how can you exploit 
this? Whenever a duplicate comes along, compute `fix=n-duplicate`. Then 
memorize the following table:

    diff[0] = -8
    diff[1] = +8
    diff[2] = -8
    diff[3] = +8
    diff[4] = -8

Now you can compute `other=(diff[duplicate/8] + fix) % (n + 1)`. Note that `fix` 
and `other` are the *only* two possible results the wheel can possibly 
have now. If (and only if) `diff[i] > 0`, then `fix` has slightly 
higher chances to win, with odds being `n`-16 to 16. Otherwise the odds 
are 16 to `n`-16.

You don't want to pick always the same number, so at the table you 
should take care to not only switch between `fix` and `other`, but also 
"explain" an imaginary (wrong) sequence, just like all players there 
follow their own system. Remember to fail often enough, and be 
"surprised" that your system doesn't reliably work. (Otherwise the pit 
boss will let you walk out with neither money nor unbroken bones.)

Sadly, you still have to wager a bit more money during duplicates. On a 
French wheel with the above "accidents", betting double money (= factor 
2) during duplicates means your average payout over the whole game 
becomes roughly 102%. On an American table, the factor must be 5 for 
the same effect.

I'm not going to explain how these numbers were achieved. 
<!--=(B$2+1)*(37-B$2)*POW(B$2,-2)+$B$1*POW(B$2,-3)*(36*$A6-B$2)-POW(B$2,-2)--> 
If someone's interested, contact me. For this challenge, I just want to 
beat and invert the house edge.

Then again, I'm bad at maths. I'm sure I overlooked some kind of corner 
case, or did some rookie stochastic mistake. But it should be clear 
that in the case of duplicates, one can make a lot of money, and avoid 
losses in the case of non-duplicates.

#### Different way of breaking ####

Initially, the "hook" was a different one:

    const uint mask = 1 << 4-1;

With this notation, I hoped everyone would be fooled into thinking that 
binary minus has a stronger precedence than bit-wise left shift. Turns 
out, I only fooled myself: The numerical precedence listed by
[the reference][] is indeed higher, but higher doesn't actually mean 
stronger. Oh well, it was worth a try.

[the reference]: http://en.cppreference.com/w/cpp/language/operator_precedence

## Why nobody stumbles upon the secret ##

**THIS SECTION IS ANOTHER SPOILER!**

I strongly recommend that you *first* read the source code and try to 
find the "cheat" on your own.

#### The gamblers ####

First of all, the gambler has to notice that runs are especially 
unlikely with this wheel. Maybe he is made aware by his peers that this 
wheel is "especially random". I think it's plausible to assume this.

Then, J. Random Gambler has to understand that the "few duplicates" 
property itself is not exploitable *at all*, no matter which way you 
look at it: No matter whether it's French or American wheel, the true 
implementation or even the described "avoidance" algorithm.

J. Random Gambler might even look over my shoulder, and observe that in 
the case of duplicates, I seem to have a good "gut feeling" or at least 
a very good system at winning. But to prove this empirically is very 
hard, as, by construction, duplicates are very sparse. And then there's 
the inevitable difficulty of grasping the linear-with-modulo 
correlation. Even if J. Random Gambler observes all duplicates on the 
casino floor, it's going to take a lot of time of observation and wits, 
until she can possibly see that pattern. I think then it's plausible to 
assume that she is then clever enough to keep it a secret.

#### Code review, or: Obfuscation by clarity ####

All the code is well-documented. All the comments explain what and why 
something is doing something. Bit operations are hard, and the `a` and 
`b` variables look difficult, so reviewers will (hopefully) either:

- Not even assume something is amiss

- Assume that something is fishy, read the comments and believe them

- Assume that something is fishy, distrust the comments, and run it a 
few times, only to see that the output is varying indeed. You can 
easily do that by yourself if you duplicate the `digest(generated);` 
call. When viewing at only a few results, the linear-with-modulo 
relation isn't that obvious. Sadly, after a few hundred results, it 
starts to become obvious that something fishy is going on. (Note that 
with the "different way of breaking", this wouldn't be a problem.)

- Run it in a controlled setup, plotting all the combinations. In this 
case, and only if the reviewer understands the pattern and its 
exploitability, then I'm in trouble.

So I'm going to assume that the reviewer is similarly paid and 
bleary-eyed like I am. In-universe of course. I'm not typing this at 2 
am in the morning.

#### The lifeline: Plausible deniability ####

And then the final day has arrived. I'm swimming in the pool of my 
villa, when big scary thugs come along, and start "questioning" me. Or, 
I hope, take me to someone who does the questioning without quotes 
around it.

"What?!", I could exclaim, "Oh no! I didn't know such a small typo 
could possibly have that effect!"

I hope this is plausible.  
  At least I can then spend the rest of my life in my beloved swimming
pool, either with martinis in each hand, or, if not plausible, with 
cement shoes.

## Credits ##

I, Ben Wiederhake, am the sole author of LuckySergiomattarellaDeuce, 
and came up on my own about how to creating and hide that algorithmic 
error. Do whatever you want with these files and algorithms. I'd be 
happy to hear from you if you think it was entertaining and worth the 
read :)
