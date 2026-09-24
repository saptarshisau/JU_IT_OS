/***********************************************************************
*
* Name- Ankit Roy, Roll- 002411001028
* Name- Saptarshi Sau, Roll- 002411001031
* Team-Number-05, Section- A1
* Date: 11th September 2026
*

* 
* Compilation command: gcc A1_05_ASN-3.c 
* Execution sequence: ./a.out

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESSES 200
#define MAX_GANTT 5000
#define MAX_QUEUE 10000

typedef struct {
    char pid[10];
    int at;
    int bt;
    int priority;
    int rt;         // Remaining time
    int start_time;
    int ct;         // Completion time
    int tat;        // Turnaround time
    int wt;         // Waiting time
    int resp_t;     // Response time
} Process;

typedef struct {
    char pid[10];
    int start;
    int end;
} GanttEntry;

typedef struct {
    char algo_name[50];
    float avg_tat;
    float avg_wt;
    float avg_rt;
    bool has_run;
} Result;

// Global variables to store comparison results
Result results[6]; 
int num_results = 0;

// Read processes from CSV
int read_processes(const char* filename, Process processes[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open '%s'.\n", filename);
        return 0;
    }

    char line[100];
    int count = 0;
    
    // Skip header
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        // Parse CSV line: PID, AT, BT, Priority (Strict check for exactly 4 fields)
        if (sscanf(line, "%[^,],%d,%d,%d", 
               processes[count].pid, 
               &processes[count].at, 
               &processes[count].bt, 
               &processes[count].priority) == 4) {
               
            processes[count].rt = processes[count].bt;
            processes[count].start_time = -1;
            processes[count].ct = 0;
            processes[count].tat = 0;
            processes[count].wt = 0;
            processes[count].resp_t = 0;
            count++;
        }
    }
    fclose(file);
    return count;
}

void copy_processes(Process src[], Process dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void calculate_metrics(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].tat = processes[i].ct - processes[i].at;
        processes[i].wt = processes[i].tat - processes[i].bt;
        processes[i].resp_t = processes[i].start_time - processes[i].at;
    }
}

void add_to_gantt(GanttEntry gantt[], int *gantt_size, const char* pid, int start, int end) {
    if (*gantt_size > 0 && strcmp(gantt[*gantt_size - 1].pid, pid) == 0) {
        // Condense continuous execution blocks
        gantt[*gantt_size - 1].end = end;
    } else {
        strcpy(gantt[*gantt_size].pid, pid);
        gantt[*gantt_size].start = start;
        gantt[*gantt_size].end = end;
        (*gantt_size)++;
    }
}

// Fixed width chunked Gantt Chart printer
void print_gantt_chart(GanttEntry gantt[], int gantt_size) {
    printf("\n--- Gantt Chart ---\n");
    if (gantt_size == 0) return;

    int items_per_row = 10; // Adjust this if your terminal is narrower/wider
    
    for (int i = 0; i < gantt_size; i += items_per_row) {
        int end_idx = (i + items_per_row < gantt_size) ? i + items_per_row : gantt_size;
        
        // Print the top row (Process IDs)
        for (int j = i; j < end_idx; j++) {
            printf("| %-6s ", gantt[j].pid); 
        }
        printf("|\n");
        
        // Print the bottom row (Times)
        printf("%-9d", gantt[i].start);
        for (int j = i; j < end_idx; j++) {
            if (j == end_idx - 1) {
                printf("%d", gantt[j].end);
            } else {
                printf("%-9d", gantt[j].end);
            }
        }
        printf("\n\n");
    }
}

void print_metrics(Process p[], int n, const char* algo_name) {
    float total_tat = 0, total_wt = 0, total_rt = 0;
    int page_size = 20;
    
    // Calculate totals first so early quit ('q') doesn't ruin the averages
    for (int i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        total_rt += p[i].resp_t;
    }

    printf("\n=== Detailed Metrics: %s ===\n", algo_name);
    printf("%-5s | %-3s | %-3s | %-3s | %-4s | %-3s | %-3s\n", "PID", "AT", "BT", "CT", "TAT", "WT", "RT");
    printf("---------------------------------------------\n");
    
    // Clear the input buffer
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    // Print with Pagination
    for (int i = 0; i < n; i++) {
        printf("%-5s | %-3d | %-3d | %-3d | %-4d | %-3d | %-3d\n", 
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt, p[i].resp_t);
        
        // Pagination Check
        if ((i + 1) % page_size == 0 && (i + 1) < n) {
            printf("---------------------------------------------\n");
            printf("Showing %d of %d records. Press [Enter] for next page or 'q' to stop: ", i + 1, n);
            
            char input[10];
            if (fgets(input, sizeof(input), stdin) != NULL) {
                if (input[0] == 'q' || input[0] == 'Q') {
                    break;
                }
            }
            
            printf("\n%-5s | %-3s | %-3s | %-3s | %-4s | %-3s | %-3s\n", "PID", "AT", "BT", "CT", "TAT", "WT", "RT");
            printf("---------------------------------------------\n");
        }
    }
    printf("---------------------------------------------\n");
    
    float avg_tat = total_tat / n;
    float avg_wt = total_wt / n;
    float avg_rt = total_rt / n;
    
    printf("End of records for %s.\n", algo_name);
    printf("Average Turnaround Time: %.2f\n", avg_tat);
    printf("Average Waiting Time:    %.2f\n", avg_wt);
    printf("Average Response Time:   %.2f\n\n", avg_rt);
    
    // Save to results array
    strcpy(results[num_results].algo_name, algo_name);
    results[num_results].avg_tat = avg_tat;
    results[num_results].avg_wt = avg_wt;
    results[num_results].avg_rt = avg_rt;
    results[num_results].has_run = true;
    num_results++;
}

// Custom sort by arrival time
void sort_by_arrival(Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j+1].at) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
}

// ---------------- ALGORITHMS ----------------

void fcfs(Process p[], int n) {
    sort_by_arrival(p, n);
    GanttEntry gantt[MAX_GANTT];
    int g_size = 0;
    int time = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at) {
            add_to_gantt(gantt, &g_size, "IDLE", time, p[i].at);
            time = p[i].at;
        }
        p[i].start_time = time;
        add_to_gantt(gantt, &g_size, p[i].pid, time, time + p[i].bt);
        time += p[i].bt;
        p[i].ct = time;
    }
    
    calculate_metrics(p, n);
    print_gantt_chart(gantt, g_size);
    print_metrics(p, n, "FCFS");
}

void sjf(Process p[], int n) {
    int time = 0, completed = 0;
    GanttEntry gantt[MAX_GANTT];
    int g_size = 0;
    
    while (completed != n) {
        int idx = -1;
        int min_bt = 999999;
        
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                if (p[i].bt < min_bt || (p[i].bt == min_bt && p[i].at < p[idx].at)) {
                    min_bt = p[i].bt;
                    idx = i;
                }
            }
        }
        
        if (idx != -1) {
            p[idx].start_time = time;
            add_to_gantt(gantt, &g_size, p[idx].pid, time, time + p[idx].bt);
            time += p[idx].bt;
            p[idx].ct = time;
            p[idx].rt = 0;
            completed++;
        } else {
            add_to_gantt(gantt, &g_size, "IDLE", time, time + 1);
            time++;
        }
    }
    
    calculate_metrics(p, n);
    print_gantt_chart(gantt, g_size);
    print_metrics(p, n, "SJF");
}

void priority_scheduling(Process p[], int n) {
    int time = 0, completed = 0;
    GanttEntry gantt[MAX_GANTT];
    int g_size = 0;
    
    while (completed != n) {
        int idx = -1;
        int min_pr = 999999; // Lower number = higher priority
        
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                if (p[i].priority < min_pr || (p[i].priority == min_pr && p[i].at < p[idx].at)) {
                    min_pr = p[i].priority;
                    idx = i;
                }
            }
        }
        
        if (idx != -1) {
            p[idx].start_time = time;
            add_to_gantt(gantt, &g_size, p[idx].pid, time, time + p[idx].bt);
            time += p[idx].bt;
            p[idx].ct = time;
            p[idx].rt = 0;
            completed++;
        } else {
            add_to_gantt(gantt, &g_size, "IDLE", time, time + 1);
            time++;
        }
    }
    
    calculate_metrics(p, n);
    print_gantt_chart(gantt, g_size);
    print_metrics(p, n, "Priority");
}

void round_robin(Process p[], int n, int quantum) {
    sort_by_arrival(p, n);
    int time = 0, completed = 0;
    GanttEntry gantt[MAX_GANTT];
    int g_size = 0;
    
    int queue[MAX_QUEUE];
    int front = 0, rear = 0;
    bool in_queue[MAX_PROCESSES] = {false};
    
    for (int i = 0; i < n; i++) {
        if (p[i].at <= time) {
            queue[rear++] = i;
            in_queue[i] = true;
        }
    }
    
    while (completed != n) {
        if (front == rear) { // Queue empty
            add_to_gantt(gantt, &g_size, "IDLE", time, time + 1);
            time++;
            for (int i = 0; i < n; i++) {
                if (p[i].at <= time && !in_queue[i] && p[i].rt > 0) {
                    queue[rear++] = i;
                    in_queue[i] = true;
                }
            }
            continue;
        }
        
        int idx = queue[front++];
        if (p[idx].start_time == -1) p[idx].start_time = time;
        
        int exec_time = (p[idx].rt < quantum) ? p[idx].rt : quantum;
        add_to_gantt(gantt, &g_size, p[idx].pid, time, time + exec_time);
        
        time += exec_time;
        p[idx].rt -= exec_time;
        
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && !in_queue[i] && p[i].rt > 0) {
                queue[rear++] = i;
                in_queue[i] = true;
            }
        }
        
        if (p[idx].rt > 0) {
            queue[rear++] = idx; 
        } else {
            p[idx].ct = time;
            completed++;
        }
    }
    
    calculate_metrics(p, n);
    print_gantt_chart(gantt, g_size);
    char name[50];
    sprintf(name, "Round Robin (Q=%d)", quantum);
    print_metrics(p, n, name);
}

void psrtf(Process p[], int n) {
    int time = 0, completed = 0;
    GanttEntry gantt[MAX_GANTT];
    int g_size = 0;
    
    while (completed != n) {
        int idx = -1;
        int min_rt = 999999;
        
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                if (p[i].rt < min_rt || (p[i].rt == min_rt && p[i].at < p[idx].at)) {
                    min_rt = p[i].rt;
                    idx = i;
                }
            }
        }
        
        if (idx != -1) {
            if (p[idx].start_time == -1) p[idx].start_time = time;
            
            add_to_gantt(gantt, &g_size, p[idx].pid, time, time + 1);
            time++;
            p[idx].rt--;
            
            if (p[idx].rt == 0) {
                p[idx].ct = time;
                completed++;
            }
        } else {
            add_to_gantt(gantt, &g_size, "IDLE", time, time + 1);
            time++;
        }
    }
    
    calculate_metrics(p, n);
    print_gantt_chart(gantt, g_size);
    print_metrics(p, n, "PSRTF");
}

void mlfq(Process p[], int n) {
    int time = 0, completed = 0;
    GanttEntry gantt[MAX_GANTT];
    int g_size = 0;
    
    int q1[MAX_QUEUE], q2[MAX_QUEUE], q3[MAX_QUEUE];
    int f1=0, r1=0, f2=0, r2=0, f3=0, r3=0;
    bool in_system[MAX_PROCESSES] = {false};
    
    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && !in_system[i] && p[i].rt > 0) {
                q1[r1++] = i;
                in_system[i] = true;
            }
        }
        
        if (f1 < r1) {
            int idx = q1[f1++];
            if (p[idx].start_time == -1) p[idx].start_time = time;
            
            int exec_time = (p[idx].rt < 2) ? p[idx].rt : 2;
            add_to_gantt(gantt, &g_size, p[idx].pid, time, time + exec_time);
            time += exec_time;
            p[idx].rt -= exec_time;
            
            for (int i = 0; i < n; i++) {
                if (p[i].at <= time && !in_system[i] && p[i].rt > 0) {
                    q1[r1++] = i;
                    in_system[i] = true;
                }
            }
            
            if (p[idx].rt > 0) {
                q2[r2++] = idx; 
            } else {
                p[idx].ct = time;
                completed++;
            }
        } 
        else if (f2 < r2) {
            int idx = q2[f2++];
            if (p[idx].start_time == -1) p[idx].start_time = time;
            
            int ticks = 0;
            bool preempted = false;
            while (ticks < 4 && p[idx].rt > 0) {
                add_to_gantt(gantt, &g_size, p[idx].pid, time, time + 1);
                time++;
                p[idx].rt--;
                ticks++;
                
                for (int i = 0; i < n; i++) {
                    if (p[i].at == time && !in_system[i] && p[i].rt > 0) {
                        q1[r1++] = i;
                        in_system[i] = true;
                        preempted = true;
                    }
                }
                if (preempted) break;
            }
            
            if (p[idx].rt > 0) {
                if (preempted) {
                    q2[r2++] = idx; 
                } else {
                    q3[r3++] = idx; 
                }
            } else {
                p[idx].ct = time;
                completed++;
            }
        }
        else if (f3 < r3) {
            int idx = q3[f3++];
            if (p[idx].start_time == -1) p[idx].start_time = time;
            
            bool preempted = false;
            while (p[idx].rt > 0) {
                add_to_gantt(gantt, &g_size, p[idx].pid, time, time + 1);
                time++;
                p[idx].rt--;
                
                for (int i = 0; i < n; i++) {
                    if (p[i].at == time && !in_system[i] && p[i].rt > 0) {
                        q1[r1++] = i;
                        in_system[i] = true;
                        preempted = true;
                    }
                }
                if (preempted) break;
            }
            
            if (p[idx].rt > 0) {
                q3[r3++] = idx;
            } else {
                p[idx].ct = time;
                completed++;
            }
        } 
        else {
            add_to_gantt(gantt, &g_size, "IDLE", time, time + 1);
            time++;
        }
    }
    
    calculate_metrics(p, n);
    print_gantt_chart(gantt, g_size);
    print_metrics(p, n, "MLFQ");
}

void show_comparison() {
    if (num_results == 0) {
        printf("\nNo algorithms have been run yet.\n");
        return;
    }
    
    printf("\n=== Performance Comparison Table ===\n");
    printf("%-20s | %-10s | %-10s | %-10s\n", "Algorithm", "Avg TAT", "Avg WT", "Avg RT");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < num_results; i++) {
        printf("%-20s | %-10.2f | %-10.2f | %-10.2f\n", 
               results[i].algo_name, 
               results[i].avg_tat, 
               results[i].avg_wt, 
               results[i].avg_rt);
    }
    printf("\n");
}

int main() {
    Process original[MAX_PROCESSES];
    Process active[MAX_PROCESSES];
    
    int n = read_processes("Process_Scheduling_Data.csv", original);
    if (n == 0) {
        return 1;
    }

    int choice;
    do {
        printf("\n=== CPU Scheduling Simulator (C) ===\n");
        printf("1. First Come First Serve (FCFS)\n");
        printf("2. Shortest Job First (SJF) - Non-Preemptive\n");
        printf("3. Priority - Non-Preemptive\n");
        printf("4. Round Robin (RR)\n");
        printf("5. Preemptive Shortest Remaining Time First (PSRTF)\n");
        printf("6. Multi-Level Feedback Queue (MLFQ)\n");
        printf("7. Display Comparison Table\n");
        printf("8. Exit\n");
        printf("Enter your choice (1-8): ");
        scanf("%d", &choice);
        
        if (choice >= 1 && choice <= 6) {
            copy_processes(original, active, n); 
        }
        
        switch(choice) {
            case 1:
                printf("\nExecuting FCFS...\n");
                fcfs(active, n);
                break;
            case 2:
                printf("\nExecuting SJF (Non-Preemptive)...\n");
                sjf(active, n);
                break;
            case 3:
                printf("\nExecuting Priority (Non-Preemptive)...\n");
                priority_scheduling(active, n);
                break;
            case 4: {
                int q;
                printf("Enter Time Quantum for Round Robin: ");
                scanf("%d", &q);
                if (q > 0) {
                    printf("\nExecuting Round Robin (Q=%d)...\n", q);
                    round_robin(active, n, q);
                } else {
                    printf("Invalid Time Quantum.\n");
                }
                break;
            }
            case 5:
                printf("\nExecuting PSRTF...\n");
                psrtf(active, n);
                break;
            case 6:
                printf("\nExecuting MLFQ (Q1=RR2, Q2=RR4, Q3=FCFS)...\n");
                mlfq(active, n);
                break;
            case 7:
                show_comparison();
                break;
            case 8:
                printf("Exiting Simulator...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 8);

    return 0;
}