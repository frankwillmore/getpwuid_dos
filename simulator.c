#include <pwd.h>
#include <mpi.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

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
    // default values
    int n_attempts = 1024; // number of attempts to make
    int noise = 1000; // max wait time between attempts in microseconds

    // parse args, get noise param, attempts
    int i;
    for (i=0; i<argc; i++) 
    {   
        if (strcmp("--n_attempts", argv[i])==0) n_attempts = (int)strtol(argv[++i], NULL, 10);
        if (strcmp("--noise", argv[i])==0) noise = (int)strtol(argv[++i], NULL, 10);
    }
printf("got n_attempts = %d", n_attempts);
printf("got noise = %d", noise);

    assert (n_attempts < INT_MAX);
    // get my uid
    uid_t my_uid = geteuid();

    // MPI Init
    MPI_Init(&argc, &argv);

    // MPI barrier
    double start = MPI_Wtime();

    // loop over number of attempts
    while (attempt++ < n_attempts)
    {
        // sleep
        long r=rand(); // use a long to prevent an overflow 
        useconds_t interval = (useconds_t)((noise * r) / RAND_MAX);
//        long interval = ((1000000 * (long)r) / RAND_MAX);
printf("interval: %ld\n", interval );
        usleep(interval);

        // now do the business
        failures += make_call(my_uid);
    }
    // MPI gather results

    // MPI finalize
    double finish = MPI_Wtime();
    MPI_Finalize();

    // calculate stats

    printf("attempts:\tfailures:\telapsed time:\n");
    printf("%d\t%d\t%g\n", n_attempts, failures, finish-start );
}
