all: knapsack knapsack_mpi
knapsack: knapsack.cxx
		g++ -g -O4 knapsack.cxx -o knapsack -lpthread
knapsack_mpi: knapsack_mpi.cxx
		mpiCC -g -O4 knapsack_mpi.cxx -o knapsack_mpi 
