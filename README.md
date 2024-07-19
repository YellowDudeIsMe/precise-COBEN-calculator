# precise-COBEN-calculator
## How to use
Open `input.txt` for input.
- 1st line: number of **athletes** (which is also the number of event scores)
- 2nd line: number of **eliminations**
- 3rd line: a list of **athlete scores**. Separate using a comma, and also end the list with a comma
- 4th line: a list of **event scores**. Separate using a comma, and also end the list with a comma
- For 3rd and 4th lines, sort the lists in descending order (largest to smallest)
- Make sure the number of athletes matches the list of athlete scores and event scores

Example of a valid input:
```
16
2
100, 70, 50, 40, 30, 25, 20, 15, 12, 10, 8, 6, 4, 3, 2, 1,
100, 70, 50, 40, 30, 25, 20, 15, 12, 10, 8, 6, 4, 3, 2, 1,
```

Once you finish, save the input file and run `print_results.exe`. A window will pop up and display the results. Usually, it runs in seconds for a few eliminations, but even in minutes for some 20-ish eliminations (which I hope you don't do), depending on your hardware. So, if you input something like 69 athletes and 42 eliminations, better wait (and I mean it, take a break from your algathlon project or something lol).

The window may display results in this format:
```
Athlete #1 : 0.000000000000000 % (IMMUNE!)
Athlete #2 : 0.000000000000000 %
Athlete #3 : 0.000000123456789 %
...

Press any key to exit
```

If you see the tag `(IMMUNE!)` next to an athlete, it means the athlete is *truly immune* and *cannot* be eliminated. Otherwise, it means the COBEN of that athlete is *too small* that it appears to be zero. So, from the example above, we would conclude that **Player #1** is *truly immune*, while **Player #2** has a *very very small* COBEN.

As instructed, press any key to exit the window.

## About this calculator
This calculator aims to achieve *precise* COBEN results (as opposed to results from most other calculators using the Monte Carlo method) without having the tremendous runtime from checking every single possibility. It also comes with a true immunity check for athletes.

The general approach is to check for every pair of athlete and event scores and count the possibilities in which that pair of scores leads to the elimination of the athlete. The counting process uses dynamic programming techniques to shrink the runtime (as opposed to using recursions in my old version).

I am still pondering on the long runtime for big numbers of eliminations and athletes (I am talking about something like 63 athletes and 32 eliminations. Running a test case for this took me a quarter of an hour). However, for the general use (which should deal with less than 4-5 eliminations), the calculator is just great.

If anyone spots any mistakes or bugs when using this calculator, please lemme know! I have to admit, I am not experienced with all of this stuff right now, so if anyone is more well-versed with any parts of the project and is willing to help, I would be more than grateful.

If you wanna run the source code (`source_code.cpp`), you're gonna need [Boost libraries](https://www.boost.org/) downloaded.

And finally, if you managed to scroll down here and ~~hopefully~~ read all of this... thank you so much! If you need anything, you can contact me via my Discord (yellowdude). Have a nice day! :sparkling_heart:

~YellowDude