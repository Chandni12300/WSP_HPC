#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CITIES 100

int n; // number of cities
int dist[MAX_CITIES][MAX_CITIES]; // distance matrix
int curr_path[MAX_CITIES]; // current path
int visited[MAX_CITIES]; // visited cities
int curr_bound = 0; // lower bound of current path
int best_cost = INT_MAX; // best solution found so far
int best_path[MAX_CITIES]; // best path found so far

// Function to calculate the lower bound of the current path
int calculateBound(int curr_vertex) {
    int curr_bound = 0;
    //memset(visited, 0, sizeof(visited));
    // Mark the current vertex as visited
    visited[curr_vertex] = 1;
    // Start from the current vertex and go to the nearest unvisited vertex
    for (int i = 0; i < n - 1; i++) {
        int v = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (v == -1 || dist[curr_vertex][j] < dist[curr_vertex][v])) {
                v = j;
            }
        }
        curr_vertex = v;
        curr_bound += dist[curr_vertex][v];
        //visited[v] = 1;
    }
    return curr_bound;
}

// Function to solve the WSP using branch and bound
void WSP(int curr_vertex, int curr_length, int curr_cost) {
    // If all cities have been visited
    if (curr_length == n) {
        // Update the best solution if the current solution is better
        if (curr_cost < best_cost) {
            best_cost = curr_cost;
            memcpy(best_path, curr_path, sizeof(curr_path));
        }
    } else {
        // Try every unvisited city
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                // Add the cost of going to the next city
                int temp_cost = curr_cost + dist[curr_path[curr_length - 1]][i];
                // Check if the current path is promising
                if (temp_cost  < best_cost) {
                    curr_path[curr_length] = i;
                    visited[i] = 1;
                    WSP(i, curr_length + 1, temp_cost);
                    visited[i] = 0;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // Read the distance matrix from a file
   /* Open the input file */
    FILE *file = fopen("dist4", "r");

    /* Read the number of nodes */
    fscanf(file, "%d", &n);

    /* Read the lower triangular matrix */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
        if(i==j){

}
    else{


            fscanf(file, "%d", &dist[i][j]);
        }
        }
    }

    /* Replicate the upper triangular matrix */
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
    if(i==j){}
    else{
            dist[i][j] = dist[j][i];
        }
    }
    }

    /* Set diagonal elements to 0 */
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
    }

    /* Print the distance matrix */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", dist[i][j]);
        }
        printf("\n");
    }

    /* Close the input file */
    fclose(file);

    // Start from the first city
    curr_path[0] = 0;
    visited[0] = 1;
    // Calculate the lower bound of the path
    curr_bound = calculateBound(0);
    // Solve the WSP using branch and bound
    WSP(0, 1, 0);

    // Print the best solution found
    printf("Best path: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", best_path[i]);
    }
    printf("\nCost: %d\n", best_cost);

    return 0;
}