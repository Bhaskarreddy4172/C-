#include <iostream>
#include <vector>
using namespace std;

const int P = 5; // Number of processes
const int R = 3; // Number of resource types

// Function to calculate the need matrix
void calculateNeed(int need[P][R], int max[P][R], int alloc[P][R]) {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

// Function to check if the system is in a safe state
bool isSafe(int avail[R], int max[P][R], int alloc[P][R]) {
    int need[P][R];
    calculateNeed(need, max, alloc);
    
    bool finish[P] = {false};
    int safeSeq[P];
    int work[R];
    for (int i = 0; i < R; i++) {
        work[i] = avail[i];
    }
    
    int count = 0;
    while (count < P) {
        bool found = false;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (need[i][j] > work[j])
                        break;
                }
                if (j == R) {
                    for (int k = 0; k < R; k++) {
                        work[k] += alloc[i][k];
                    }
                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            cout << "System is not in a safe state!" << endl;
            return false;
        }
    }
    cout << "System is in a safe state. Safe sequence: ";
    for (int i = 0; i < P; i++) {
        cout << "P" << safeSeq[i] << " ";
    }
    cout << endl;
    return true;
}

// Function to check if a request can be granted
bool resourceRequest(int avail[R], int max[P][R], int alloc[P][R], int process, int request[R]) {
    int need[P][R];
    calculateNeed(need, max, alloc);
    
    for (int i = 0; i < R; i++) {
        if (request[i] > need[process][i] || request[i] > avail[i]) {
            cout << "Request cannot be granted." << endl;
            return false;
        }
    }
    
    for (int i = 0; i < R; i++) {
        avail[i] -= request[i];
        alloc[process][i] += request[i];
        need[process][i] -= request[i];
    }
    
    if (isSafe(avail, max, alloc)) {
        cout << "Request granted." << endl;
        return true;
    } else {
        for (int i = 0; i < R; i++) {
            avail[i] += request[i];
            alloc[process][i] -= request[i];
            need[process][i] += request[i];
        }
        cout << "Request cannot be granted as it leads to an unsafe state." << endl;
        return false;
    }
}

int main() {
    int avail[R] = {3, 3, 2};
    int max[P][R] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    int alloc[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };
    
    isSafe(avail, max, alloc);
    
    int request[R] = {1, 0, 2};
    resourceRequest(avail, max, alloc, 1, request);
    
    return 0;
}
