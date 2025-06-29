# tideman
The hardest <b>CS50x</b> problem (it was for me at least), from <b>pset3</b>, year 2024.
<br>
### Problem to solve:
Based on ballots input, determine the winner of an election: every voter gets one vote, and the candidate with the most votes wins.
<br><br>
References:
https://en.wikipedia.org/wiki/Ranked_pairs
<br>
https://cs50.harvard.edu/x/2024/psets/3/tideman/
<br><br>
My approach was to use <b>Depth First Search</b> with <b>stack</b> implementation using <b>dynamic memory allocation</b>. I'm still a learner and on a long way to write robust C code, but <b>valgrind</b> said <b>no memory leaks</b> :D 
<br>
PS: i used cs50.h library to define <b>typedef char *string</b>, <b>int i = get_int("Prompt: ") (get_int is scanf("%i", &i) implementation)</b>, <b>string s = get_string("Prompt: ")</b>, but this was only to make sure to pass the <b>check50</b>.
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
Winner: Alice
<br><br>
Visualiser:
https://tideman.netlify.app/
<br>
PS: <b>data.txt</b> can be passed to the <b>CLI</b> using ./tideman Alice Bob Charlie < data.txt and I provided CLI program <b>tideman</b> - executable file compiled with <b>clang18.1</b> on <b>linux</b>.
