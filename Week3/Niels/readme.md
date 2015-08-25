# In the cards #
## Introduction ##
Code written for entry of the DailyWTF contest LuckyDeuceCasino, see 
http://thedailywtf.com/articles/the-lucky-deuce-getting-in-the-slot

## Features ##
What the code does is giving the girl (let's call het Alice) 
the last five aces that are available in the deck. The
deck gets reshuffeled after all cards are given out.
But, as only Alice wants to get this trick, it is required that 
players enter their name (as such, it could be nice that we can call their name).
The cheat is only triggered when Alice enters here name as MAlice.
The rewrite of the values are obscured in a bogus if statement in the CheckValue method.

## To run ##
Just run the LuckyDeuceCasino.exe. Press any key after each round to enter the next round.

## *Spoiler* ##
The deck will give cards at random, but it counts the given aces. As that number is
greater than 15, it will replace the cards in the MAlice hand with aces.
We know it is MAlice as we sum the chars of her name (and it sums to 555).
We bomb Alice because:
- we might have an off by one error in the code (those hard problems in CS!) 
or the last hand was given more than one ace. So the 
the cheat is triggered when >16 aces are given out. Count the aces and you will 
find that there were more than 20 aces given before the reshuffle.
- we do not switch the replaced cards back in the deck, leading to more aces
- we do it every time the deck gets spilled
