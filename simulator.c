#include <pwd.h>
#include <mpi.h>
#include <errno.h>
#include <stdio.h>

//struct passwd *getpwuid(uid_t uid);


int make_call(uid_t uid)
{
    struct passwd pwd;
    errno = ENOENT;
    char buffer[256];
    getpwuid_r(uid, &pwd, buffer, 256, struct passwd **result); [Option End]
    if (errno != 0) return 1;
    else return 0;
}

int main(int argc, char** argv)
{
    int failures = 0;
    // parse args, get noise param, attempts

    assert (attempts < INT_MAX);
    // get my uid
    uid_t my_uid = geteuid();

    // MPI Init

    // MPI barrier
//    double start = MPI_Wtime();

    // loop over number of attempts
    while ()
        failures += make_call(my_uid);

    // MPI gather results

    // MPI finalize
//    double finish = MPI_Wtime();

    // calculate stats
}
