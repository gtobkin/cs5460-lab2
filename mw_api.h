struct userdef_work_t; /* definition provided by user */
struct userdef_result_t; /* definition provided by user */
typedef struct userdef_work_t mw_work_t;
typedef struct userdef_result_t mw_result_t;

struct mw_api_spec {
	// create work: return a NULL-terminated list of work.
	// Return NULL if it fails. */
	mw_work_t **(*create) (int argc, char **argv); 
	
	// Process result. Input is a collection of results, of size sz.
	// Returns 1 on success, 0 on failure.
	int (*result) (int sz, mw_result_t *res);      

	// Compute, returning NULL if there is no result,
	// non-NULL if there is a result to be returned.
  	mw_result_t *(*compute) (mw_work_t *work);    

   	// Size in bytes of the work structure and result structure,
	// needed to send/receive messages.
	int work_sz, res_sz; 
};
 	// Run master-worker
void MW_Run (int argc, char **argv, struct mw_api_spec *f);
