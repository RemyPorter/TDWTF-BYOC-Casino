In a world full of ISOs, IECs, IETF, IEEE, YAC (yet another acronym), I end up having to type more code comments than actual code and to adhere to strict coding conventions.

I present you with my submission, where the actual needed lines are +/- 20, in a file with 125 lines.

All the code follows the official "PEP 0008 -- Style Guide for Python Code" plus some adapted naming conventions (based loosely in what I've to deal with).

As for the cheat, it is just a simple algorithm design bug when handling the requirement of no "runs" -> when detecting a number twice, instead of twice in a row, (oops) the next number will be the next in clockwise order, instead of random selection (double oops). 

"I swear it was an innocent mistake! With all this 'needed' clutter, didn't notice it myself!"

To run it: Python 2.7.x, either directly (it will prompt for sequence size) or in interactive mode using the exported "spin" function (returns an iterator)
