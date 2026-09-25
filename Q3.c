#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int i, j, sum;
    int start, end;
    int total=0;
    int fd[2];

    if (pipe(fd) < 0)
    {
        perror("pipe failed");
        exit(1);
    }

    for (i = 0; i < 5; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0)
        {
            // Child process

            start = i * 10 + 1;
            end = (i + 1) * 10;

            sum = 0;

            printf("\nCHILD %d\n", i + 1);
            printf("PID  : %d\n", getpid());
            printf("PPID : %d\n", getppid());

            printf("Numbers: ");

            for (j = start; j <= end; j++)
            {
                printf("%d ", j);
                sum += j;
            }

            printf("\nSum = %d\n", sum);

            // Child has its own copy of 'total', so send the sum to the parent
            close(fd[0]);
            write(fd[1], &sum, sizeof(sum));
            close(fd[1]);

            exit(0);
        }else{
            wait(NULL);
            read(fd[0], &sum, sizeof(sum));
            total += sum;
        }
    }

    close(fd[0]);
    close(fd[1]);

    printf("\nParent Process\n");
    printf("PID : %d\n", getpid());
    printf("Total Sum = %d\n", total);

    return 0;
}