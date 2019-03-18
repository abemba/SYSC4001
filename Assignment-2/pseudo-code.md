## SYSC 4001 Assignment 2

*Samuel Dewan*

*Chance Abemba*

#### Algorithm

Our program splits the sorting into rounds, in each round every sorting process compares its array elements and swaps them if they are out of order. After each round the main process checks if the array is sorted and starts a new round if not.  

It works as follows (where n is the length of the array to be sorted):

```
- main process spawns (n - 1) processes
- each process aquires the lock for it's lower array element
- each process aquires the lock for it's higher array element
- once it has aquired both locks, each process checks if its elements need to be swaped and swaps them if they do
- after swaping (or not), each process waits for the round to be complete
- when all processes are waiting for the round to be complete, the main process will check if the array is sorted
- if the array is sorted then the main process will kill all of the sorting processes and finish
- if the array is not yet sorted then the main process will release all of the other processes for another round
```

The program makes use of two semaphore sets. The first set contains a semaphore for each item in the list, these are used as binary semaphores to serialize access to the list elements. When locking list elements to perform a comparison the sorting processes always lock the semaphore with a lower index first.

The second set contains two semaphores which control the rounds. The first is the round done semaphore, it is initilized to the number of processes, as each process finishes it decrements the semaphore. After starting a round, the main process waits for the semaphore to reach 0, which will happen when all of the sorting processes have completed their round. The second semaphore in the set is used to start a new round. After decrementing the first semaphore, the sorting processes attempt to decrement the second, it is initilized to 0 so they block. When the main process is ready to start a new round it sets the semaphore to the number of processes so that they can all decrement the semaphore and continue.


