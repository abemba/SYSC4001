#define _POSIX_SOURCE

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/sem.h>
#include "semun.h"

#define LIST_LENGTH 5

int *list;

/**
 *  Decrement a semaphore, if decrementing the semaphore would make the value
 *  negative then this function will block until the semaphore can be
 *  decremented without going negative.
 */
#define SEMAPHORE_P     -1
/**
 *  Increment a sempahore.
 */
#define SEMAPHORE_V     1
/**
 *  Wait for a semaphore to become 0.
 */
#define SEMAPHORE_WAIT  0

/**
 *  Perform an operation on a single semaphore.
 *
 *  @param sem_id The id of the semaphore set on which the operation should be
 *                performed
 *  @param sem_num The index of the semaphore on which the operation should be
 *                 performed
 *  @param op The operation to perform
 *
 *  @return 0 if the operation completed successfully
 */
static int semaphore_operation(int sem_id, int sum_num, int op)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = sum_num;
    sem_b.sem_op = op;
    sem_b.sem_flg = SEM_UNDO;
    
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore operation %d failed: %s\n", op,
                strerror(errno));
        return 1;
    }
    
    return 0;
}

/**
 *  Check if an array is sorted.
 *
 *  @param a The array to check
 *  @param length The length of the array
 *  @param comparison The comparison to use to determine if two elements are in order
 *
 *  @return 1 if the array is sorted, 0 otherwise
 */
static int array_is_sorted (int *a, int length, int (*comparison)(int, int))
{
    for (int i = 0; i < (length - 1); i++) {
        if (comparison(a[i], a[i + 1]) > 0) {
            return 0;
        }
    }
    return 1;
}

/**
 *  Repeatedly check if an two elements of an array are in order and swap them
 *  if they are not
 *
 *  @param a The array to be sorted
 *  @param length The length of the array to be sorted
 *  @param lock_sem_id Semaphore set to lock individual elements of the array
 *  @param control_sem_id Semaphore set to control sorting rounds
 *  @param comparison Function used to determin if array elements are in order
 *  @param verbose Flag which indicates whether verbose output should be generated
 */
static inline void swap_proc (int *a, int i, int lock_sem_id,
                              int control_sem_id, int (*comparison)(int, int),
                              int verbose)
{
    for (;;) {
//        printf("Process \%d acquiring lock %d\n", i, i);
        
        // Acquire locks, lower address first
        semaphore_operation(lock_sem_id, i, SEMAPHORE_P);
//        printf("Process \%d has lock %d, acquiring lock %d\n", i, i, i + 1);
        semaphore_operation(lock_sem_id, i + 1, SEMAPHORE_P);
        
//        printf("Process \%d has lock %d\n", i, i + 1);
        
        // Swap values if they are out of order
        if (comparison(a[i], a[i + 1]) > 0) {
            int temp = a[i];
            a[i] = a[i + 1];
            a[i + 1] = temp;
            if (verbose) {
                printf("Process \%d swaped elements %d and %d.\n", i, i, i + 1);
            }
        } else if (verbose) {
            printf("Process \%d did not swap elements %d and %d.\n", i, i, i + 1);
        }
        
//        printf("Process \%d releasing locks %d and %d\n", i, i , i + 1);
        // Release locks
        semaphore_operation(lock_sem_id, i, SEMAPHORE_V);
        semaphore_operation(lock_sem_id, i + 1, SEMAPHORE_V);
        
        // Signal that this round is complete
        semaphore_operation(control_sem_id, 0, SEMAPHORE_P);
        
//        printf("Process \%d waiting for next round.\n", i);
        // Wait for next round
        semaphore_operation(control_sem_id, 1, SEMAPHORE_P);
    }
}

/**
 *  Sort an array of ints with multiple processes as described in the assignment.
 *
 *  @param a The array to be sorted
 *  @param length The length of the array to be sorted
 *  @param verbose Flag which indicates whether verbose output should be generated
 *
 *  @return 0 if successfull
 */
