#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define LIST_LENGTH 5

int *list;

/**
 *  Get a single line from stdin.
 *
 *  @param buffer The buffer in which the line should be stored
 *  @param buf_len The size of the buffer
 *
 *  @return 0 if successfull, a possitive integer otherwise
 */
int get_line (char *buffer, int buf_len)
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
int get_yes_no (char *prompt)
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
void print_array (int *a, int length)
{
    printf("%d", a[0]);
    for (int i = 1; i < length; i++) {
        printf(", %d", a[i]);
    }
    printf("\n");
}

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
    printf("verbose: %d\n", verbose);
    
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
    
    
    
    /* Print list */
    printf("\nSorted list: ");
    print_array(list, LIST_LENGTH);
    printf("Minimum: %d\n", list[0]);
    printf("Maximum: %d\n", list[LIST_LENGTH - 1]);
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
