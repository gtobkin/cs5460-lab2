#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <assert.h>
#include "mw_api.h"

struct userdef_work_t {
	int quantity; // how many 1's to add together
};

struct userdef_result_t {
	int subSum;
};

// Take a task description from command line arguments
// Produce a pointer to a list (array of pointers) to work elements
// (Partially) specified by the user
// argc/argv: from int main(); command-line argument count, identities
// numWorkers: number of (non-master) threads in this parallel computation
// (may let the user define work units more intelligently)
mw_work_t ** createWorkList (int argc, char **argv, int numWorkers) {
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

// Process our collection of results (by master, post-worker phase).
// Returns 1 on success, 0 otherwise.
// sz: # of mw_result_t's in collection
// res: pointer to first mw_result_t
// User-defined.
int result (int sz, mw_result_t *res) {
	int sum = 0;
	int i;
	mw_result_t * ptr = res;
	for (i = 0; i < sz; i++) {
		sum += (*ptr).subSum;
		ptr += sizeof(mw_result_t*);
	}
	printf("Sum is: %d\n", sum);
	return(1);
}

// Process (on a worker) a single unit of work
// Return NULL if there is no result, non-NULL otherwise
// work: pointer to mw_work_t to be processed
// User-defined.
mw_result_t * compute (mw_work_t *work) {
	// Vapidly simple process for our first test case
	// Just add together n 1's
	int sum = 0;
	int i;
	for (i = 0; i < (*work).quantity; i++) {
		sum++;
	}
	mw_result_t res = { sum };
	mw_result_t *ptr = &res;
	return (ptr);
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

	MPI_Init(&argc, &argv);

	// Create an MPI_Datatype for the work unit struct...
	const int workItemCount = 1;
	int workBlocklengths[1] = {1};
	MPI_Datatype workTypes[1] = {MPI_INT};
	MPI_Datatype * workType;
	MPI_Aint workOffsets[1];
	workOffsets[0] = offsetof(mw_work_t, quantity);
	MPI_Type_create_struct(workItemCount, workBlocklengths,
		workOffsets, workTypes, workType);
	MPI_Type_commit(workType);

	// ... and the results unit struct
	const int resultsItemCount = 1;
	int resultsBlocklengths[1] = {1};
	MPI_Datatype resultsTypes[1] = {MPI_INT};
	MPI_Datatype * resultType;
	MPI_Aint resultsOffsets[1];
	resultsOffsets[0] = offsetof(mw_result_t, subSum);
	MPI_Type_create_struct(resultsItemCount, resultsBlocklengths,
		resultsOffsets, resultsTypes, workType);
	MPI_Type_commit(workType);

	// ... and the results unit struct
	struct mw_api_spec f;
	
	f.create = &createWorkList;
	
	exit(0);
}

