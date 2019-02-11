#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MATRIX_SIZE 4


long *output;


int main (int argc, char **argv)
{
    /* Setup shared memory */
    int shmid = shmget((key_t)1234, sizeof(long) * MATRIX_SIZE * MATRIX_SIZE, 0666 | IPC_CREAT); 
    if (shmid == -1) {
        fprintf(stderr, "Failed to get shared memory: %s\n", strerror(errno));
        exit(1);
    }

    output = shmat(shmid, NULL, 0);
    if (output == (int*) -1) {
        fprintf(stderr, "Failed to attach shared memory: %s\n", strerror(errno));
        exit(1);
    }

    /* Setup inputs */
    long input_m[] = [1, 2, 3, 4, 5, 6, 7, 8, 4, 3, 2, 1, 8, 7, 6, 5];
    long input_n[] = [1, 3, 5, 7, 2, 4, 6, 8, 7, 3, 5, 7, 8, 6, 4, 2];

    /* Compute product */
    pid_t children[MATRIX_SIZE];

    for (int i = 0; i < MATRIX_SIZE; i++) {
        children[i] = fork();

        if (children[i] == -1) {
            fprintf(stderr, "Failed to fork: %s\n", strerror(errno));
            exit(1);
        } else if (child_pids[i] == 0) {
            // Child
            for (int k = 0; k < MATRIX_SIZE; k++) {
                long long sum = 0;
                for (int j = 0; j < MATRIX_SIZE; j++) {
                    sum += matrices[]
                }
            }
        }
    } 
}