static int fork_sort (int *a, int length, int (*comparison)(int, int), int verbose)
{
    /* Create semaphores */
    union semun sem_union;
    // Semaphore set used to lock array elements
    int lock_sem_id = semget((key_t)IPC_PRIVATE, LIST_LENGTH, 0666 | IPC_CREAT);
    // Initilize locks to 1 (unclaimed)
    sem_union.val = 1;
    for (int i = 0; i < LIST_LENGTH; i++) {
        if (semctl(lock_sem_id, i, SETVAL, sem_union) == -1) {
            fprintf(stderr, "Failed to set lock semaphore %d: %s\n", i, strerror(errno));
            return 1;
        }
    }
    
    // Semaphore set used as events to control sorting process
    int control_sem_id = semget((key_t)IPC_PRIVATE, 2, 0666 | IPC_CREAT);
    // Initilize round done semaphore to number of processes
    sem_union.val = LIST_LENGTH - 1;
    if (semctl(control_sem_id, 0, SETVAL, sem_union) == -1) {
        fprintf(stderr, "Failed to set round done semaphore: %s\n", strerror(errno));
        return 1;
    }
    // Initilize next round semaphore to 0
    sem_union.val = 0;
    if (semctl(control_sem_id, 1, SETVAL, sem_union) == -1) {
        fprintf(stderr, "Failed to set next round semaphore: %s\n", strerror(errno));
        return 1;
    }
    
    /* Fork processes */
    pid_t children[LIST_LENGTH - 1] = {0};
    for (int i = 0; i < (LIST_LENGTH - 1); i++) {
        children[i] = fork();
        
        if (children[i] == -1) {
            fprintf(stderr, "Failed to fork: %s\n", strerror(errno));
            return 1;
        } else if (children[i] == 0) {
            // Child for elements i and i + 1
            swap_proc(a, i, lock_sem_id, control_sem_id, comparison, verbose);
            exit(0);
        }
    }
    
    /* Manage sorting process */
    for (;;) {
        // Wait for round to complete
        semaphore_operation(control_sem_id, 0, SEMAPHORE_WAIT);
        
        if (array_is_sorted(a, LIST_LENGTH, comparison)) {
            /* All done! */
            // Kill children
            for (int i = 0; i < (LIST_LENGTH - 1); i++) {
                kill(children[i], SIGTERM);
            }
            break;
        }
        
        /* Start another round */
        // Reset round complete semaphore
        sem_union.val = LIST_LENGTH - 1;
        if (semctl(control_sem_id, 0, SETVAL, sem_union) == -1) {
            fprintf(stderr, "Failed to set round done semaphore: %s\n", strerror(errno));
            return 1;
        }
        // Release children for another round
        sem_union.val = LIST_LENGTH - 1;
        if (semctl(control_sem_id, 1, SETVAL, sem_union) == -1) {
            fprintf(stderr, "Failed to set next round semaphore: %s\n", strerror(errno));
            return 1;
        }
    }
    
    /* Remove semaphore sets */
    if (semctl(lock_sem_id, 0, IPC_RMID, sem_union) == -1) {
        fprintf(stderr, "Failed to delete lock semaphore set: %s\n", strerror(errno));
        return 1;
    }
    if (semctl(control_sem_id, 0, IPC_RMID, sem_union) == -1) {
        fprintf(stderr, "Failed to delete control semaphore set: %s\n", strerror(errno));
        return 1;
    }
    
    return 0;
}

/**
 *  Get a single line from stdin.
 *
 *  @param buffer The buffer in which the line should be stored
 *  @param buf_len The size of the buffer
 *
 *  @return 0 if successfull, a possitive integer otherwise
 */
static int get_line (char *buffer, int buf_len)
{
    char *retval;
    retval = fgets(buffer, buf_len, stdin);
    
    if (!retval) {
        // fgets failed
        return 1;
    }
    
    if ((retval = strchr(buffer, '\n')) == 0) {
        // There is no new line in the input, the buffer was filled
        return 1;
    }
    
    // Discard newline
    *retval = '\0';
    
    return 0;
}

