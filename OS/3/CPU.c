#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 20

typedef struct process{
    int BT;          // Remaining Burst Time
    int original_BT; // Original Burst Time
    int AT;
    int TAT;
    int WT;
    int PNO;
    int PID;
    char name[10];
} process;

typedef struct RQ{
    process pr[MAX];
    int f, r;
} RQ;

// Function Prototypes
void get_PCB(process[], int *);
void sort_AT(process[], int);
void sort_BT(RQ *);
void disp_table(process[], int);
void SJF_P(process[], int);
void RR(process p[], int n, int);
float cal_avgwt(process[], int);
float cal_avgtat(process[], int);

// Function Definitions
void menu(){
    printf("\n\t\t****MENU*****");
    printf("\n\t\t1. RR");
    printf("\n\t\t2. SJF P");
    printf("\n\t\t3. EXIT");
    printf("\n\t\tEnter Choice: \t");
}

int main(){
    int ch, TQ, n;
    process P[MAX];
    float avg_WT, avg_TAT;

    printf("\nEnter Time Quantum for Round Robin : ");
    scanf("%d", &TQ);

    get_PCB(P, &n);

    do{
        menu();
        scanf("%d", &ch);

        switch (ch){
            case 1:
                RR(P, n, TQ);
                disp_table(P, n);
                avg_WT = cal_avgwt(P, n);
                avg_TAT = cal_avgtat(P, n);
                printf("\nAVERAGE WT  : %.2f", avg_WT);
                printf("\nAVERAGE TAT : %.2f\n", avg_TAT);
                break;

            case 2:
                SJF_P(P, n);
                disp_table(P, n);
                avg_WT = cal_avgwt(P, n);
                avg_TAT = cal_avgtat(P, n);
                printf("\nAVERAGE WT  : %.2f", avg_WT);
                printf("\nAVERAGE TAT : %.2f\n", avg_TAT);
                break;

            case 3:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid Choice! Please try again.\n");
        }
    } while (ch != 3);

    return 0;  // Return 0 to indicate successful execution
}

float cal_avgwt(process p[], int n){
    float avg = 0;
    for (int i = 0; i < n; i++){
        avg += p[i].WT;
    }
    avg = avg / n;
    return avg;
}

float cal_avgtat(process p[], int n){
    float avg = 0;
    for (int i = 0; i < n; i++){
        avg += p[i].TAT;
    }
    avg = avg / n;
    return avg;
}

int get_first_process(process p[], int n){
    if(n <= 0){
        return -1;
    }

    int min = p[0].AT;
    int in = 0;
    for(int j = 1; j < n; j++){
        if (p[j].AT < min){
            min = p[j].AT;
            in = j;
        }
    }
    return in;
}

void check_arrival(RQ *r, process p[], int time, int n){
    for (int i = 0; i < n; i++){
        if(p[i].AT == time){
            int is_in_queue = 0;
            for (int j = r->f; j <= r->r; j++){
                if (strcmp(p[i].name, r->pr[j].name) == 0){
                    is_in_queue = 1;
                    break;
                }
            }

            if (!is_in_queue){
                if(r->r + 1 < MAX){
                    r->r++;
                    r->pr[r->r] = p[i];
                    printf("Time %d: Process %s arrived and added to Ready Queue.\n", time, p[i].name);
                }
                else{
                    printf("Ready Queue is full. Cannot add process %s.\n", p[i].name);
                }
            }
        }
    }
}

