/***********************************************************************
*
* Name- Ankit Roy, Roll- 002411001028
* Name- Saptarshi Sau, Roll- 002411001031
* Team-Number-05, Section- A1
* Date: 21st August 2026
*
* Assignment Details:
* Write a program to display process information 
* using process related system calls like getpid, getppid.
* Input Description: None.
* Output Description: Prints process ID and parent process ID for both 
* child and parent processes.
* 
* Compilation command: gcc A1_05_ASN-2_Q1.c 
* Execution sequence: ./a.out
* 
* Sample Input and Output generated:
* Child Processing
* I am child and my process id is 22518
* I am child and my parents process id is 22519
* Parent processing
* I am parent and my process id is 22519
* I am parent and my parents process id is 22517
*
***********************************************************************/

#include <stdio.h>      // Required for standard input/output functions like printf and fprintf
#include <unistd.h>     // Required for POSIX operating system API, including fork(), getpid(), and getppid()
#include <sys/types.h>  // Required for defining system data types like pid_t
#include <sys/wait.h>   // Required for the wait() system call

int main() {
    // fork() creates a new child process by duplicating the calling process.
    // It returns a process ID (pid_t) which helps us distinguish between the parent and child.
    pid_t pid = fork();

    // Condition 1: If fork() returns a negative value, the creation of a child process was unsuccessful.
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1; // Exit with an error code
    } 
    // Condition 2: If fork() returns exactly 0, we are currently executing inside the newly created Child process.
    else if (pid == 0) {
        printf("Child Processing\n");
        
        // getpid() returns the process ID of the current process (the child).
        printf("I am child and my process id is %d\n", getpid());
        
        // getppid() returns the process ID of the parent of the current process.
        printf("I am child and my parents process id is %d\n", getppid());
    } 
    // Condition 3: If fork() returns a positive value (the PID of the child), we are executing inside the Parent process.
    else {
        // wait(NULL) suspends execution of the parent process until its child terminates.
        // This prevents the parent from finishing before the child and creating an orphan process.
        wait(NULL); 
        
        printf("Parent processing\n");
        
        // getpid() here returns the parent's own process ID.
        printf("I am parent and my process id is %d\n", getpid());
        
        // getppid() here returns the process ID of the parent's parent (often the shell or terminal that ran the program).
        printf("I am parent and my parents process id is %d\n", getppid());
    }

    return 0; // Successful execution
}