#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <mpi.h>
#include <math.h>
#define MAX_CITIES 100

int n; // number of cities
int initial_city; // starting city for calculating best cost
int dist[MAX_CITIES][MAX_CITIES]; // distance matrix
int curr_path[MAX_CITIES]; // current path
int visited[MAX_CITIES]; // visited cities
int curr_bound = 0; // lower bound of current path
int best_cost = INT_MAX; // best solution found so far
int best_path[MAX_CITIES]; // best path found so far
int num_procs, rank; // Function to calculate the lower bound of the current path
int cities_per_proc,start_city,end_city;

int calculateBound(int curr_vertex) {
    int curr_bound = 0;
    memset(visited, 0, sizeof(visited));
    // Mark the current vertex as visited
    visited[curr_vertex] = 1;
    // Start from the current vertex and go to the nearest unvisited vertex
    if (rank==0)
    {
    for (int i = 0; i < n - 1; i++)
    {
        int v = -1;
        for (int j = 0; j < n; j++)
        {
            if (!visited[j] && (v == -1 || dist[curr_vertex][j] < dist[curr_vertex][v]))
            {
                v = j;
            }
        }
        curr_vertex = v;
        curr_bound += dist[curr_vertex][v];
        // visited[curr_vertex] = 1;
    }
    }
    // visited[curr_vertex] = 1;
    return curr_bound;
}

// Function to solve the WSP using branch and bound
void WSP(int curr_vertex, int curr_length, int curr_cost) {
    // If all cities have been visited
    if (curr_length == n)
    {
        // Update the best solution if the current solution is better
        if (curr_cost < best_cost)
        {

            MPI_Allreduce(&curr_cost, &best_cost, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
            if (rank == 0)
            {
                memcpy(best_path, curr_path, sizeof(curr_path));
            }
        }
    }
    else
    {
        // Try every unvisited city
        for (int i = 0; i < n; i++)
        {
            if (!visited[i])
            {
                // Add the cost of going to the next city
                int temp_cost = curr_cost + dist[curr_path[curr_length - 1]][i];
                // Check if the current path is promising
                if (temp_cost + curr_bound < best_cost)
                {
                    curr_path[curr_length] = i;
                    visited[i] = 1;
                    WSP(i, curr_length + 1, temp_cost);
                    visited[i] = 0;
                }
            }
        }
    }
}

int get_subpaths(int rank, int num_procs) {
    int remaining_cities = n % num_procs;
    if (n >= num_procs)
    {
        cities_per_proc = n / num_procs;
        start_city = rank * cities_per_proc;
        end_city = start_city + cities_per_proc - 1;
        if(rank<remaining_cities)
        {
            start_city +=rank;
            end_city = start_city + cities_per_proc;
        }
        else
        {
        start_city += remaining_cities;
        end_city = start_city + cities_per_proc - 1;
        }
    }
    else
    {
        if(rank<num_procs)
        {
            cities_per_proc = 1;
            start_city = rank;
            end_city = start_city;
        }
        else
        {
            cities_per_proc = 0;
        }
    }
    return start_city, end_city;
}

int main(int argc, char* argv[]) { MPI_Init(&argc , &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time,end_time,t3,t4, max_time;

    // Read input for the number of cities and distances
    char file_name[100];
    if (rank == 0)
    {
        printf("Enter the filename : \t");
        scanf("%s", file_name);
        if (access(file_name, F_OK) != -1)
        {
        printf("File %s exist\n", file_name);
        }
        else
        {
        printf("File %s does not exist\n. Please enter a valid filename!!! : \t", file_name);
        scanf("%s",file_name);
        }
        FILE* file = fopen(file_name, "r");

        /* Read the number of nodes */
        fscanf(file, "%d", &n);

        /* Read the lower triangular matrix */
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                if (i == j)
                {
                }
                else
                {
                    fscanf(file, "%d", &dist[i][j]);
                }
            }
        }

        /* Replicate the upper triangular matrix */
         for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (i == j)
                {
                }
                else
                {
                    dist[i][j] = dist[j][i];
                }
            }
        }

        /* Set diagonal elements to 0 */
        for (int i = 0; i < n; i++)
        {
            dist[i][i] = 0;
        }

        /* Print the distance matrix */
        printf("The number of cities : %d\t", n);
        printf("The Entered Distance Matrix is: \n");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d ", dist[i][j]);
            }
            printf("\n");
        }

        /* Close the input file */
        fclose(file);

        // Start from the first city
        printf("Enter the initial city (0 to %d): \t",n-1);
        scanf("%d", &initial_city);
        if(initial_city < 0 || initial_city >= n)
        {
        printf("Invalid city. Please enter a valid city(0 to %d) only: \t",n-1);
        scanf("%d", &initial_city);
        }
    }
    start_time = MPI_Wtime();
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(dist, MAX_CITIES * MAX_CITIES, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&initial_city, 1, MPI_INT, 0, MPI_COMM_WORLD);
    curr_path[0] = initial_city;
    visited[initial_city] = 1;
    MPI_Bcast(curr_path, MAX_CITIES, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(visited, MAX_CITIES, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the lower bound of the path
    curr_bound = calculateBound(initial_city);
    MPI_Bcast(&curr_bound, MAX_CITIES, MPI_INT, 0, MPI_COMM_WORLD);

    // Solve the WSP using branch and bound
    t3 = MPI_Wtime();
    start_city, end_city = get_subpaths(rank, num_procs);
    for(int i=start_city; i<=end_city; i++){
    WSP(i, 1, 0);
    }
    t4 = MPI_Wtime();
    double time_taken1 = t4-t3;
    end_time = MPI_Wtime();
    double time_taken = end_time - start_time;
    // Print the best solution found
    if (rank == 0)
    {
        printf("Best path: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", best_path[i]);
        }
        printf("\nCost: %d\n", best_cost);
        printf("Time taken : %f seconds\n", time_taken);
    }
    MPI_Finalize();
    return 0;
}