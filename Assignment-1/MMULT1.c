#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h> // pid_t
#include <unistd.h> // fork
#include <stdlib.h> // exit


#define MATRIX_SIZE 4


long (*matrix_q)[MATRIX_SIZE];

/**
 *  Calculate the matrix product for certain rows
 *
 *  @param row The first row index
 *  @param num_rows The number of rows for which the product should be calculated
 *  @param m The first input matrix
 *  @param n The second input matrix
 *  @param out The matrix in which the output should be placed
 */
void product_for_rows (int row, int num_rows, long (*m)[MATRIX_SIZE] ,long (*n)[MATRIX_SIZE], long (*out)[MATRIX_SIZE])
{
    for (int i = row; i < (row + num_rows); i++) {
        for (int k = 0; k < MATRIX_SIZE; k++) {
            out[i][k] = 0;
            for (int j = 0; j < MATRIX_SIZE; j++) {
                out[i][k] += m[i][j] * n[j][k];
            }
        }
    }
}

/**
 *  Print a matrix
 *
 *  @param The matrix to be printed
 */
void print_matrix (long (*matrix)[MATRIX_SIZE])
{
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%3ld  ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main (int argc, char **argv)
{
    /* Setup shared memory */
    int shmid = shmget((key_t)1234, sizeof(long) * MATRIX_SIZE * MATRIX_SIZE, 0666 | IPC_CREAT); 
    if (shmid == -1) {
        fprintf(stderr, "Failed to get shared memory: %s\n", strerror(errno));
        exit(1);
    }

    matrix_q = shmat(shmid, NULL, 0);
    if ((int)matrix_q == -1) {
        fprintf(stderr, "Failed to attach shared memory: %s\n", strerror(errno));
        exit(1);
    }


    /* Setup inputs */
	long matrix_m[MATRIX_SIZE][MATRIX_SIZE] = {
				{1, 2, 3, 4},
				{5, 6, 7, 8},
				{4, 3, 2, 1},
                {8, 7, 6, 5}};

	long matrix_n[MATRIX_SIZE][MATRIX_SIZE] = {
                {1, 3, 5, 7},
                {2, 4, 6, 8},
                {7, 3, 5, 7},
                {8, 6, 4, 2}};

    /* Compute product */
    pid_t children[MATRIX_SIZE];
    
    for (int i = 0; i < MATRIX_SIZE; i++) {
        children[i] = fork();
        
        if (children[i] == -1) {
            fprintf(stderr, "Failed to fork: %s\n", strerror(errno));
            exit(1);
        } else if (children[i] == 0) {
            // Child for row i
            printf("Child Process: working with row %d\n", i);
            product_for_rows(i, 1, matrix_m, matrix_n, matrix_q);
            exit(0);
        }
    }
    
    /* Wait for all children to complete */
    for (int i = 0; i < MATRIX_SIZE; i++) {
        waitpid(children[i], NULL, 0);
    }
    
    /* Print matrices */
    printf("M:\n");
    print_matrix(matrix_m);
    printf("N:\n");
    print_matrix(matrix_n);
    printf("Q:\n");
    print_matrix(matrix_q);
    
    /* Unmap shared memory */
    shmdt(matrix_q);
}
