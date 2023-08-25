# WSP_HPC
The wandering salesman problem involves visiting cities once without returning. Solve using permutations and branch and bound. For parallelism, MPI (Message Passing Interface) suits, managing message passing topology for efficient computation.


1. Introduction
The wandering salesman problem is a classic optimization problem in which a salesperson must visit each city exactly once but is not expected to reach back to the source . The approach is to find all the possible permutations set of vertices and find the smallest by dividing the problem in to smaller problems and solve them simultaneously on different nodes. In order to partition the total set of feasible solutions into smaller subsets of solutions branch and bound technique is approached. Branch and bound is an algorithm design paradigm which is used in solving combinatorial optimizations problems i.e., typically exponential in terms of the time complexity . For parallelization of algorithm MPI (Message passing Interface) is going to be used as paradigms as it provide a high level of abstraction on top of the computing resources in terms of the message passing topology.
1.1 Aim
The major aim of our project is to develop and implement different distributed parallel schemes of Branch and bound algorithm using message-passing-interface(MPI) for multi node parallelization by using the respective approach that will generate all possible routes between cities and use a lower bound on the distance of each route to shear branches that cannot possibly contain the optimal solution . Each node is responsible for solving a subproblem in order to combine the solution to find the overall optimal solution .
1.2 Objective
The goal of branch and bound algorithm [1] is to recursively split the search space in to smaller spaces and minimize on this smaller spaces. Branching alone would amount to brute-force
WANDERING SALESMAN PROBLEM
4
enumeration. As it is contemplated under NP-hard optimization problem and product efficient implementation using high performance computing, parallel branch and bound will be used to evaluate approximate original problem using semidefinite relaxations.
The following objectives are:
• To device a serial algorithm followed by devising a way to parallelise it.
• Analyse our parallel algorithm and implement with a respective programming language.
• Comparing the solution obtained by parallel algorithm with the serial one.
• Measuring the performance
• Measuring the cost of communication and the cost of computation for our parallel solution.
2. Literature review
The objective is to minimize the distance, with n cities and n! different permutation exploring all paths. There are multiple approaches to solve. The respective approaches are as follows:
• Brute force: This approach involves in generating all possible routes between cities and check the distance of each by calculating the total number of route and listing all the possible routes to find the shortest one [2].
• Dynamic Programming: This approach involves breaking down the problem into smaller subproblems and solving them individually, then combining the solutions to find the overall optimal solution. This approach can be implemented using a recursive function or a bottom-up dynamic programming algorithm.
• Genetic Algorithm: This approach involves simulating the process of natural selection to find the best solution. A population of possible solutions is generated and evolved over multiple generations. This approach can be implemented using a standard genetic algorithm library or by writing the code from scratch.
• Dijkstra Algorithm: This approach is used to determine the shortest possible path based on the smallest weight from one node to another using a complete graph where each node is connected to all other nodes [3].
3. Methodology
The term branch and bound refer to all state-space searching methods in which all the children node are generated before any other live node i.e., the state space tree expansion. As a set of cities and the distance between every pair of cities , the problem is to get the shortest possible route.
    
WANDERING SALESMAN PROBLEM
5
While generating the node , for every node we generate the costs. If any node we are getting the costs is greater than some upper bound , we will generate the node which is fruitful instead of relying on all the nodes by following the path which is taking nearer to the minimization , maximization or optimization of node.
We prepare the tree starting from the root and expand it .
For example, considering distance: 54
76 30
24 51 64
After evaluation of matrix , C0 C1 C2 C3
 C0 0 C1 54 C2 76 C3 24
54    76    24
0     30    51
30    0     64
51    64    0
Fig1: Evaluation of matrix

WANDERING SALESMAN PROBLEM
6
Following, is the state space tree exploring the entire problem for the above WSP problem , which shows the optimal solution
Fig2: State space tree
As we see from the above diagram every node has a cost associated with it , when we travel from city ‘i’ to city ‘j’ , the cost of node j will be the sum of the parent node ‘i’ , the cost of an edge (i , j) and the lower bound of the path starting from ‘j’. Since the root node is the first node to be expanded the cost will be only the lower bound path starting at the root.
We try to calculate the lower bound of the path , using the resulting cost matrix i.e., at level 4 , we get the minimized and maximized cost.
Devising Serial Algorithm
In serial algorithm , we introduce a function that tries to visit all the city and adds costs of going to the next city and compares the current costs and lower bound of the path by comparing with the best costs found so far. The function if recursive calls itself with the next city as the current vertex , while the array keeps the track of which of the cities visited so far and calculates the lower bound of the current path and the main function calculates the execution time of the respective algorithm to find the best path and costs.
  
WANDERING SALESMAN PROBLEM
7
 Fig3: Pseudocode on devising Serial Algorithm
The function starts from the city and visits to the next city , the current solution along with the best solution is stored in an array as current_path and b_path . The visited array is used to keep a track of the cities that already has been visited.The function helps in getting the best solution and displays the best optimal path and costs.A lower bound function is used in the whole search tree to check the best path. It calculates the lower bound of the current path by visiting to the nearest unvisited vertex starting from the current vertex and consecutively add costs of going to that vertex.

