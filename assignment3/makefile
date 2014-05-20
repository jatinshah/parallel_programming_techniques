all: makegrid simulate_blocking simulate_unblocking

makegrid: makegrid.c
		g++ makegrid.c -o makegrid

simulate_blocking: life-blocking.cxx
		mpiCC life-blocking.cxx -o simulate_blocking

simulate_unblocking: life-unblocking.cxx
		mpiCC life-unblocking.cxx -o simulate_unblocking
