/***********************************************************************
*
* Name- Ankit Roy, Roll- 002411001028
* Name- Saptarshi Sau, Roll- 002411001031
* Team-Number-05, Section- A1
* Date: 21st August 2026
*
* Assignment Details: Demonstrate the usage of Linux system calls of 
* EXEC FAMILY (execl, execv, execlp, execvp) for executing various 
* Linux commands like whoami, ls, pwd.
* Input Description: None.
* Output Description: Standard output of the respective Linux commands.
* 
*
* Compilation command: gcc A1_05_ASN-2_Q3.c 
* Execution sequence: ./a.out
* 
* Sample Input and Output generated:
* --- 1. execl demonstrating 'pwd' ---
* /home/ankit/assignment
* --- 2. execv demonstrating 'whoami' ---
* ankit
* --- 3. execlp demonstrating 'ls' ---
* prog1.c prog2.c prog3.c
* --- 4. execvp demonstrating 'ls -l' ---
* total 12
* -rw-r--r-- 1 ankit ankit ... prog1.c
*
***********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // ---------------------------------------------------------
    // 1. execl: 'l' stands for List. 
    // Requires the absolute/relative path to the executable.
    // Arguments are passed as a comma-separated list, ending with NULL.
    // ---------------------------------------------------------
    printf("\n--- 1. execl demonstrating 'pwd' ---\n");
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Path: "/bin/pwd", Arg0: "pwd", Terminator: NULL
        execl("/bin/pwd", "pwd", (char *)NULL);
        perror("execl failed"); // This only runs if execl fails to replace the process image
        exit(1);
    } else {
        wait(NULL); // Parent waits for the 'pwd' command to finish
    }

    // ---------------------------------------------------------
    // 2. execv: 'v' stands for Vector (Array).
    // Requires the absolute/relative path to the executable.
    // Arguments are passed as an array of character pointers.
    // ---------------------------------------------------------
    printf("\n--- 2. execv demonstrating 'whoami' ---\n");
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Define the argument vector. Must end with NULL.
        char *args[] = {"whoami", NULL}; 
        // Path: "/usr/bin/whoami", Vector: args
        execv("/usr/bin/whoami", args);
        perror("execv failed");
        exit(1);
    } else {
        wait(NULL);
    }

    // ---------------------------------------------------------
    // 3. execlp: 'p' means it searches the PATH environment variable.
    // 'l' stands for List.
    // You only need to provide the command name, not the full path.
    // ---------------------------------------------------------
    printf("\n--- 3. execlp demonstrating 'ls' ---\n");
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Command: "ls", Arg0: "ls", Terminator: NULL
        // The OS will automatically look for "ls" in /bin, /usr/bin, etc.
        execlp("ls", "ls", (char *)NULL);
        perror("execlp failed");
        exit(1);
    } else {
        wait(NULL);
    }

    // ---------------------------------------------------------
    // 4. execvp: 'p' means it searches the PATH environment variable.
    // 'v' stands for Vector (Array).
    // You provide the command name and an array of arguments.
    // ---------------------------------------------------------
    printf("\n--- 4. execvp demonstrating 'ls -l' ---\n");
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Vector includes the command, the "-l" flag, and the NULL terminator
        char *args[] = {"ls", "-l", NULL};
        // Command: "ls", Vector: args
        execvp("ls", args);
        perror("execvp failed");
        exit(1);
    } else {
        wait(NULL);
    }

    printf("\nAll exec calls completed successfully.\n");
    return 0;
}