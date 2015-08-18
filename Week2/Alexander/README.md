The proper code is
slot←{{⎕DL ⊃2,⎕←⍵}¨3↑⌽({⍵⊃⍨?⍴⍵}(⍳⍴⍵),(,⍵ ⍵⍴⍳⍵)~first),({⍵⊃⍨?⍴⍵}(⍳⍵),100⍴first),first←(?⍵)/⍨1+∧/'1'=⍕100⊥3↑3↓⎕TS}
 
However, the online interpreter found at http://tryapl.org/ does not allow the {⎕DL ⊃2,⎕←⍵} construct so use the below code instead
slot←{3↑⌽({⍵⊃⍨?⍴⍵}(⍳⍴⍵),(,⍵ ⍵⍴⍳⍵)~first),({⍵⊃⍨?⍴⍵}(⍳⍵),100⍴first),first←(?⍵)/⍨1+∧/'1'=⍕100⊥3↑3↓⎕TS}
 
The so called “cheat” is pretty obvious (Ed: Says Alexander, and I guess, the ten people who can read APL), if the time in hours, minutes and second is just 1’s with leading zeroes, like 11:11:11 or 00:11:11 two identical numbers are generated for the first number. The “second” number being generated has a higher probability of being the same as the first two, of course it’ll be the third number generated. The third number generated is just discarded.
 
To run the solution, first fix the solution by copy-pasting the code into input field of http://tryapl.org/, this will create a function called slot. It can then be invoked by typing slot n, where n is the number of different integers that it can generate, eg slot 20 for a 20-sided slot machine.
 
Hitting 11:11:11 exactly is a bit hard, so here’s a cheatfunction
cheatslot←{3↑⌽({⍵⊃⍨?⍴⍵}(⍳⍴⍵),(,⍵ ⍵⍴⍳⍵)~first),({⍵⊃⍨?⍴⍵}(⍳⍵),100⍴first),first←(?⍵)/⍨1+∧/'1'=’111111’}