WANDERING SALESMAN PROBLEM
8
The algorithm relies on if lb and the cost of the current path is less than the best solution , the algorithm will continue the path i.e., reducing the effect on number of paths to be explored and overall execution time. It will measure the execution time of the program by using the clock() function by returning the number of clock cycles as it will determine the starting and the ending point time
Devising Parallel Algorithm
Using MPI , we define the data structure in order to represent the problem statement and lower bounding function to estimate the cost of the remaining path followed by implementing a parallel branching strategy tending calculation to multi process by determining each city that each process is responsible for processing
The main function performs the following steps:
• Reading a distance matrix of the cities
• Broadcasting the data to other MPI processors
• Function uses MOI to divide the cities among different processors
• Calls the WSP function which is a recursive function to calculate the shortest possible path
WSP function performs the following steps:
• Calculate the total distance of the current path and checks if less then current best bound
• If less , it updates the best bound and copies the current path to the best path and continues by
visiting all the unvisited cities and recursively calling itself with in the next city .
    Fig4: Pseudocode on devising Parallel Algorithm

WANDERING SALESMAN PROBLEM
9
Implementing a parallel branching strategy where each MPI process is responsible for exploring a different branch of the search tree. MPI functions such as MPI_Send and MPI_Recv is used to communicate the state of the search between processes and to share the best path. A termination is required to stop the search when the optimal solution is found.
4. Analysis
Analysing the time complexity of WSP
As branch and bound algorithm requires a search tree to be generated it involves gathering of all the possible subsets of cities which has a total 2^n subsets and for each subset it perform O(n^2) to get the lower bound of the respective solution. Hence the time complexity is O(n^2 2^n) [4]
Analysing the cost of communication
The cost of communication is depending on a specific implementation w.r.t, to the size of the problem. As the algorithm divides the problem in to subproblems and communicate the solution and the bounds to find the optimal solution , it is relying on the time and the resources required to send and receive information between subproblems hence can become more significant and improve the performance by increasing the parallelism . In serial algorithm , the processing time required to combine and update the bounds are taking more time hence parallel computation helps in reducing the execution time by distributing the computation across multiple processors [5], [6].
Analysing the cost of computation
The computational costs is determined using the input data as the code implements a branch and bound algorithm it is a NP-hard problem resulting the computational costs quite high specifically for large inputs. The cost precisely depends on the distribution of work among the processors and parallel optimization . The cities are divided among the different processor with the procedure if the number of cities is not divisible by the number of processes then the remaining cities are assigned to the first process. The starting and the ending city is stored in the respective variables [7].
The total number of possible path depends upon number of cities
For distance 17, we get, the best path as 0-9-3-13-14-8-5-10-12-4-7-2-15-1-16-6-11 and best cost as 278. If the processor would have chosen an alternate path the least cost could not be found effecting the execution time . As the number of processor increases , it distributes the data among them and start finding the optimum path resulting in better computational cost and less execution of time.
   
WANDERING SALESMAN PROBLEM
10
Load Balancing and potential synchronization
The algorithm balances the load of the solution among multiple tasks in order to reduce the overall execution time as the data is broadcast to all the processors and each processor takes a portion of the cities as its starting pint which is determined by the bock size variable. At the end, each processor send its best path back and compare the received solution and update the best one .This approach helps in load balancing as each processor has a portion of the cities as its starting points. Adding more processor creates a noticeable amount of synchronization. The barrier is used after MPI_Bcast to broadcast number of cities n, the distance matrix dist and the current lower bound to avoid any race conditions and overlapping between processors and to ensure that each process can start its calculations.
Fig5: Load Balancing
Here MPI_Barrier is used to synchronize the program so that portions of the parallel code can be timed accurately.
5. Result
Serial : Time Execution
The serial program reads the distance matrix of cities from respective file and calculates the lower bound of the current path. As it works sequentially , it performs single task at a time
Configuring the time execution of Serial algorithm by following the respective commands in crescent
• ssh -X s387796@crescent.central.cranfield.ac.uk
- establishing a secure shell (SSH) connection to a remote server allowing GUI application to run on the remote server and displaying on the local machine.
    
WANDERING SALESMAN PROBLEM
11
• nano sequentialwsp.c
-To write , edit and open the file named “sequentialwsp.c” to be edited
• gcc sequentialwsp.c
-Compiling the serial code in the file by using the GNU Compiler Collection (GCC) for integrated
distribution of compilers.
• ./a.out
- Running the executable file and getting the distance matrix with execution time.
Execution of distance 17:
   Best Cost: 278 , Execution time: 228.96 s
Fig6: Serial Execution

WANDERING SALESMAN PROBLEM Similarly , Execution with w.r.t, other distance
12
    Distance
