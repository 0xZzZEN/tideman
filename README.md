# tideman
The hardest <b>CS50x</b> problem (for me it was at least), from <b>pset3</b>.
<br>
### Problem to solve:
Based on ballots input, determine the winner of an election: every voter gets one vote, and the candidate with the most votes wins.
<br><br>
References:
https://en.wikipedia.org/wiki/Ranked_pairs
<br>
https://cs50.harvard.edu/x/2024/psets/3/tideman/
<br><br>
My approach was to use <b>Depth First Search</b>. I'm still a learner and on a long way to write robust C code, so don't consider it to be correct.
### Example:
./tideman Alice Bob Charlie
<br><br>
Number of voters: 5
<br><br>
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob
<br><br>
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob
<br><br>
Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice
<br><br>
Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice
<br><br>
Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob
<br><br>
Visualiser:
https://tideman.netlify.app/
