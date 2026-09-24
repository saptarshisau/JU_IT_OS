thik ee toh cholche pipe() charao otay

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int choice;
    int child_sleep_time = 1;
    int parent_sleep_time = 1;

    printf("Select delay durations (sleep) for Child and Parent processes:\n");
    printf("1: Both Child and Parent sleep(1)\n");
    printf("2: Child sleep(2), Parent sleep(1)\n");
    printf("3: Child sleep(1), Parent sleep(2)\n");
    printf("4: Both Child and Parent sleep(2)\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Exiting.\n");
        return 1;
    }

    switch (choice) {
        case 1:
            child_sleep_time = 1;
            parent_sleep_time = 1;
            break;
        case 2:
            child_sleep_time = 2;
            parent_sleep_time = 1;
            break;
        case 3:
            child_sleep_time = 1;
            parent_sleep_time = 2;
            break;
        case 4:
            child_sleep_time = 2;
            parent_sleep_time = 2;
            break;
        default:
            printf("Invalid choice. Defaulting to Option 1.\n");
            child_sleep_time = 1;
            parent_sleep_time = 1;
            break;
    }

    printf("\nStarting execution...\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // Child Process: Prints odd numbers (1, 3, 5, 7, 9)
        for (int i = 1; i <= 10; i += 2) {
            printf("[Child PID: %d] Odd: %d\n", getpid(), i);
            sleep(child_sleep_time);
        }
        exit(0);
    } 
    else {
        // Parent Process: Prints even numbers (2, 4, 6, 8, 10)
        for (int i = 2; i <= 10; i += 2) {
            printf("[Parent PID: %d] Even: %d\n", getpid(), i);
            sleep(parent_sleep_time);
        }
        
        // Wait for the child process to complete before terminating the program
        wait(NULL);
        printf("\n[Parent] Child process has finished. Execution complete.\n");
    }

    return 0;
}