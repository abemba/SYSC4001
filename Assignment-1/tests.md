## SYSC 4001 Assignment 1

*Samuel Dewan*

*Chance Abemba*



#### Part 1

Mandatory Test Results:

```
$ part-1/MMULT1
Child Process: working with row 1
Child Process: working with row 0
Child Process: working with row 2
Child Process: working with row 3
M:
  1    2    3    4  
  5    6    7    8  
  4    3    2    1  
  8    7    6    5  
N:
  1    3    5    7  
  2    4    6    8  
  7    3    5    7  
  8    6    4    2  
Q:
 58   44   48   52  
130  108  128  148  
 32   36   52   68  
104  100  132  164  
```

Additonal Test Results:

```
$ part-1/MMULT1
Child Process: working with row 0
Child Process: working with row 1
Child Process: working with row 2
Child Process: working with row 3
M:
 56   34   87    3  
  0   56   73   28  
 41   13   26   31  
 82   70   66    0  
N:
 18   31   15   17  
 20   32    9    3  
  7   56   42   90  
100    0    7   10  
Q:
2597  7696  4821  8914  
4431  5880  3766  7018  
4280  3143  2041  3386  
3338  8478  4632  7544

```

These test results have been verified against an online calculator to ensure that our program is acurate.

​	It is interesting to note that in the manditory data test, the child processes did not finish in order, this is not an issue as the program waits for all processes to have completed before printing the results.

#### Part 2

Mandatory Test Results:

```
$ part-2/MMULT2
Child Process: working with row 0
Child Process: working with row 1
Child Process: working with row 2
Child Process: working with row 3
M:
  1    2    3    4  
  5    6    7    8  
  4    3    2    1  
  8    7    6    5  
N:
  1    3    5    7  
  2    4    6    8  
  7    3    5    7  
  8    6    4    2  
Q:
 58   44   48   52  
130  108  128  148  
 32   36   52   68  
104  100  132  164  
Time elapsed: 3130 µs

$ part-2/MMULT2 2
Child Process: working with rows 0 and 1
Child Process: working with rows 2 and 3
M:
  1    2    3    4  
  5    6    7    8  
  4    3    2    1  
  8    7    6    5  
N:
  1    3    5    7  
  2    4    6    8  
  7    3    5    7  
  8    6    4    2  
Q:
 58   44   48   52  
130  108  128  148  
 32   36   52   68  
104  100  132  164  
Time elapsed: 3011 µs

$ part-2/MMULT2 1
Child Process: working with rows 0, 1, 2 and 3
M:
  1    2    3    4  
  5    6    7    8  
  4    3    2    1  
  8    7    6    5  
N:
  1    3    5    7  
  2    4    6    8  
  7    3    5    7  
  8    6    4    2  
Q:
 58   44   48   52  
130  108  128  148  
 32   36   52   68  
104  100  132  164  
Time elapsed: 1449 µs

$ part-2/MMULT2 0
Number of processes must be between 1 and 4.

$ part-2/MMULT2 3
Number of processes must be a factor of 4.
```

Additonal Test Results:

```
$ part-2/MMULT2 4
Child Process: working with row 0
Child Process: working with row 2
Child Process: working with row 1
Child Process: working with row 3
M:
   6     8     3    10  
  30     1     5     0  
  56     2     8    11  
   3    89     0     5  
N:
   6    90    54     3  
   1     0    34     6  
  63    78     4    22  
  32    62     8    12  
Q:
 553  1394   688   252  
 496  3090  1674   206  
1194  6346  3212   488  
 267   580  3228   603  
Time elapsed: 2842 µs

$ part-2/MMULT2 2
Child Process: working with rows 0 and 1
Child Process: working with rows 2 and 3
M:
   6     8     3    10  
  30     1     5     0  
  56     2     8    11  
   3    89     0     5  
N:
   6    90    54     3  
   1     0    34     6  
  63    78     4    22  
  32    62     8    12  
Q:
 553  1394   688   252  
 496  3090  1674   206  
1194  6346  3212   488  
 267   580  3228   603  
Time elapsed: 1476 µs

$ part-2/MMULT2 1
Child Process: working with rows 0, 1, 2 and 3
M:
   6     8     3    10  
  30     1     5     0  
  56     2     8    11  
   3    89     0     5  
N:
   6    90    54     3  
   1     0    34     6  
  63    78     4    22  
  32    62     8    12  
Q:
 553  1394   688   252  
 496  3090  1674   206  
1194  6346  3212   488  
 267   580  3228   603  
Time elapsed: 995 µs
```

Our program for part two produces the correct results for all of the possible numbers of process.

​	The program is faster with fewer processes (the results for a single processes being almost 3 times faster than the results for four processes). This implies that the overhead of forking processes is a signifigant factor in the runtime of our program and that, for this size of matrix, multi-processing is not an efficent way of approaching up the problem. Likely we would see better results using threading, as there is less latency in spawning threads, however for this limited application a non-concurent solution is probably best.

​	If we where to scale our solution to larger matrices it would at some point become faster to use multiple threads. If we had to multiply many matrices, even smaller ones, it might be efficent to use a pool of long lived worker threads so that the calculations could be performed in parallel without the overhead of spawning new threads for each calculation.



_Note: The test cases where run on a computer with two true cores and four SMT cores, had they been run on a system with four or more true cores the test cases with four processes may have run slightly faster._