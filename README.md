# precise-COBEN-calculator
## How to use
Open `input.txt` for input.
- 1st line: number of **athletes** (which is also the number of event scores)
- 2nd line: number of **eliminations**
- 3rd line: a list of **athlete scores**. Separate using a comma, and also end the list with a comma
- 4th line: a list of **event scores**. Separate using a comma, and also end the list with a comma
- For 3rd and 4th lines, sort the lists in descending order (largest to smallest)
- Make sure the number of atheletes matches the list of athlete scores and event scores

Example of a valid input:
```
16
2
100, 70, 50, 40, 30, 25, 20, 15, 12, 10, 8, 6, 4, 3, 2, 1,
100, 70, 50, 40, 30, 25, 20, 15, 12, 10, 8, 6, 4, 3, 2, 1,
```

Once you're done, save the input file and run `print_results.exe`. Usually it runs in seconds for 1 or 2 eliminations, but may even in minutes for more eliminations, depending on your hardware. So yeah, if you input something like 69 atheletes and 42 eliminations, better patiently wait (and I mean it, take a break or something lol).

## About this calculator
This calculator is aim to achieve *precise* COBEN results (as opposed to results from other calculators which mostly use the Monte Carlo method) without dealing the tremendous runtime from checking every single possibilities.

The general approach is to check for every pair of athlete and event scores and count the posibilities in which that pair of scores lead to the elimination of the athelete. The counting process uses dynamic programming techniques to shrink the runtime (as opposed to the use of recursions in my old version).

I was concerned with handling big numbers for this calculator, but it is no longer a problem (I guess?). The only remaining issue is long runtime for big number of eliminations (for reference, with my hardware, one specific testcase I ran for 63 atheletes took about 15 seconds for 2 eliminations and about 40 seconds for 3 eliminations), which is a slight letdown for me, and I wish there is some way to trememdously improve the performance. Otherwise, for the general use (which should deal with about 20-ish atheletes and less than 4 eliminations), the calculator is pretty good.

If anyone spots any mistakes and bugs when using this calculator, please lemme know! I have to admit, I am not experienced with all of this stuff right yet, so if anyone is more experienced and well-versed with C++, with DSA or even GitHub (I still don't know how this thing works) and feel like helping with the project, I would be more than grateful.

If you wanna run the source code, you're gonna need Boost libraries downloaded. Again, I wish I know how to correctly download and add that to my project, but I'm a little too lazy for my own good...

And finally, if you managed to scroll down here and ~~hopefully~~ read all of this... thank you so much! My Discord is yellowdude BTW, if you need anything with the project. Have a nice day! :sparkling_heart: