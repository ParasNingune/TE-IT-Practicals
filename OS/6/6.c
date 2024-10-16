#include <stdio.h>
#include <limits.h>
#define MAX_PAGES 20
 
typedef struct {
    char data[MAX_PAGES];
    int end;              
} queue;
 
void enqueue(queue *q, char data);
char dequeue(queue *q);
void fifo(char string[], int frameSize, int count);
void lru(char string[], int frameSize, int count);
void optimal(char string[], int frameSize, int count);
 
int main() {
    int frameSize, count, ch;
    char string[50];
    printf("Enter the page reference string (end with newline): ");
    count = 0;
    do {
        scanf("%c", &string[count]);
        count++;
    } while (string[count - 1] != '\n');
    count--;
    printf("\nEnter the size of the frame (minimum 3): ");
    scanf("%d", &frameSize);
    if (frameSize < 3) {
        printf("Frame size must be at least 3.\n");
        return 1;
    }
    do {
        printf("\nMENU\n====\n1. FIFO\n2. Least Recently Used (LRU)\n3. Optimal\n4. Exit\n\nYour Choice: ");
        scanf("%d", &ch);
        switch (ch) {
            case 1:
                printf("\nRunning FIFO page replacement algorithm...\n");
                fifo(string, frameSize, count);
                break;
            case 2:
                printf("\nRunning LRU page replacement algorithm...\n");
                lru(string, frameSize, count);
                break;
            case 3:
                printf("\nRunning Optimal page replacement algorithm...\n");
                optimal(string, frameSize, count);
                break;
            case 4:
                break;
            default:
                printf("\nInvalid choice! Please try again!");
                continue;
}} while (ch != 4);
    return 0; }
void enqueue(queue *q, char data) {
    q->data[q->end] = data;
    q->end++;
}
char dequeue(queue *q) {
    char value = q->data[0];
    for (int i = 0; i < q->end - 1; i++) {
        q->data[i] = q->data[i + 1];
    } q->end--;
    return value; }
void fifo(char string[], int frameSize, int count) {
    queue q;
    int pageFaults = 0;
    int flag;  
    q.end = 0;
    printf("\nData Requested\tFrame contents\t    Page Fault\n==============================================");
    for (int i = 0; i < count; i++) {
        printf("\n\n\t%c", string[i]);
        flag = 0;
        for (int j = 0; j < q.end; j++) {
            if (string[i] == q.data[j]) {
                flag = 1;
                break;
            }  }
        if (flag == 0) {
            pageFaults++;
            if (q.end < frameSize) {
                enqueue(&q, string[i]);
            } else {
                dequeue(&q);
                enqueue(&q, string[i]);
            }
            printf("\t  ");
            for (int j = 0; j < q.end; j++) {
                printf("%c   ", q.data[j]);
            }
            printf("\t\tY");
        } else {
            printf("\t  ");
            for (int j = 0; j < q.end; j++) {
                printf("%c   ", q.data[j]);
            }
            printf("\t\tN");
        }   }
    printf("\n\n==============================================\n");
    printf("\nTotal no. of Page Faults: %d\n\n", pageFaults);​}
void lru(char string[], int frameSize, int count) {
    queue q;
    int pageFaults = 0;
    q.end = 0;
    int recent[MAX_PAGES];
    printf("\nData Requested\tFrame contents\t    Page Fault\n==============================================");
    for (int i = 0; i < count; i++) {
        printf("\n\n\t%c", string[i]);
        int flag = 0;
        for (int j = 0; j < q.end; j++) {
            if (string[i] == q.data[j]) {
                flag = 1;
                recent[j] = i;
                break;    }    }
        if (flag == 0) {
            pageFaults++;
            if (q.end < frameSize) {
                enqueue(&q, string[i]);
                recent[q.end - 1] = i;
            } else {
                int lruIndex = 0;
                for (int j = 1; j < frameSize; j++) {
                    if (recent[j] < recent[lruIndex]) {
                        lruIndex = j;​}     }
                dequeue(&q);
                enqueue(&q, string[i]);
                recent[lruIndex] = i;
            }   printf("\t  ");
            for (int j = 0; j < q.end; j++) {
                printf("%c   ", q.data[j]);
            }    printf("\t\tY");
        } else {
            printf("\t  ");
            for (int j = 0; j < q.end; j++) {
                printf("%c   ", q.data[j]);  }
            printf("\t\tN");
        }   }
    printf("\n\n==============================================\n");
    printf("\nTotal no. of Page Faults: %d\n\n", pageFaults);}
void optimal(char string[], int frameSize, int count) {
    queue q;
    int pageFaults = 0;
    q.end = 0;
    printf("\nData Requested\tFrame contents\t    Page Fault\n==============================================");
    for (int i = 0; i < count; i++) {
        printf("\n\n\t%c", string[i]);
        int flag = 0;
        for (int j = 0; j < q.end; j++) {
            if (string[i] == q.data[j]) {
                flag = 1;
                break;   }     }
        if (flag == 0) {
            pageFaults++;
            if (q.end < frameSize) {
                enqueue(&q, string[i]);
            } else {
                int replaceIndex = -1, farthest = -1;
                for (int j = 0; j < q.end; j++) {
                    int found = 0;
                    for (int k = i + 1; k < count; k++) {
                        if (q.data[j] == string[k]) {
                            found = 1;
                            if (k > farthest) {
                                farthest = k;
                                replaceIndex = j;   }
                            break;   }    }
                    if (!found) {
                        replaceIndex = j;
                        break;   }  }
                q.data[replaceIndex] = string[i];
            }    printf("\t  ");
            for (int j = 0; j < q.end; j++) {
                printf("%c   ", q.data[j]);
            }   printf("\t\tY");
        } else {
            printf("\t  ");
            for (int j = 0; j < q.end; j++) {
                printf("%c   ", q.data[j]);  }
            printf("\t\tN");     }   }
    printf("\n\n==============================================\n");
    printf("\nTotal no. of Page Faults: %d\n\n", pageFaults);
}
 
 