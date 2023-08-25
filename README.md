# WSP_HPC
The wandering salesman problem involves visiting cities once without returning. Solve using permutations and branch and bound. For parallelism, MPI (Message Passing Interface) suits, managing message passing topology for efficient computation.


1. Introduction
The wandering salesman problem is a classic optimization problem in which a salesperson must visit each city exactly once but is not expected to reach back to the source . The approach is to find all the possible permutations set of vertices and find the smallest by dividing the problem in to smaller problems and solve them simultaneously on different nodes. In order to partition the total set of feasible solutions into smaller subsets of solutions branch and bound technique is approached. Branch and bound is an algorithm design paradigm which is used in solving combinatorial optimizations problems i.e., typically exponential in terms of the time complexity . For parallelization of algorithm MPI (Message passing Interface) is going to be used as paradigms as it provide a high level of abstraction on top of the computing resources in terms of the message passing topology.


1.1 Aim
The major aim of our project is to develop and implement different distributed parallel schemes of Branch and bound algorithm using message-passing-interface(MPI) for multi node parallelization by using the respective approach that will generate all possible routes between cities and use a lower bound on the distance of each route to shear branches that cannot possibly contain the optimal solution . Each node is responsible for solving a subproblem in order to combine the solution to find the overall optimal solution .


1.2 Objective
The goal of branch and bound algorithm [1] is to recursively split the search space in to smaller spaces and minimize on this smaller spaces. Branching alone would amount to brute-force enumeration. As it is contemplated under NP-hard optimization problem and product efficient implementation using high performance computing, parallel branch and bound will be used to evaluate approximate original problem using semidefinite relaxations.
The following objectives are:
• To device a serial algorithm followed by devising a way to parallelise it.
• Analyse our parallel algorithm and implement with a respective programming language.
• Comparing the solution obtained by parallel algorithm with the serial one.
• Measuring the performance
• Measuring the cost of communication and the cost of computation for our parallel solution.


3. Literature review
The objective is to minimize the distance, with n cities and n! different permutation exploring all paths. There are multiple approaches to solve. The respective approaches are as follows:
• Brute force: This approach involves in generating all possible routes between cities and check the distance of each by calculating the total number of route and listing all the possible routes to find the shortest one [2].
• Dynamic Programming: This approach involves breaking down the problem into smaller subproblems and solving them individually, then combining the solutions to find the overall optimal solution. This approach can be implemented using a recursive function or a bottom-up dynamic programming algorithm.
• Genetic Algorithm: This approach involves simulating the process of natural selection to find the best solution. A population of possible solutions is generated and evolved over multiple generations. This approach can be implemented using a standard genetic algorithm library or by writing the code from scratch.
• Dijkstra Algorithm: This approach is used to determine the shortest possible path based on the smallest weight from one node to another using a complete graph where each node is connected to all other nodes [3].


5. Methodology
The term branch and bound refer to all state-space searching methods in which all the children node are generated before any other live node i.e., the state space tree expansion. As a set of cities and the distance between every pair of cities , the problem is to get the shortest possible route.
While generating the node , for every node we generate the costs. If any node we are getting the costs is greater than some upper bound , we will generate the node which is fruitful instead of relying on all the nodes by following the path which is taking nearer to the minimization , maximization or optimization of node.
We prepare the tree starting from the root and expand it .

Following, is the state space tree exploring the entire problem for the above WSP problem , which shows the optimal solution when we travel from city ‘i’ to city ‘j’ , the cost of node j will be the sum of the parent node ‘i’ , the cost of an edge (i , j) and the lower bound of the path starting from ‘j’. Since the root node is the first node to be expanded the cost will be only the lower bound path starting at the root.
We try to calculate the lower bound of the path , using the resulting cost matrix i.e., at level 4 , we get the minimized and maximized cost.
Devising Serial Algorithm
In serial algorithm , we introduce a function that tries to visit all the city and adds costs of going to the next city and compares the current costs and lower bound of the path by comparing with the best costs found so far. The function if recursive calls itself with the next city as the current vertex , while the array keeps the track of which of the cities visited so far and calculates the lower bound of the current path and the main function calculates the execution time of the respective algorithm to find the best path and costs.
  
The function starts from the city and visits to the next city , the current solution along with the best solution is stored in an array as current_path and b_path . The visited array is used to keep a track of the cities that already has been visited.The function helps in getting the best solution and displays the best optimal path and costs.A lower bound function is used in the whole search tree to check the best path. It calculates the lower bound of the current path by visiting to the nearest unvisited vertex starting from the current vertex and consecutively add costs of going to that vertex.


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
   

Load Balancing and potential synchronization
The algorithm balances the load of the solution among multiple tasks in order to reduce the overall execution time as the data is broadcast to all the processors and each processor takes a portion of the cities as its starting pint which is determined by the bock size variable. At the end, each processor send its best path back and compare the received solution and update the best one .This approach helps in load balancing as each processor has a portion of the cities as its starting points. Adding more processor creates a noticeable amount of synchronization. The barrier is used after MPI_Bcast to broadcast number of cities n, the distance matrix dist and the current lower bound to avoid any race conditions and overlapping between processors and to ensure that each process can start its calculations.

Here MPI_Barrier is used to synchronize the program so that portions of the parallel code can be timed accurately.
