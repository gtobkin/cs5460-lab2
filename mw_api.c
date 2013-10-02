#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
	int size, myid;
  	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);	
	MPI_Comm_rank (MPI_COMM_WORLD, &myid);

	if (myid == 0) {
		doMasterStuff();
	} else {
		doWorkerStuff();
	}

	MPI_Finalize ();
	exit(0);
}

int doMasterStuff() {
	
	/*
		The plan:
		1) Process input into a list of work units
		2) Send one unit to min(# work, # workers);
			track # sent/received back
		2a) If # work < # workers, request remainder terminate
		3) Upon recv result, increment # recv, and:
		3a) If # unsent work < # workers, request sender terminate
		3b) Else send work unit (and increment # sent)
	*/
	
	exit(0);

	/* Crufty code; retained temp. for syntax reference
	int size, myid;
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &myid);
	int i;
	for (i = 1; i < size; i++) {
		int j = 1, k = 2, out;
		MPI_Send(&j, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		MPI_Send(&k, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		MPI_Recv(&out, 1, MPI_INT, i, 0, MPI_COMM_WORLD,
			MPI_STATUS_IGNORE);
		printf("Result from process %d: %d\n", i, out);
	}
	MPI_Finalize();
	exit(0);
	*/
}

int doWorkerStuff() {

	/*
		The plan:
		1) Wait to receive
		2a) If terminate request, terminate
		2b) Else, perform work and return it
	*/

	exit(0);

	/* More crufty code
	int i, j, out;
	MPI_Recv(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
		MPI_STATUS_IGNORE);
	MPI_Recv(&j, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
		MPI_STATUS_IGNORE);
	out = i + j;
	MPI_Send(&out, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	MPI_Finalize();
	exit(0);
	*/
}
