#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>   // For date and time
#define MAX_SEATS 5
#define MAX_QUEUE 5
#define MAX_STATIONS 10

// Structure for passenger
typedef struct {
    int id;
    char name[50];
    char source[30];
    char destination[30];
} Passenger;

// Arrays for seats and waiting list
Passenger seats[MAX_SEATS];
Passenger waitingQueue[MAX_QUEUE];
int seatCount = 0;
int queueFront = 0, queueRear = -1;

// Graph for shortest route
int graph[MAX_STATIONS][MAX_STATIONS];
char stations[MAX_STATIONS][30];
int stationCount = 0;

// Function prototypes
void bookTicket();
void cancelTicket();
void displayStatus();
void enqueueWaiting(Passenger p);
Passenger dequeueWaiting();
void saveToFile();
void loadFromFile();
void shortestRoute();  // new module
int minDistance(int dist[], int visited[]);

// --------------------------- MAIN ------------------------------
int main() {
    int choice;
    loadFromFile();  // Load previous bookings

    // Predefined stations and graph for demo
    stationCount = 5;
    strcpy(stations[0], "Chennai");
    strcpy(stations[1], "Bangalore");
    strcpy(stations[2], "Hyderabad");
    strcpy(stations[3], "Mumbai");
    strcpy(stations[4], "Delhi");

    int sampleGraph[5][5] = {
        {0, 350, 520, 0, 0},
        {350, 0, 500, 980, 0},
        {520, 500, 0, 700, 1100},
        {0, 980, 700, 0, 1200},
        {0, 0, 1100, 1200, 0}
    };
    memcpy(graph, sampleGraph, sizeof(sampleGraph));

    while (1) {
        printf("\n=== Train Reservation System ===\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. Display Status\n");
        printf("4. Find Shortest Route\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: bookTicket(); break;
            case 2: cancelTicket(); break;
            case 3: displayStatus(); break;
            case 4: shortestRoute(); break;
            case 5:
                printf("Exiting system...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// --------------------------- BOOKING MODULE ------------------------------

void bookTicket() {
    Passenger p;
    int n,i;
    printf("Enter the no of passengers:");
    scanf("%d",&n);
    for(i=0;i<n;i++){
    	printf("\nEnter Passenger ID: ");
    scanf("%d", &p.id);
    getchar();
    printf("Enter Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = 0;
    printf("Enter Source: ");
    fgets(p.source, sizeof(p.source), stdin);
    p.source[strcspn(p.source, "\n")] = 0;
    printf("Enter Destination: ");
    fgets(p.destination, sizeof(p.destination), stdin);
    p.destination[strcspn(p.destination, "\n")] = 0;

    if (seatCount < MAX_SEATS) {
        seats[seatCount++] = p;
        printf("Booking Confirmed! Seat Number: %d\n", seatCount);
        saveToFile();
    } else if (queueRear < MAX_QUEUE - 1) {
        enqueueWaiting(p);
        printf("All seats full! Added to waiting list. Position: %d\n", queueRear - queueFront + 1);
    } else {
        printf("All seats and waiting list full! Try later.\n");
    }
	}
}

// --------------------------- CANCELLATION MODULE ------------------------------

void cancelTicket() {
    if (seatCount == 0) {
        printf("No bookings to cancel.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Passenger ID to cancel: ");
    scanf("%d", &id);
	int i,j;
    for (i = 0; i < seatCount; i++) {
        if (seats[i].id == id) {
            found = 1;
            printf("Booking of %s cancelled.\n", seats[i].name);
            for (j = i; j < seatCount - 1; j++) {
                seats[j] = seats[j + 1];
            }
            seatCount--;

            if (queueRear >= queueFront) {
                Passenger p = dequeueWaiting();
                seats[seatCount++] = p;
                printf("Seat reassigned to waiting passenger: %s\n", p.name);
            }
            saveToFile();
            break;
        }
    }

    if (!found) printf("Passenger ID not found.\n");
}

// --------------------------- DISPLAY MODULE (Alternative Clean Format) ------------------------------

void displayStatus() {
    printf("\n\n========================================================\n");
    printf("                  TRAIN RESERVATION STATUS              \n");
    printf("========================================================\n");

    // Display current date and time
    time_t now;
    time(&now);
    printf("Last Updated: %s", ctime(&now));

    // Sort booked passengers by seat ID (simple Bubble Sort)
    // ... (sorting code remains the same) ...
    int i, j;
    for (i = 0; i < seatCount - 1; i++) {
        for (j = 0; j < seatCount - i - 1; j++) {
            if (seats[j].id > seats[j + 1].id) {
                Passenger temp = seats[j];
                seats[j] = seats[j + 1];
                seats[j + 1] = temp;
            }
        }
    }

	printf("\n--------------------------------------------------------");
    printf("\n                Confirmed Bookings                    \n");
    printf("--------------------------------------------------------\n");
    // Print header with spacing
    printf("%-6s %-4s %-20s %-12s %-12s\n", "SEAT", "ID", "NAME", "FROM", "TO");
    printf("------ ---- -------------------- ------------ ----------\n"); // Separator line

    if (seatCount == 0) {
        printf("No seats booked.\n");
    } else {
        for (i = 0; i < seatCount; i++) {
            // Print data with spacing
            printf("%-6d %-4d %-20s %-12s %-12s\n",
                   i + 1, seats[i].id, seats[i].name, seats[i].source, seats[i].destination);
        }
    }
	printf("\n--------------------------------------------------------");
    printf("\n                   Waiting List                        \n");
    printf("--------------------------------------------------------\n");
    // Print header with spacing
    printf("%-8s %-4s %-20s %-12s %-12s\n", "POSITION", "ID", "NAME", "FROM", "TO");
    printf("-------- ---- -------------------- ------------ --------\n"); // Separator line

    if (queueRear < queueFront) {
        printf("Waiting list empty.\n");
    } else {
        for (i = queueFront; i <= queueRear; i++) {
            // Print data with spacing
            printf("%-8d %-4d %-20s %-12s %-12s\n",
                   i - queueFront + 1, waitingQueue[i].id, waitingQueue[i].name,  
                   waitingQueue[i].source, waitingQueue[i].destination);
        }
    }

    // Display summary totals
    printf("\n========================================================\n");
    printf("Summary:\n");
    printf("Total Confirmed Bookings: %d\n", seatCount);
    printf("Passengers in Waiting List: %d\n", (queueRear >= queueFront) ? (queueRear - queueFront + 1) : 0);
    printf("Available Seats: %d\n", MAX_SEATS - seatCount);
    printf("========================================================\n\n");
}


// --------------------------- QUEUE FUNCTIONS ------------------------------

void enqueueWaiting(Passenger p) {
    if (queueRear < MAX_QUEUE - 1) {
        waitingQueue[++queueRear] = p;
    }
}

Passenger dequeueWaiting() {
    Passenger p = waitingQueue[queueFront++];
    return p;
}

// --------------------------- FILE HANDLING ------------------------------

void saveToFile() {
    FILE *fp = fopen("bookings.txt", "w");
    int i;
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    for (i = 0; i < seatCount; i++) {
        fprintf(fp, "%d,%s,%s,%s\n", seats[i].id, seats[i].name,
                seats[i].source, seats[i].destination);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("bookings.txt", "r");
    if (fp == NULL) return;

    seatCount = 0;
    while (fscanf(fp, "%d,%49[^,],%29[^,],%29[^\n]\n",
                  &seats[seatCount].id, seats[seatCount].name,
                  seats[seatCount].source, seats[seatCount].destination) == 4) {
        seatCount++;
        if (seatCount >= MAX_SEATS) break;
    }
    fclose(fp);
}

// --------------------------- SHORTEST ROUTE MODULE ------------------------------

void shortestRoute() {
    int src, dest;
    printf("\nAvailable Stations:\n");
    int i;
    for (i = 1; i < stationCount; i++)
        printf("%d. %s\n", i, stations[i]);

    printf("Enter Source Station Number: ");
    scanf("%d", &src);
    printf("Enter Destination Station Number: ");
    scanf("%d", &dest);
	int count,v;
    int dist[MAX_STATIONS];
    int visited[MAX_STATIONS] = {0};
    int prev[MAX_STATIONS];
  
    for (i = 0; i < stationCount; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (count = 0; count < stationCount - 1; count++) {
        int u = minDistance(dist, visited);
        visited[u] = 1;

        for (v = 0; v < stationCount; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }

    printf("\nShortest Distance from %s to %s = %d km\n",
           stations[src], stations[dest], dist[dest]);

    printf("Path: ");
    int path[MAX_STATIONS], pathCount = 0;
    for (v = dest; v != -1; v = prev[v])
        path[pathCount++] = v;

    for (i = pathCount - 1; i >= 0; i--) {
        printf("%s", stations[path[i]]);
        if (i != 0) printf(" -> ");
    }
    printf("\n");
}

// --------------------------- HELPER FUNCTION ------------------------------

int minDistance(int dist[], int visited[]) {
    int min = INT_MAX, min_index = -1;
    int v;
    for (v = 0; v < stationCount; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

