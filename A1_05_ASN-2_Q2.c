/***********************************************************************
*
* Name- Ankit Roy, Roll- 002411001028
* Name- Saptarshi Sau, Roll- 002411001031
* Team-Number-05, Section- A1
* Date: 21st August 2026
*
* Assignment Details: Write a program to demonstrate usage of linux 
* system calls on Process Management (Fork(), Orphan, Zombie, Exec(), 
* Wait(), and Sleep()).
* Input Description: None.
* Output Description: Demonstrates process status for Zombie process 
* via 'ps' command and shows adoption of an Orphan process by init (1).
* 
*  
*
*
* Compilation command: gcc A1_05_ASN-2_Q2.c 
* Execution sequence: ./a.out
*
*
*
* 
* Sample Input and Output generated:
* --- ZOMBIE PROCESS DEMONSTRATION ---
* Flag  S  uid    pid     ppid    c     PRI    tty     time         cmd 
* I        z   501   201    202     0      76    pts/3   2:00:00    a.out
* 
* --- ORPHAN PROCESS DEMONSTRATION ---
* Child processed:34567 childs parent process id:344567 
* Parent process ppid: 344567 
* Orphan process parent is init process whose id is 1
*
***********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("--- ZOMBIE PROCESS DEMONSTRATION ---\n");
    
    // Create the first child process
    pid_t pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } 
    else if (pid1 > 0) {
        // PARENT PROCESS
        // Sleep guarantees the child finishes execution before the parent calls wait()
        sleep(1); 
        
        printf("Parent process ID: %d\n", getpid());
        printf("Parent's PPID: %d\n", getppid());
        printf("Child process ID (Zombie): %d\n", pid1);
        printf("\nExecuting: ps -l | grep Z\n");
        
        // The child has exited, but the parent hasn't acknowledged it yet. 
        // This leaves the child in the process table as a "Zombie" (Z state).
        system("ps -l | grep Z");
        
        // Parent finally collects the child's exit status, cleaning up the zombie
        wait(NULL); 
        
        printf("\n--- ORPHAN PROCESS DEMONSTRATION ---\n");
        
        // Create a new branch to demonstrate the orphan state cleanly
        pid_t main_pid = fork();
        
        if (main_pid < 0) {
            fprintf(stderr, "Fork failed\n");
            return 1;
        }
        else if (main_pid > 0) {
            // Main program waits for the entire orphan demonstration branch to finish
            wait(NULL); 
            sleep(4); 
        }
        else if (main_pid == 0) {
            // Inside the sub-process branch
            pid_t pid2 = fork();
            
            if (pid2 > 0) {
                // SUB-PARENT PROCESS
                printf("Parent process ppid: %d\n", getppid());
                sleep(1); 
                // Sub-parent exits voluntarily while its child is still running.
                exit(0); 
            } 
            else if (pid2 == 0) {
                // SUB-CHILD PROCESS
                printf("Before Orphan -> Child PID: %d, Parent PID: %d\n", getpid(), getppid());
                
                // Sleep ensures the sub-parent terminates first. 
                // When the sub-parent dies, this process is adopted by 'init' (PID 1).
                sleep(3); 
                
                printf("After Orphan -> Child PID: %d, Parent is init process whose id is %d\n", getpid(), getppid());
                exit(0);
            }
        }
    } 
    else if (pid1 == 0) {
        // CHILD PROCESS 1
        // Exits immediately so the sleeping parent turns it into a zombie
        exit(0);
    }

    return 0;
}