/**
 *  Get a yes or no response from the user.
 *
 *  @param prompt The string with which to prompt the user
 *
 *  @return 1 if the user entered yes, 0 if the user entered no
 */
static int get_yes_no (char *prompt)
{
    printf("%s (Y/N): ", prompt);
    
    char result = '\0';
    char buf[1024];
    
    for (;;) {
        if (!get_line(buf, 1024) && (sscanf(buf, " %c", &result) == 1)) {
            if (result == 'y' || result == 'n' || result == 'Y' || result == 'N') {
                break;
            }
        }
        
        printf("Please enter Y or N: ");
    }
    
    return result == 'y' || result == 'Y';
}

/**
 *  Print an array of integers.
 *
 *  @param a The array to be printed
 *  @param length The length of the array to be printed
 */
static void print_array (int *a, int length)
{
    printf("%d", a[0]);
    for (int i = 1; i < length; i++) {
        printf(", %d", a[i]);
    }
    printf("\n");
}

/**
 *  Compares two integers.
 *
 *  @param a The first int to be compared
 *  @param b The second int to be compared
 *
 *  @return a negative integer, 0 or a positive integer if a is greater than,
 *          equal to, or less than b respectively
 */
static int inverse_compare(int a, int b)
{
    return b - a;
}

//static int compare(int a, int b)
//{
//    return a - b;
//}
//
//static int first_digit_compare(int a, int b)
//{
//    while (a >= 10) {
//        a = a / 10;
//    }
//    while (b >= 10) {
//        b = b / 10;
//    }
//    return b - a;
//}

int main (int argc, char **argv)
{
    /* Setup shared memory */
    int shmid = shmget((key_t)1234, sizeof(int) * LIST_LENGTH, 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "Failed to get shared memory: %s\n", strerror(errno));
        exit(1);
    }
    
    list = shmat(shmid, NULL, 0);
    if ((void*)list == (void*)-1) {
        fprintf(stderr, "Failed to attach shared memory: %s\n", strerror(errno));
        exit(1);
    }
    
    /* Get input */
    int verbose = get_yes_no("Would you like verbose output?");
    
    for (int i = 0; i < LIST_LENGTH; i++) {
        char buf[1024];
        
        printf("Please enter integer %d of %d: ", i + 1, LIST_LENGTH);
        
        for (;;) {
            if (!get_line(buf, 1024) && (sscanf(buf, " %d", list + i) == 1)) {
                // Check that integer is unique
                int unique = 1;
                for (int j = 0; j < i; j++) {
                    if (list[j] == list[i]) {
                        unique = 0;
                        break;
                    }
                }
                if (!unique) {
                    printf("All integers must be unique: ");
                } else {
                    break;
                }
            } else {
                printf("Please enter a valid integer: ");
            }
        }
    }
    
    /* Sort list */
    if (fork_sort(list, LIST_LENGTH, inverse_compare, verbose)) {
        fprintf(stderr, "Error sorting list.\n");
        exit(1);
    }
    
    /* Print list */
    printf("\nSorted list: ");
    print_array(list, LIST_LENGTH);
    if (list[LIST_LENGTH - 1] < list[0]) {
        printf("Minimum: %d\n", list[LIST_LENGTH - 1]);
        printf("Maximum: %d\n", list[0]);
    } else {
        printf("Minimum: %d\n", list[0]);
        printf("Maximum: %d\n", list[LIST_LENGTH - 1]);
    }
    int median = 0;
    if ((LIST_LENGTH % 2) == 0) {
        median = (list[(LIST_LENGTH / 2) - 1] + list[LIST_LENGTH / 2]) / 2;
    } else {
        median = list[(LIST_LENGTH / 2)];
    }
    printf("Median: %d\n", median);
    
    /* Unmap shared memory */
    shmdt(list);
}