Dist16 Dist14 Dist12
Parallel-Time Execution
Best Cost
255 249 239
Execution time
42.33 seconds 2.54 seconds 0.16 seconds
           Table 1: Serial Execution time
 The number of cities after getting divided by the number of process in order to determine the average block size of cities that each processor will handle, there is a variable that represent the number of cities which cannot be divided among the processes. The block size is then set to the ceiling value and divided. The parallel execution to the simultaneous execution of multiple tasks reduces the time here as it takes to complete the task by dividing the work between multiple processor.
Configuring the time execution of Parallel algorithm by following the respective commands in crescent
• Module load intel
-To modify the environment to set the path and other variables
• nano parallelalgo.c
-To write , edit and open the file named “parallelalgo.c” to be edited
• mpicc parallelalgo.c
-To compile the source file by providing special libraries that are needed to compile
• mpirun -np number of processor ./a.out distname
The shell script is used to determine the kind of machine it is running on and starts the required number of jobs on that machine.

WANDERING SALESMAN PROBLEM Execution time of Dist17 with processors
Processors
      2
      3
      4
      5
      6
      7
      8
      9
      10
      11
      12
      13
      14
      15
      16
13
  Execution Time
207.122 166.348 164.22 159.78 146.69 151.227 150.155 137.68 137.61 143.11 142.73 142.615 142.731 133.80 133.50
                                Best cost:278 , Best Path: 0-9-3-13-14-8-5-10-12-4-7-2-15-1-16-6-11
Table 2: Parallel Execution time
Scalability
The measure of capacity to effectively utilize an increasing number of processors depends upon of parallel speed-up and parallel efficiency.
• Parallel speed-up: The speed-up of the parallel code is compared to its sequential code as the ratio of the execution time of the serial code to the execution time of the respective parallel code [8].
S = !" [s = serial ; p = parallel] !#
• Parallel efficiency: To estimate the better execution of a parallel calculation work , it is determined with parallel efficiency which is the ratio of speed-up factor S(n) and the number of processors[8].
𝐸 = $(&)[ s = speed-up , p- number of processors] #
   
WANDERING SALESMAN PROBLEM
14
 Fig7: Speed-up Graph
 Fig8: Efficiency graph

WANDERING SALESMAN PROBLEM
15
Measuring the performance
  Fig10: Scalability
In computer architecture , Amdahl’s law [9] gives the theoretical speedup in latency of the execution of a task at fixed workload of a system whose resources are improved. However , in the above graph it can be observed that it does not reflect on how it will behave when runs as part of the full program. As we speed-up one part of the program it leads to more cache hence underlying performance model , it is extremely complex . On basis of practical implementation , S=N it is difficult to get as it depend on the parallel implementation as it is more than the one of the sequential program and also speed-up is generally limited by speed of the processor.
   Processors
Speed-up
Efficiency
   2 1.10 0.55
3 1.37 0.45
4 1.39 0.38
5 1.43 0.28
6 1.55 0.25
7 1.50 0.21
8 1.52 0.19
9 1.66 0.18
10 1.66 0.16
11 1.59 0.14
                             
WANDERING SALESMAN PROBLEM
16
   12 1.60 0.13
13 1.60 0.12
14 1.60 0.11
15 1.71 0.11
16 1.71 0.10
Table 3: Speed-up and efficiency
It can be seen that with 8 cores the respective simulation almost achieves its maximum speed up , with 16 , it is more higher. On the other hand, parallel efficiency is decreasing significantly. It is noticed that , with more even larger number of cores the speed-up decreases again due to the increase proportion of communication that has to be maintained in order to count the cores at the same time The optimum we are getting here is between 8 to 16 cores.
Measuring the cost of communication and the cost of computations for the parallel solution
                   Processors
Cost of Communication
Cost of computation
207.196 166.265 146.699 151.150 139.384 142.714 142.6938 133.4466
   2 0.001410 4 0.000348 6 0.001159 8 0.000387
10 0.000367 12 0.000366 14 0.000384 16 0.000371
                       Table 4: Cost of communication and computation
There are several types of communication mode in HPC , such as :
• Unicast
• Multicast
• Broadcast
• Gather
• Reduce

WANDERING SALESMAN PROBLEM
17
The communication pattern that is used here is broadcast and it is affected by broadcast as the entire distance matrix is broadcasted from the root process to all other process using MPI_Bcast. This broadcast operation collects a costs in terms of time and resources, as the data is transmitted from one process to all others. The cost is depending upon the size of the distance matrix and the number of process in the MPI environment. If the size of the distance matrix Is large and the number of process is high , the communication cost could be significant.
6. Conclusion
In this paper , we presented a parallel implementation of solution for Wandering Salesman Problem . To obtain the shortest path and best cost different methods of computation has been performed w.r.t, the functions of MPI, as the functionalities offered by MPI have made it possible to convert the WSP sequential algorithm in to parallel. The major challenge that came across with the input data was to distribute the cities in to processor to get the less execution time. In present , the best reducing time is attained in 16 cores , where it achieved the maximum speed up. In between there has been a slight increase as speed up is generally limited by the speed of the processor. Some problem came across while using the other type of communication mode in HPC . In the future work the more investigation shall be on other types of communication in order to focus on speed up in latency of the execution of a task at fixed workload of a system.
