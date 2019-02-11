#include <stdio.h>
#include <errno.h>
#include <string.h>
//type pid_t is found in this file
#include <sys/types.h>
//import the declaration of the fork function
#include <unistd.h>
//import the exit function
#include <stdlib.h>
#define MATRIX_SIZE 4


long *output;


int main (int argc, char **argv)
{
	//Separation of concern
	//solve the problem in a non shared memory space
	//then solve the problem in a shared memory space


    /* Setup shared memory 
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
	*/


    /* Setup inputs */
          //long input_m[] = {1, 2, 3, 4, 5, 6, 7, 8, 4, 3, 2, 1, 8, 7, 6, 5};
    
	long matrix_a[3][3] ={
				{1,2,3},
				{1,2,1},
				{2,1,2}};

	long matrix_b[3][3] ={
				{2,1,3},
				{2,2,5},
				{2,3,4}};
	long product[3][3];

	printf("%d \n",matrix_a[0][2]);

    //long input_n[] = {1, 3, 5, 7, 2, 4, 6, 8, 7, 3, 5, 7, 8, 6, 4, 2};

    /* Compute product */
    pid_t children[MATRIX_SIZE];



    for (int i = 0; i < 3; i++) {
        children[i] = fork();

        if (children[i] == -1) {
            fprintf(stderr, "Failed to fork: %s\n", strerror(errno));
            exit(1);
        } else if (children[i] == 0) 
	{
            // Child doing matrix_a*matrix_b 
		
	    long long sum;
            
	    for (int k = 0; k < 3; k++) 
	    {
		sum=0;
                for (int j = 0; j < 3; j++) 
		{
                    sum = matrix_a[i][k]*matrix_b[j][i] + sum;
		 
                }
		product[i][k]=sum;
            }


	  //  printf("%d  %d  %d \n", product[i][0],product[i][1],product[i][2]);
	    
		exit(0);
        }
    } 
}
