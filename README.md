This will simulate load to authentication stack, in order to determine point of failure, and deliver results in hits/second. 
Options:
  - --noise, a random interval between 00 and $arg miliseconds to wait between subsequent calls

This is multithreaded with MPI and will start simulating from an MPI barrier, then gathering statistics.

