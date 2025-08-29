#include <stdio.h>
#include <stdlib.h>

void fcfs(int requests[], int n, int head) {
    int total_movement = 0;
    printf("\nFCFS Disk Scheduling Steps:\n");
    for (int i = 0; i < n; i++) {
        printf("Head moves from %d to %d, movement = %d\n", head, requests[i], abs(requests[i] - head));
        total_movement += abs(requests[i] - head);
        head = requests[i];
    }
    printf("Total Head Movement (FCFS): %d\n", total_movement);
}

void scan(int requests[], int n, int head, int disk_size, int direction) {
    int total_movement = 0, pos = 0;
    int sorted[n];
    for (int i = 0; i < n; i++) 
        sorted[i] = requests[i];
    sorted[n++] = head;
    sorted[n++] = (direction == 1) ? disk_size - 1 : 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (sorted[i] > sorted[j]) {
                int temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (sorted[i] == head) {
            pos = i;
            break;
        }
    }
    printf("\nSCAN Disk Scheduling Steps:\n");
    if (direction == 1) {
        for (int i = pos; i < n; i++) {
            printf("Head moves from %d to %d, movement = %d\n", head, sorted[i], abs(sorted[i] - head));
            total_movement += abs(sorted[i] - head);
            head = sorted[i];
        }
        for (int i = pos - 1; i >= 0; i--) {
            printf("Head moves from %d to %d, movement = %d\n", head, sorted[i], abs(sorted[i] - head));
            total_movement += abs(sorted[i] - head);
            head = sorted[i];
        }
    } else {
        for (int i = pos; i >= 0; i--) {
            printf("Head moves from %d to %d, movement = %d\n", head, sorted[i], abs(sorted[i] - head));
            total_movement += abs(sorted[i] - head);
            head = sorted[i];
        }
        for (int i = pos + 1; i < n; i++) {
            printf("Head moves from %d to %d, movement = %d\n", head, sorted[i], abs(sorted[i] - head));
            total_movement += abs(sorted[i] - head);
            head = sorted[i];
        }
    }
    printf("Total Head Movement (SCAN): %d\n", total_movement);
}

void c_scan(int requests[], int n, int head, int disk_size) {
    int total_movement = 0, pos = -1;
    int sorted[n];
    for (int i = 0; i < n; i++) 
        sorted[i] = requests[i];
    sorted[n++] = head;
    sorted[n++] = 0;
    sorted[n++] = disk_size - 1;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (sorted[i] > sorted[j]) {
                int temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (sorted[i] == head) {
            pos = i;
            break;
        }
    }
    
    printf("\nC-SCAN Disk Scheduling Steps:\n");
    for (int i = pos; i < n; i++) {
        int movement = abs(sorted[i] - head);
        if (movement > 0) {
            printf("Head moves from %d to %d, movement = %d\n", head, sorted[i], movement);
            total_movement += movement;
            head = sorted[i];
        }
    }

    int jump_movement = abs(sorted[0] - 0);
    printf("Head moves from %d to %d (wrap-around), movement = %d\n", head, 0, abs(0-head));
    total_movement += abs(0-head);
    head = 0;

    for (int i = 0; i < pos; i++) {
        int movement = abs(sorted[i] - head);
        if (movement > 0) { 
            printf("Head moves from %d to %d, movement = %d\n", head, sorted[i], movement);
            total_movement += movement;
            head = sorted[i];
        }
    }

    printf("Total Head Movement (C-SCAN): %d\n", total_movement);
}

void sstf(int requests[], int n, int head) {
    int total_movement = 0;
    int visited[n];
    for (int i = 0; i < n; i++) 
        visited[i] = 0;

    printf("\nSSTF Disk Scheduling Steps:\n");
    for (int i = 0; i < n; i++) {
        int min_seek = 1e9, min_index = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && abs(requests[j] - head) < min_seek) {
                min_seek = abs(requests[j] - head);
                min_index = j;
            }
        }
        visited[min_index] = 1;
        printf("Head moves from %d to %d, movement = %d\n", head, requests[min_index], min_seek);
        total_movement += min_seek;
        head = requests[min_index];
    }
    printf("Total Head Movement (SSTF): %d\n", total_movement);
}

int main() {
    int n, head, disk_size, direction;
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);
    int requests[n];
    printf("Enter the requests: ");
    for (int i = 0; i < n; i++) 
        scanf("%d", &requests[i]);
    printf("Enter the initial head position: ");
    scanf("%d", &head);
    printf("Enter the disk size: ");
    scanf("%d", &disk_size);
    printf("Enter the direction (1 for up, 0 for down): ");
    scanf("%d", &direction);
    fcfs(requests, n, head);
    scan(requests, n, head, disk_size, direction);
    c_scan(requests, n, head, disk_size);
    sstf(requests, n, head);
    return 0;
}
