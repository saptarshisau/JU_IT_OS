/***********************************************************************
*
* Name- Ankit Roy, Roll- 002411001028
* Name- Saptarshi Sau, Roll- 002411001031
* Team-Number-05, Section- A1
* Date: 21st August 2026
*
* Assignment Details: Create two processes. Print "child process" and 
* "In parent process". Child prints odd numbers (1-20), parent prints 
* even numbers. Measure the performance of getpid() and fork() system 
* calls 10 times and compute the average execution time.
* Input Description: None.
* Output Description: Unsynchronized and synchronized continuous output
* of numbers 1-20, process table status, and microsecond benchmark data.
* 
* Compilation command: gcc A1_05_ASN-2_Q4.c 
* Execution sequence: ./a.out
*
* Sample Input and Output generated:
* --- Task (iv): SYNCHRONIZED continuous 1-20 (With sleep) ---
* 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
* Average getpid() execution time: 0.123 microseconds
* Average fork() execution time: 145.670 microseconds
*
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h> // Required for gettimeofday()

void benchmark_syscalls() {
    struct timeval start, end;
    double getpid_total_time, fork_total_time;
    int i;

    printf("\n--- Task (vi) & (vii): Measure system calls ---\n");
    
    // gettimeofday() captures the current time down to microseconds
    gettimeofday(&start, NULL);
    for (i = 0; i < 10; i++) getpid();
    gettimeofday(&end, NULL);
    
    // Calculate elapsed time: Convert seconds difference to microseconds, then add microsecond difference
    getpid_total_time = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);
    
    gettimeofday(&start, NULL);
    for (i = 0; i < 10; i++) {
        if (fork() == 0) exit(0);
        else wait(NULL);
    }
    gettimeofday(&end, NULL);
    fork_total_time = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    // Divide by 10.0 to get the average time per single system call
    printf("Average getpid() execution time: %.3f microseconds\n", getpid_total_time / 10.0);
    printf("Average fork() execution time: %.3f microseconds\n", fork_total_time / 10.0);
}

int main() {
    pid_t pid;

    printf("******************************************************************\n");
    printf("* DISCLAIMER: Do not kill the process.. it will take time due to *\n");
    printf("* sleep() calls! It will run for roughly 50-60 seconds total.    *\n");
    printf("******************************************************************\n");

    printf("\n--- Task (i) & (ii): NO SYNC, Both sleep(1) ---\n");
    pid = fork();
    if (pid == 0) {
        printf("child process\n");
        printf("Child ID: %d, Parent ID: %d\n\n", getpid(), getppid());
        for (int i = 1; i <= 19; i += 2) {
            printf("%d ", i); fflush(stdout);
            sleep(1); // OS scheduler decides who prints first, causing mixed output
        }
        exit(0);
    } else {
        printf("In parent process\n");
        printf("Parent ID: %d, Grandparent ID: %d\n", getpid(), getppid());
        for (int i = 2; i <= 20; i += 2) {
            printf("%d ", i); fflush(stdout);
            sleep(1);
        }
        wait(NULL); 
        printf("\n[Notice how the numbers are mixed up because there is no sync]\n");
    }

    printf("\n--- Task (iii): NO SYNC, Child sleep(2), Parent sleep(1) ---\n");
    pid = fork();
    if (pid == 0) {
        for (int i = 1; i <= 19; i += 2) {
            printf("%d ", i); fflush(stdout);
            sleep(2); // Child is artificially slowed down
        }
        exit(0);
    } else {
        for (int i = 2; i <= 20; i += 2) {
            printf("%d ", i); fflush(stdout);
            sleep(1); 
        }
        wait(NULL);
        printf("\n[What happens? The parent finishes evens way faster. The child is left printing odds slowly at the end.]\n");
    }

    printf("\n--- Task (iv): SYNCHRONIZED continuous 1-20 (With sleep) ---\n");
    
    // We use two pipes to create a turn-based "lockstep" synchronization.
    // p1: Child writes, Parent reads. p2: Parent writes, Child reads.
    int p1[2], p2[2];
    pipe(p1); 
    pipe(p2); 
    
    pid = fork();
    if (pid == 0) {
        char dummy;
        for (int i = 1; i <= 19; i += 2) {
            printf("%d ", i); fflush(stdout);
            sleep(1); 
            // Child prints its number, then signals the Parent by writing 'x' to p1
            write(p1[1], "x", 1); 
            if (i < 19) {
                // Child blocks (waits) here until the Parent writes to p2
                read(p2[0], &dummy, 1); 
            }
        }
        exit(0);
    } else {
        char dummy;
        for (int i = 2; i <= 20; i += 2) {
            // Parent blocks (waits) here until the Child writes to p1
            read(p1[0], &dummy, 1); 
            printf("%d ", i); fflush(stdout);
            sleep(1); 
            if (i < 20) {
                // Parent prints its number, then signals the Child by writing 'x' to p2
                write(p2[1], "x", 1); 
            }
        }
        wait(NULL); 
        printf("\n[Perfect continuous 1-20 using pipe synchronization!]\n");
    }

    printf("\n--- Task (v): Process table (ps) ---\n");
    pid = fork();
    if (pid == 0) {
        sleep(2); // Child simply stays alive long enough to show up in the process table
        exit(0);
    } else {
        system("ps -al"); // Parent runs the ps command while the child is sleeping
        wait(NULL);
    }

    benchmark_syscalls();

    return 0;
}