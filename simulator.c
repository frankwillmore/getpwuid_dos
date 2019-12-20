#include <pwd.h>
#include <mpi.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include <dlfcn.h>


int dl_make_call(char* function_lib, char* function_name, char** args)
{
    static void* handle;
    static int (*function)();
    static char* error;

    // get the function, first time will open the lib
    if (handle == NULL) 
    {
        handle = dlopen(function_lib, RTLD_LAZY);
        if (!handle) {
            fputs (dlerror(), stderr);
            goto ERROR;
        }
    }

    if (function == NULL)
    {
        // This awkward cast provided to you by the ISO standards team...
        *(void **) (&function) = dlsym(handle, function_name);
        if ((error = dlerror()) != NULL)  {
            fputs(error, stderr);
            goto ERROR;
        }   
    }

    // call it
    int retval = (int)(*function)();
    return retval;

ERROR:

    return 0;
}

int make_call(uid_t uid)
{
    struct passwd pwd;
    errno = ENOENT;
    char buffer[256];
    struct passwd* result;
    getpwuid_r(uid, &pwd, buffer, 256, &result); 
    if (errno != 0) return 1;
    else return 0;
}

int main(int argc, char** argv)
{
    int failures = 0;
    int attempt = 0;

    // parse args, get noise param, attempts
    int n_attempts = 1024; // number of attempts to make
    int noise = 1000; // max wait time between attempts in microseconds
    int i;
    for (i=0; i<argc; i++) 
    {   
        if (strcmp("--n_attempts", argv[i])==0) n_attempts = (int)strtol(argv[++i], NULL, 10);
        if (strcmp("--noise", argv[i])==0) noise = (int)strtol(argv[++i], NULL, 10);
    }

    assert (n_attempts < INT_MAX);
    // get my uid
    // default values
    uid_t my_uid = geteuid();

    // MPI Init
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // MPI barrier
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // loop over number of attempts
    while (attempt++ < n_attempts)
    {
        // sleep
        long r=rand(); // use a long to prevent an overflow 
        useconds_t interval = (useconds_t)((noise * r) / RAND_MAX);
        usleep(interval);

        // now do the business
        failures += make_call(my_uid);
    }

    int total_failures;

    // MPI reduce results
    MPI_Reduce(
        &failures,			/* void* send_data */ 
        &total_failures,		/* void* recv_data */
        1,				/* int count */
        MPI_INT,			/* MPI_Datatype datatype */
        MPI_SUM, 			/* MPI_Op op */
        0,				/* int root */
        MPI_COMM_WORLD			/* MPI_Comm communicator*/
        );

    double finish = MPI_Wtime();

    // report stats
    if (world_rank == 0)
    {
        printf("\n##############################################################\n\n");
        printf("  Total failures:                   %d\n", total_failures); 
        printf("  Total attempts:                   %d\n", world_size * n_attempts); 
        printf("  Elapsed time:                     %g\n", finish-start );
        printf("  Attempts per second:              %g\n", world_size * n_attempts / (finish - start) );
        printf("  Percentage of failed attempts:    %g%%\n", 100.0 * failures / n_attempts);
        printf("\n##############################################################\n\n");
    }

    // MPI finalize
    MPI_Finalize();

} 
