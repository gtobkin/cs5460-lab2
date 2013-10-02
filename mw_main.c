#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include "mw_api.h"

struct userdef_work_t {
	int quantity; // how many 1's to add together
};

struct userdef_result_t {
	int sum;
};

mw_work_t** createWorkList (int argc, char **argv, int numWorkers) {
	if (argc == 1) {
		exit(-1); // no maxInt parameter
	}
	int n = atoi(argv[1]); // assumes valid input; no parsing
	// +1 for the null terminator
	mw_work_t ** workList;
	workList = calloc(numWorkers + 1, sizeof(mw_work_t*));
	int i;
	for (i = 0; i < numWorkers-1; i++) {
		workList[i] = malloc(sizeof(mw_work_t));
		*workList[i] = n / numWorkers;
	}
	// TODO: better workload splitting
	// this overloads the last worker, but does have the right # work
	workList[numWorkers-1] = malloc(sizeof(mw_work_t));
	int k = n - (n / numWorkers)*(numWorkers-1);
	*workList[numWorkers-1] = k;
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

	printf("%d\n", argc);
	if (argc > 1) {
		printf("%d\n", atoi(argv[1]));
	}
	exit(0);
}

