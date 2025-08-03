#include <stdio.h>
#include <stdbool.h>

int main() {
    int numProcesses, numResources;

    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);
    printf("Enter the number of resources: ");
    scanf("%d", &numResources);

    int available[numResources], maximum[numProcesses][numResources], allocation[numProcesses][numResources], need[numProcesses][numResources];
    
    printf("\nEnter the available units for each of the %d resources:\n", numResources);
    for (int j = 0; j < numResources; j++) {
        printf("Resource %d: ", j);
        scanf("%d", &available[j]);
    }
    printf("\n");

    printf("Enter the maximum matrix for %d processes and %d resources:\n", numProcesses, numResources);
    for (int i = 0; i < numProcesses; i++) {
        printf("Process P%d (enter %d numbers): ", i, numResources);
        for (int j = 0; j < numResources; j++) 
            scanf("%d", &maximum[i][j]);
    }
    printf("\n");

    printf("Enter the allocation matrix for %d processes and %d resources:\n", numProcesses, numResources);
    for (int i = 0; i < numProcesses; i++) {
        printf("Process P%d (enter %d numbers): ", i, numResources);
        for (int j = 0; j < numResources; j++) 
            scanf("%d", &allocation[i][j]);
    }
    printf("\n");

    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) 
            need[i][j] = maximum[i][j] - allocation[i][j];
    }

    printf("Need Matrix:\n");
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) 
            printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("\n");

    bool finish[numProcesses];
    int work[numResources], safeSequence[numProcesses], count = 0;
    
    for (int j = 0; j < numResources; j++) work[j] = available[j];
    for (int i = 0; i < numProcesses; i++) finish[i] = false;

    while (count < numProcesses) {
        bool found = false;
        for (int i = 0; i < numProcesses; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < numResources; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    for (int j = 0; j < numResources; j++) 
                        work[j] += allocation[i][j];
                    safeSequence[count++] = i;
                    finish[i] = true;
                    printf("Process P%d has finished. New work: ", i);
                    for (int j = 0; j < numResources; j++) 
                        printf("%d ", work[j]);
                    printf("\n");
                    found = true;
                }
            }
        }
        if (!found) 
            break; 
    }

    if (count == numProcesses) {
        printf("\nSystem is in a SAFE state.\nSafe sequence: ");
        for (int i = 0; i < numProcesses; i++) 
            printf("P%d ", safeSequence[i]);
        printf("\n");
    } else 
        printf("\nSystem is in an UNSAFE state! No safe sequence exists.\n");
    return 0;
}
