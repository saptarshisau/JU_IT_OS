/***********************************************************************
*
* Name- Ankit Roy, Roll- 002411001028
* Name- Saptarshi Sau, Roll- 002411001031
* Team-Number-05, Section- A1
* Date: 21st August 2026
*
* Assignment Details: Demonstrate the usage of Linux system calls of
* EXEC FAMILY (execl, execv, execlp, execvp) for executing various
* Linux commands like whoami, ls, pwd. Every exec call is used with
* every command, giving 4 x 3 = 12 cases.
* Input Description: None.
* Output Description: Standard output of the respective Linux commands.
*
*
* Compilation command: gcc A1_05_ASN-2_Q3.c
* Execution sequence: ./a.out
*
* Sample Input and Output generated:
* --- Case 1: execl  -> pwd ---
* /home/ankit/assignment
* --- Case 2: execl  -> whoami ---
* ankit
* --- Case 3: execl  -> ls ---
* prog1.c prog2.c prog3.c
* ... (Cases 4-12 repeat the same three commands using execv,
*      execlp and execvp)
*
***********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

// exec functions only return on failure, so every case ends with perror.

// ---------------------------------------------------------
// execl: 'l' = List of arguments, terminated by NULL.
// Needs the full path of the executable.
// ---------------------------------------------------------
void execl_pwd(void) {
    execl("/bin/pwd", "pwd", (char *)NULL);
    perror("execl pwd failed");
}

void execl_whoami(void) {
    execl("/usr/bin/whoami", "whoami", (char *)NULL);
    perror("execl whoami failed");
}

void execl_ls(void) {
    execl("/bin/ls", "ls", (char *)NULL);
    perror("execl ls failed");
}

// ---------------------------------------------------------
// execv: 'v' = Vector (NULL-terminated array) of arguments.
// Needs the full path of the executable.
// ---------------------------------------------------------
void execv_pwd(void) {
    char *args[] = {"pwd", NULL};
    execv("/bin/pwd", args);
    perror("execv pwd failed");
}

void execv_whoami(void) {
    char *args[] = {"whoami", NULL};
    execv("/usr/bin/whoami", args);
    perror("execv whoami failed");
}

void execv_ls(void) {
    char *args[] = {"ls", NULL};
    execv("/bin/ls", args);
    perror("execv ls failed");
}

// ---------------------------------------------------------
// execlp: 'l' = List of arguments, 'p' = searches PATH.
// Only the command name is needed, not the full path.
// ---------------------------------------------------------
void execlp_pwd(void) {
    execlp("pwd", "pwd", (char *)NULL);
    perror("execlp pwd failed");
}

void execlp_whoami(void) {
    execlp("whoami", "whoami", (char *)NULL);
    perror("execlp whoami failed");
}

void execlp_ls(void) {
    execlp("ls", "ls", (char *)NULL);
    perror("execlp ls failed");
}

// ---------------------------------------------------------
// execvp: 'v' = Vector of arguments, 'p' = searches PATH.
// Only the command name is needed, not the full path.
// ---------------------------------------------------------
void execvp_pwd(void) {
    char *args[] = {"pwd", NULL};
    execvp("pwd", args);
    perror("execvp pwd failed");
}

void execvp_whoami(void) {
    char *args[] = {"whoami", NULL};
    execvp("whoami", args);
    perror("execvp whoami failed");
}

void execvp_ls(void) {
    char *args[] = {"ls", NULL};
    execvp("ls", args);
    perror("execvp ls failed");
}

// Forks a child that runs the given exec case; the parent waits for it.
int run_case(int num, const char *title, void (*exec_case)(void)) {
    printf("\n--- Case %d: %s ---\n", num, title);
    fflush(stdout); // flush before fork so buffered text is not lost/reordered

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        exec_case();
        exit(1); // reached only if the exec call failed
    }
    wait(NULL);
    return 0;
}

int main() {
    struct {
        const char *title;
        void (*fn)(void);
    } cases[] = {
        {"execl  -> pwd",    execl_pwd},
        {"execl  -> whoami", execl_whoami},
        {"execl  -> ls",     execl_ls},
        {"execv  -> pwd",    execv_pwd},
        {"execv  -> whoami", execv_whoami},
        {"execv  -> ls",     execv_ls},
        {"execlp -> pwd",    execlp_pwd},
        {"execlp -> whoami", execlp_whoami},
        {"execlp -> ls",     execlp_ls},
        {"execvp -> pwd",    execvp_pwd},
        {"execvp -> whoami", execvp_whoami},
        {"execvp -> ls",     execvp_ls},
    };
    int total = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < total; i++) {
        if (run_case(i + 1, cases[i].title, cases[i].fn) != 0) {
            return 1;
        }
    }

    printf("\nAll %d exec cases completed successfully.\n", total);
    return 0;
}
