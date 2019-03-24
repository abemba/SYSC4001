## SYSC 4001 Assignment 2 Tests

*Samuel Dewan*

*Chance Abemba*

#####Mandatory Test Results:

In both of these tests our program was able to successfully sort the list in descending order. It also found the correct values for the minimum, maximum and median values.

```
$ ./STATS
Would you like verbose output? (Y/N): n
Please enter integer 1 of 5: 5
Please enter integer 2 of 5: 6
Please enter integer 3 of 5: 8
Please enter integer 4 of 5: 2
Please enter integer 5 of 5: 7

Sorted list: 8, 7, 6, 5, 2
Minimum: 2
Maximum: 8
Median: 6
```

```
$ ./STATS
Would you like verbose output? (Y/N): y
Please enter integer 1 of 5: 10
Please enter integer 2 of 5: 9
Please enter integer 3 of 5: 11
Please enter integer 4 of 5: 5
Please enter integer 5 of 5: 7
Process 0 did not swap elements 0 and 1.
Process 1 swaped elements 1 and 2.
Process 2 did not swap elements 2 and 3.
Process 3 swaped elements 3 and 4.
Process 2 did not swap elements 2 and 3.
Process 0 swaped elements 0 and 1.
Process 2 did not swap elements 2 and 3.
Process 0 did not swap elements 0 and 1.

Sorted list: 11, 10, 9, 7, 5
Minimum: 5
Maximum: 11
Median: 9
```

#####Additonal Test Results:

This test shows the input validation in our program. The progam will repeatedly prompt the user until they enter valid input, it also will not allow the user to enter the same integer twice as the assignment states that the integers must be distinct.

```
$ ./STATS 
Would you like verbose output? (Y/N): q
Please enter Y or N: jfaksld;fafs
Please enter Y or N: 
Please enter Y or N: 789
Please enter Y or N: 
Please enter Y or N: No
Please enter integer 1 of 5: not an int
Please enter a valid integer: 
Please enter a valid integer: 5
Please enter integer 2 of 5: 5
All integers must be unique: 4
Please enter integer 3 of 5: 5
All integers must be unique: 3
Please enter integer 4 of 5: 2
Please enter integer 5 of 5: ^
Please enter a valid integer: 1

Sorted list: 5, 4, 3, 2, 1
Minimum: 1
Maximum: 5
Median: 3
```

These tests show that our algorithm is able to support arrays of arbitrary length (the length is selected at compile time). Note that in the second test the array has an even length and that the median is therefor the average of the two center most values.

```
$ ./STATS 
Would you like verbose output? (Y/N): n
Please enter integer 1 of 7: 890
Please enter integer 2 of 7: 1024
Please enter integer 3 of 7: 672
Please enter integer 4 of 7: 7
Please enter integer 5 of 7: 56000
Please enter integer 6 of 7: 34
Please enter integer 7 of 7: 198

Sorted list: 56000, 1024, 890, 672, 198, 34, 7
Minimum: 7
Maximum: 56000
Median: 672
```

```
$ ./STATS 
Would you like verbose output? (Y/N): n
Please enter integer 1 of 10: 45
Please enter integer 2 of 10: 627
Please enter integer 3 of 10: 891
Please enter integer 4 of 10: 31
Please enter integer 5 of 10: 3
Please enter integer 6 of 10: 9
Please enter integer 7 of 10: 17
Please enter integer 8 of 10: 223
Please enter integer 9 of 10: 195
Please enter integer 10 of 10: 513

Sorted list: 891, 627, 513, 223, 195, 45, 31, 17, 9, 3
Minimum: 3
Maximum: 891
Median: 120
```

These tests show how our algorthim supports abitrary sorting orders. The order is specified by providing the algorithm with a sorting function. The first test sorts the list in asscending order, the second test sorts the list in descending order by first digit. Note that the output code assumes that the list is sorted in ascending or descending order, so not all orders will result in correct determination of the maximum, minimum and median values.

```
$ ./STATS 
Would you like verbose output? (Y/N): n
Please enter integer 1 of 5: 45
Please enter integer 2 of 5: 67
Please enter integer 3 of 5: 23
Please enter integer 4 of 5: 89
Please enter integer 5 of 5: 12

Sorted list: 12, 23, 45, 67, 89
Minimum: 12
Maximum: 89
Median: 45
```

```
$ ./STATS 
Would you like verbose output? (Y/N): n
Please enter integer 1 of 5: 9
Please enter integer 2 of 5: 456
Please enter integer 3 of 5: 23
Please enter integer 4 of 5: 1234
Please enter integer 5 of 5: 67

Sorted list: 9, 67, 456, 23, 1234
Minimum: 9
Maximum: 1234
Median: 456
```

The following are more examples of the program running configured as required for the assignment.

```
$ ./STATS 
Would you like verbose output? (Y/N): y
Please enter integer 1 of 5: 45
Please enter integer 2 of 5: 61
Please enter integer 3 of 5: 0
Please enter integer 4 of 5: 43
Please enter integer 5 of 5: -120
Process 0 swaped elements 0 and 1.
Process 1 did not swap elements 1 and 2.
Process 2 swaped elements 2 and 3.
Process 3 did not swap elements 3 and 4.

Sorted list: 61, 45, 43, 0, -120
Minimum: -120
Maximum: 61
Median: 43
```

```
$ ./STATS 
Would you like verbose output? (Y/N): n
Please enter integer 1 of 5: 8
Please enter integer 2 of 5: 1
Please enter integer 3 of 5: 4
Please enter integer 4 of 5: 2
Please enter integer 5 of 5: 7

Sorted list: 8, 7, 4, 2, 1
Minimum: 1
Maximum: 8
Median: 4
```

```
$ ./STATS 
Would you like verbose output? (Y/N): n
Please enter integer 1 of 5: 1
Please enter integer 2 of 5: 2
Please enter integer 3 of 5: 3
Please enter integer 4 of 5: 4
Please enter integer 5 of 5: 5

Sorted list: 5, 4, 3, 2, 1
Minimum: 1
Maximum: 5
Median: 3
```

```
$ ./STATS 
Would you like verbose output? (Y/N): n
Please enter integer 1 of 5: -45
Please enter integer 2 of 5: -67
Please enter integer 3 of 5: -123
Please enter integer 4 of 5: -5
Please enter integer 5 of 5: -1002

Sorted list: -5, -45, -67, -123, -1002
Minimum: -1002
Maximum: -5
Median: -67
```

This test shows that the program is unable to handle input which is large than the maximum value for an integer.

```
$ ./STATS 
Would you like verbose output? (Y/N): n
Please enter integer 1 of 5: 5000000000
Please enter integer 2 of 5: 0
Please enter integer 3 of 5: 1
Please enter integer 4 of 5: 2
Please enter integer 5 of 5: 3

Sorted list: 705032704, 3, 2, 1, 0
Minimum: 0
Maximum: 705032704
Median: 2
```

