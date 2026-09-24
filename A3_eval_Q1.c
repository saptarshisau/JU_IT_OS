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
    int fd[2];
    int sum;
    int total = 0;

    if (pipe(fd) < 0)
    {
        perror("pipe failed");
        exit(1);
    }

    // Parent Process
    printf("Parent Process\n");
    printf("PID  : %d\n", getpid());
    printf("PPID : %d\n", getppid());

    printf("Range: ");
    for (int i = 1; i <= 10; i++)
        printf("%d ", i);

    sum = findSum(1, 10);
    printf("\nSum  : %d\n\n", sum);

    // Parent writes its sum
    write(fd[1], &sum, sizeof(sum));

    child = fork();

    if (child < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (child == 0)
    {
        // Child does not read
        close(fd[0]);

        printf("Child Process\n");
        printf("PID  : %d\n", getpid());
        printf("PPID : %d\n", getppid());

        printf("Range: ");
        for (int i = 11; i <= 20; i++)
            printf("%d ", i);

        sum = findSum(11, 20);
        printf("\nSum  : %d\n\n", sum);

        // Child writes its sum
        write(fd[1], &sum, sizeof(sum));

        grandchild = fork();

        if (grandchild < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (grandchild == 0)
        {
            // Grandchild also does not read
            // fd[0] is already closed and remains closed

            printf("Grandchild Process\n");
            printf("PID  : %d\n", getpid());
            printf("PPID : %d\n", getppid());

            printf("Range: ");
            for (int i = 21; i <= 30; i++)
                printf("%d ", i);

            sum = findSum(21, 30);
            printf("\nSum  : %d\n\n", sum);

            // Grandchild writes its sum
            write(fd[1], &sum, sizeof(sum));

            greatgrandchild = fork();

            if (greatgrandchild < 0)
            {
                perror("fork failed");
                exit(1);
            }

            if (greatgrandchild == 0)
            {
                // Great-grandchild does not read
                // fd[0] is already closed

                printf("Great-grandchild Process\n");
                printf("PID  : %d\n", getpid());
                printf("PPID : %d\n", getppid());

                printf("Range: ");
                for (int i = 31; i <= 40; i++)
                    printf("%d ", i);

                sum = findSum(31, 40);
                printf("\nSum  : %d\n\n", sum);

                // Great-grandchild writes its sum
                write(fd[1], &sum, sizeof(sum));

                close(fd[1]);
                exit(0);
            }
            else
            {
                // Grandchild no longer needs write end
                close(fd[1]);

                waitpid(greatgrandchild, NULL, 0);
                exit(0);
            }
        }
        else
        {
            // Child no longer needs write end
            close(fd[1]);

            waitpid(grandchild, NULL, 0);
            exit(0);
        }
    }
    else
    {
        // Parent does not write anymore
        close(fd[1]);

        waitpid(child, NULL, 0);

        for (int i = 0; i < 4; i++)
        {
            read(fd[0], &sum, sizeof(sum));
            total += sum;
        }

        close(fd[0]);

        printf("Final Total Sum = %d\n", total);
    }

    return 0;
}