void RR(process p[], int n, int tq){
    int count = 0, start, time = 0;
    RQ r;

    r.f = r.r = -1;
    start = get_first_process(p, n);
    if(start == -1){
        printf("No process available to start.\n");
        return;
    }
    r.pr[0] = p[start];
    r.f = r.r = 0;
    printf("Starting Round Robin with process %s\n", r.pr[0].name);
    check_arrival(&r, p, time, n);

    while (r.f != -1){
        printf("\nProcessing %s (PID: %d), Remaining BT: %d\n", r.pr[r.f].name, r.pr[r.f].PID, r.pr[r.f].BT);
        for (count = 0; count < tq; count++){
            if(r.pr[r.f].BT <= 0){
                break;
            }
            r.pr[r.f].BT--;
            time++;
            printf("Time: %d, %s executing, Remaining BT: %d\n", time, r.pr[r.f].name, r.pr[r.f].BT);
            if (r.pr[r.f].BT == 0){
                break;
            }
            check_arrival(&r, p, time, n);
        }

        if (r.pr[r.f].BT > 0){
            if(r.r + 1 < MAX){
                r.r++;
                r.pr[r.r] = r.pr[r.f];
                printf("Time %d: Enqueued %s back to Ready Queue.\n", time, r.pr[r.f].name);
            }
            else{
                printf("Ready Queue is full. Cannot enqueue process %s.\n", r.pr[r.f].name);
            }
        }
        else{
            p[r.pr[r.f].PID].TAT = time - p[r.pr[r.f].PID].AT;
            p[r.pr[r.f].PID].WT = p[r.pr[r.f].PID].TAT - p[r.pr[r.f].PID].original_BT;
            printf("Time %d: Process %s completed. WT: %d, TAT: %d\n", 
                   time, p[r.pr[r.f].PID].name, p[r.pr[r.f].PID].WT, p[r.pr[r.f].PID].TAT);
        }

        if (r.f == r.r)
            r.f = r.r = -1;
        else
            r.f++;
    }
}

void sort_BT(RQ *r){
    // Implemented as selection sort based on BT
    for (int i = r->f; i <= r->r; i++){
        int min_idx = i;
        for (int j = i + 1; j <= r->r; j++){
            if (r->pr[j].BT < r->pr[min_idx].BT){
                min_idx = j;
            }
        }
        if(min_idx != i){
            process temp = r->pr[i];
            r->pr[i] = r->pr[min_idx];
            r->pr[min_idx] = temp;
        }
    }
}

int get_total_time(process p[], int n){
    int sum = 0;
    for (int i = 0; i < n; i++){
        sum += p[i].original_BT;
    }
    return sum;
}

void SJF_P(process p[], int n){
    int time = 0, start, sum;
    RQ r;

    r.f = r.r = -1;
    start = get_first_process(p, n);
    if(start == -1){
        printf("No process available to start.\n");
        return;
    }
    r.pr[0] = p[start];
    r.f = r.r = 0;
    printf("Starting SJF Preemptive with process %s\n", r.pr[0].name);
    check_arrival(&r, p, time, n);
    sort_BT(&r);
    sum = get_total_time(p, n);

    for (int i = 0; i < sum; i++){
        if(r.f == -1){
            break;
        }
        if(r.pr[r.f].BT > 0){
            r.pr[r.f].BT--;
            time++;
            printf("Time: %d, %s executing, Remaining BT: %d\n", time, r.pr[r.f].name, r.pr[r.f].BT);

            if (r.pr[r.f].BT == 0){
                p[r.pr[r.f].PID].TAT = time - p[r.pr[r.f].PID].AT;
                p[r.pr[r.f].PID].WT = p[r.pr[r.f].PID].TAT - p[r.pr[r.f].PID].original_BT;
                printf("Time %d: Process %s completed. WT: %d, TAT: %d\n", 
                       time, p[r.pr[r.f].PID].name, p[r.pr[r.f].PID].WT, p[r.pr[r.f].PID].TAT);
                r.f++;
            }

            check_arrival(&r, p, time, n);
            sort_BT(&r);
        }
    }
}

void sort_AT(process p[], int n){
    // Implemented as selection sort based on AT
    for (int i = 0; i < n; i++){
        int min_idx = i;
        for (int j = i + 1; j < n; j++){
            if (p[j].AT < p[min_idx].AT){
                min_idx = j;
            }
        }
        if(min_idx != i){
            process temp = p[i];
            p[i] = p[min_idx];
            p[min_idx] = temp;
        }
    }
}

void disp_table(process p[], int n){
    printf("\n\nP_NAME\tAT\tBT\tWT\tTAT");
    for (int i = 0; i < n; i++){
        printf("\n%-10s\t%d\t%d\t%d\t%d", 
               p[i].name, p[i].AT, p[i].original_BT, p[i].WT, p[i].TAT);
    }
    printf("\n");
}
