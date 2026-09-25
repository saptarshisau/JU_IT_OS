#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int findSum(int start, int end)
{
    int sum = 0;

    for (int i = start; i <= end; i++)
        sum += i;

    return sum;
}

int main()
{
    pid_t child, grandchild, greatgrandchild;
    int sum;

    // Parent Process
    printf("Parent Process\n");
    printf("PID  : %d\n", getpid());
    printf("PPID : %d\n", getppid());
    printf("Range: ");

    for (int i = 1; i <= 10; i++)
        printf("%d ", i);

    sum = findSum(1, 10);
    printf("\nSum  : %d\n\n", sum);

    child = fork();

    if (child < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (child == 0)
    {
        // Child Process
        printf("Child Process\n");
        printf("PID  : %d\n", getpid());
        printf("PPID : %d\n", getppid());
        printf("Range: ");

        for (int i = 11; i <= 20; i++)
            printf("%d ", i);

        sum = findSum(11, 20);
        printf("\nSum  : %d\n\n", sum);

        grandchild = fork();

        if (grandchild < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (grandchild == 0)
        {
            // Grandchild Process
            printf("Grandchild Process\n");
            printf("PID  : %d\n", getpid());
            printf("PPID : %d\n", getppid());
            printf("Range: ");

            for (int i = 21; i <= 30; i++)
                printf("%d ", i);

            sum = findSum(21, 30);
            printf("\nSum  : %d\n\n", sum);

            greatgrandchild = fork();

            if (greatgrandchild < 0)
            {
                perror("fork failed");
                exit(1);
            }

            if (greatgrandchild == 0)
            {
                // Great-grandchild Process
                printf("Great-grandchild Process\n");
                printf("PID  : %d\n", getpid());
                printf("PPID : %d\n", getppid());
                printf("Range: ");

                for (int i = 31; i <= 40; i++)
                    printf("%d ", i);

                sum = findSum(31, 40);
                printf("\nSum  : %d\n\n", sum);

                exit(0);
            }
            else
            {
                waitpid(greatgrandchild, NULL, 0);
                exit(0);
            }
        }
        else
        {
            waitpid(grandchild, NULL, 0);
            exit(0);
        }
    }
    else
    {
        waitpid(child, NULL, 0);

        printf("Final Total Sum = %d\n",
               findSum(1, 10) +
               findSum(11, 20) +
               findSum(21, 30) +
               findSum(31, 40));
    }

    return 0;
}