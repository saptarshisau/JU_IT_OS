#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int i, j, sum = 0, total = 0;
    int start, end;

    for (i = 0; i < 4; i++) {
        start = i * 10 + 1;
        end = (i + 1) * 10;
        sum = 0;

        for (j = start; j <= end; j++) {
            sum += j;
        }

        printf("PID: %d | PPID: %d | Range: %d-%d | Sum: %d\n", getpid(), getppid(), start, end, sum);

        if (i == 3) {
            exit(0); // Great-grandchild stops here
        }

        pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid > 0) {
            // Wait for child generation to finish
            wait(NULL);

            if (i == 0) {
                // Parent prints total sum (1 to 40)
                for (j = 1; j <= 40; j++) total += j;
                printf("Total Sum (1-40): %d\n", total);
            }
            exit(0);
        }
        // Child (pid == 0) continues to the next loop iteration
    }

    return 0;
}