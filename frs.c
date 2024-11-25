#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_CITIES 100

struct node {
    char city[50];
    int dist;
    int cost;
    int journey_time;
    struct node *next;
};

struct CityNode {
    char city[50];
    struct node *head;
};

struct CityNode* graph[MAX_CITIES];
int city_count = 0;

// Function to create a new city
void addCity(char city[]) {
	int i;
    for (i = 0; i < city_count; i++) 
    {
        if (strcmp(graph[i]->city, city) == 0) 
        {
            printf("City %s already exists.\n", city);
            return;
        }
    }
    struct CityNode* newCity = (struct CityNode*)malloc(sizeof(struct CityNode));
    strcpy(newCity->city, city);
    newCity->head = NULL;
    graph[city_count] = newCity;
    city_count++;
    char ans, City[50];
    printf("\nDo you want to insert another city? (Y/N)");
    scanf(" %c", &ans);
    if(ans=='Y')
    {
        printf("Enter city name: ");
        scanf(" %s", &City);
        addCity(City);
    }
    else if(ans=='N')
    {
        return;
    }
    else
    {
        printf("Invalid choice");
    }
}


// Function to get the index of a city by its name
int getCityIndex(char city[]) {
	int i;
    for (i = 0; i < city_count; i++) {
        if (strcmp(graph[i]->city, city) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to add a flight (edge) between two cities
void addFlight(int origin, char destination[], int distance, int cost, int journey_time) {
    struct node* newFlight = (struct node*)malloc(sizeof(struct node));
    strcpy(newFlight->city, destination);
    newFlight->dist = distance;
    newFlight->cost = cost;
    newFlight->journey_time = journey_time;
    newFlight->next = graph[origin]->head;
    graph[origin]->head = newFlight;
    printf("Flight added from %s to %s.\n", graph[origin]->city, destination);
    char dest[50], originCity[50], ans;
    int index, dist, Cost,journeyTime;
    printf("Do you want to insert another flight? (Y/N)");
    scanf(" %c", &ans);
    if(ans=='Y')
    {
        printf("Enter origin city ");
        scanf(" %s", &originCity);
        index = getCityIndex(originCity);
        printf("Enter destination city ");
        scanf(" %s", &dest);
        printf("Enter distance ");
        scanf(" %d", &dist);
        printf("Enter cost");
        scanf(" %d", &Cost);
        printf("Enter journey time ");
        scanf(" %d", &journeyTime);
        addFlight(index, dest, dist, Cost, journeyTime);
    }
}


// Find the vertex with the minimum distance value that hasn't been processed yet
int minDistance(int dist[], bool sptSet[], int V) {
    int min = INT_MAX, min_index;int v;
    for (v = 0; v < V; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// Function to print the path from source to destination
void printPath(int predecessor[], int j, struct CityNode* graph[]) {
    if (predecessor[j] == -1) {
        printf("%s ", graph[j]->city);
        return;
    }
    printPath(predecessor, predecessor[j], graph);
    printf("-> %s ", graph[j]->city);
}

// Function to print the solution (distance and path)
void printSolution_dist(int dist[], int dest, int predecessor[], struct CityNode* graph[], char* src, char* destCity) {
    printf("Shortest Distance from %s to %s: %d\n", src, destCity, dist[dest]);
    printf("Path: ");
    printPath(predecessor, dest, graph);
    printf("\n");
}
void printSolution_cost(int dist[], int dest, int predecessor[], struct CityNode* graph[], char* src, char* destCity) {
    printf("Path with minimum cost from %s to %s: %d\n", src, destCity, dist[dest]);
    printf("Path: ");
    printPath(predecessor, dest, graph);
    printf("\n");
}

// Dijkstra's algorithm for shortest paths based on cost or distance
void dijkstra(struct CityNode* graph[], char* srcCity, int V, char* destCity, int mode) {
    int dist[V];
    bool sptSet[V];
    int predecessor[V];int i;
    for (i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
        predecessor[i] = -1;
    }

    int src = getCityIndex(srcCity);
    int dest = getCityIndex(destCity);
    if (src == -1 || dest == -1) {
        printf("Source or Destination city not found.\n");
        return;
    }
    dist[src] = 0;
int count;
    for (count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = true;

        struct node* adjNode = graph[u]->head;
        while (adjNode != NULL) {
            int v = getCityIndex(adjNode->city);
            int weight = (mode == 1) ? adjNode->cost : adjNode->dist;
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                predecessor[v] = u;
            }
            adjNode = adjNode->next;
        }
    }
    if(mode == 1) {
    	printSolution_cost(dist, dest, predecessor, graph, srcCity, destCity);
	}
	else{
		printSolution_dist(dist, dest, predecessor, graph, srcCity, destCity);
	}
    
}
// Function to check if a flight exists between two cities
void check_flight(int origin, char destination[]) {
    struct node* temp = graph[origin]->head;
    while (temp != NULL) {
        if (strcmp(temp->city, destination) == 0) {
            printf("Flight exists from %s to %s with distance %d and cost %d.\n",
                   graph[origin]->city, destination, temp->dist, temp->cost);
            return;
        }
        temp = temp->next;
    }
    printf("No flight exists from %s to %s.\n", graph[origin]->city, destination);
}

// Function to update the flight details between two cities
void update_flight(int origin, char destination[], int newDist, int newCost, int newTime, int choice) {
    struct node* temp = graph[origin]->head;
    while (temp != NULL) {
        if (strcmp(temp->city, destination) == 0) {
            if (choice == 1 && newCost != -1) {
                temp->cost = newCost;
                printf("Cost updated successfully for flight from %s to %s.\n", graph[origin]->city, destination);
            } else if (choice == 2 && newDist != -1) {
                temp->dist = newDist;
                printf("Distance updated successfully for flight from %s to %s.\n", graph[origin]->city, destination);
            } else if (choice == 3 && newTime != -1) {
                temp->journey_time = newTime;
                printf("Time updated successfully for flight from %s to %s.\n", graph[origin]->city, destination);
            }
            return;
        }
        temp = temp->next;
    }
    printf("Flight to %s from %s not found. Cannot update.\n", destination, graph[origin]->city);
}

void userMenu() {
    char src[50], dest[50];
    int option;
do{
    printf("Find shortest route based on:\n");
    printf("1. Cost\n");
    printf("2. Distance\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &option);

    

    switch (option) {
        case 1:
            printf("Enter source city: ");
            scanf("%s", src);
            printf("Enter destination city: ");
            scanf("%s", dest);
            dijkstra(graph, src, city_count, dest, 1);  // Mode 1 for cost
            break;
        case 2:
            printf("Enter source city: ");
            scanf("%s", src);
            printf("Enter destination city: ");
            scanf("%s", dest);
            dijkstra(graph, src, city_count, dest, 2);  // Mode 2 for distance
            break;
        case 3:
                printf("Exiting User Menu.\n");
                break;
        default:
            printf("Invalid option. Please enter 1 or 2.\n");
            break;
    }
}while(option!=3);
}


// Menu for Administrator
void adminMenu() {
    int option, dist, cost, choice, journey_time;
    char originCity[50], destinationCity[50];

    do {
        printf("\nAdministrator Menu:\n");
        printf("1. Add a new city\n");
        printf("2. Check if a flight exists\n");
        printf("3. Update a flight\n");
        printf("4. Add a flight\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1: // Add a new city
                printf("Enter city name: ");
                scanf("%s", originCity);
                addCity(originCity);
                break;

            case 2: // Check if a flight exists
                printf("Enter origin city name: ");
                scanf("%s", originCity);
                int origin = getCityIndex(originCity);
                if (origin == -1) {
                    printf("City %s not found.\n", originCity);
                    continue;
                }
                printf("Enter destination city name: ");
                scanf("%s", destinationCity);
                check_flight(origin, destinationCity);
                break;

            case 3: // Update a flight
                printf("Enter origin city name: ");
                scanf("%s", originCity);
                origin = getCityIndex(originCity);
                if (origin == -1) {
                    printf("City %s not found.\n", originCity);
                    continue;
                }
                printf("Enter destination city name: ");
                scanf("%s", destinationCity);
                printf("Update:\n1. Cost\n2. Distance\n3. Time\nChoice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        printf("Enter new cost: ");
                        scanf("%d", &cost);
                        update_flight(origin, destinationCity, -1, cost, -1, 1);
                        break;
                    case 2:
                        printf("Enter new distance: ");
                        scanf("%d", &dist);
                        update_flight(origin, destinationCity, dist, -1, -1, 2);
                        break;
                    case 3:
                        printf("Enter new time: ");
                        scanf("%d", &journey_time);
                        update_flight(origin, destinationCity, -1, -1, journey_time, 3);
                        break;
                    default:
                        printf("Invalid choice. No update performed.\n");
                        break;
                }
                break;

            case 4: // Add a flight
                printf("Enter origin city name: ");
                scanf("%s", originCity);
                origin = getCityIndex(originCity);
                if (origin == -1) {
                    printf("City %s not found.\n", originCity);
                    continue;
                }
                printf("Enter destination city name: ");
                scanf("%s", destinationCity);
                printf("Enter distance: ");
                scanf("%d", &dist);
                printf("Enter cost: ");
                scanf("%d", &cost);
                printf("Enter duration of journey: ");
                scanf("%d", &journey_time);
                addFlight(origin, destinationCity, dist, cost, journey_time);
                break;

            case 5:
                printf("Exiting Administrator Menu.\n");
                break;

            default:
                printf("Invalid option.\n");
                break;
        }
    } while (option != 5);
}

int main() {
    printf("Welcome to Route Advisor System..\n");
    adminMenu();
    userMenu();
}



