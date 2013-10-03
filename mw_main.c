#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <assert.h>
#include "mw_api.h"

struct userdef_work_t {
	int quantity; // how many 1's to add together
};

struct userdef_result_t {
	int sum;
};

// Take a task description from command line arguments
// Produce a pointer to a list (array of pointers) to work elements
// (Partially) specified by the user
// argc/argv: from int main(); command-line argument count, identities
// numWorkers: number of (non-master) threads in this parallel computation
// (may let the user define work units more intelligently)
mw_work_t** createWorkList (int argc, char **argv, int numWorkers) {
	// Nonspecific createWorkList code follows
	// Do not change for different master-worker tasks
	if (argc == 1) {
		exit(-1); // no parameter(s), so we can't describe the task
	}
	mw_work_t ** workList;
	
	// And now the task-specific portion. User-supplied.
	int n = atoi(argv[1]); // assumes valid input; no parsing
	// +1 for the null terminator
	workList = calloc(numWorkers + 1, sizeof(mw_work_t*));
	int i;
	mw_work_t tempWork;
	for (i = 0; i < numWorkers-1; i++) {
		workList[i] = malloc(sizeof(mw_work_t));
		tempWork = (mw_work_t) { n / numWorkers };
		*workList[i] = tempWork;
	}
	// TODO: better workload splitting
	// this overloads the last worker, but does have the right # work
	// only matters when # work doesn't >> # nodes anyway
	workList[numWorkers-1] = malloc(sizeof(mw_work_t));
	tempWork = (mw_work_t) { n - (n / numWorkers)*(numWorkers-1) };
	*workList[numWorkers-1] = tempWork;
	// and finally add the terminator
	workList[numWorkers] = NULL;
	return(workList);
}

int main (int argc, char **argv) {
	
	/*
		TODO:

		1) Instantiate a mw_api_spec struct
		2) Populate it
		3) Run MW_Run(&struct)
		That's it, actually
	
		For our first, trivial problem, we'll just add
		n 1's together. BUT IN PARALLEL!
	*/

	struct mw_api_spec f;
	
	f.create = &createWorkList;

	exit(0);
}

