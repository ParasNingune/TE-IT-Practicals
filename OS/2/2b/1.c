#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<stdlib.h>

void bubble_asc(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printf("\nArray in ascending order: ");
    for (i = 0; i < n; i++)
    {
        printf("\t%d", arr[i]);
    }
}

int main(int argc, char *argv[]) // e.g., ./programName executable 5 2 7 9 4
{
    if (argc < 3)
    {
        printf("Usage: %s <executable> <list of numbers>\n", argv[0]);
        return 1;
    }

    int i, j = 2, l;
    int n = argc - 2;
    int a[n];
    char *args[n + 2];  // Increase the size of args by 1 to accommodate NULL

    args[0] = argv[1];  // executable name

    // Copying and converting command-line arguments to the array
    for (i = 1; i <= n; i++, j++)
    {
        a[i - 1] = atoi(argv[j]);
    }

    // Sorting the array in ascending order
    bubble_asc(a, n);

    // Allocating memory for sorted arguments
    for (i = 1; i <= n; i++)
    {
        args[i] = malloc(10 * sizeof(char));  // Allocate memory for each string
        sprintf(args[i], "%d", a[i - 1]);     // Convert the sorted integers to strings
    }
    args[n + 1] = NULL;  // Null terminate the arguments array for execve

    pid_t pid = fork();
    if (pid == 0)
    {
        printf("\nI am the Child Process, my pid is %d \n", getpid());
        printf("My Parent Process pid is %d \n", getppid());
        execve(argv[1], args, NULL);  // Execute the given program
        perror("execve failed");  // If execve returns, it must have failed
        exit(1);
    }
    else if (pid > 0)
    {
        wait(NULL);  // Wait for child process to finish
        printf("\nI am the Parent Process, my pid is %d \n", getpid());
        printf("My Parent's Parent Process pid is %d\n\n", getppid());
    }
    else
    {
        perror("Fork failed");
    }

    // Free allocated memory
    for (i = 1; i <= n; i++)
    {
        free(args[i]);
    }

    return 0;
}
