This will simulate load to authentication server by storming calls to getpwuid and deliver results as:

##############################################################

  Total failures:                   0
  Total attempts:                   4096
  Elapsed time:                     0.862779
  Attempts per second:              1186.86
  Percentage of failed attempts:    0%

##############################################################

Options:
  - --noise, a random interval between 0 and $arg microseconds to wait between subsequent calls
  - --n_attempts, number of attempts per task 

This is an MPI code and will start simulating after synchronization at an MPI barrier.

