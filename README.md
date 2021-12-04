# Cache-Simulator
## Description:
  - This program is a simulator for the behavior of the cache memory system.
  - It is part A of the cache lab - the 4th lab of [2015 Fall: 15-213 Introduction to Computer Systems](https://www.cs.cmu.edu/afs/cs/academic/class/15213-f15/www/schedule.html).
  - The simulator takes a [valgrind](https://man7.org/linux/man-pages/man1/valgrind.1.html) memory trace as input, simulates the behavior of cache memory of an arbitrary size on that trace and outputs the total number of hits, misses and evictions.
  - This simulator uses LRU to choose the least recently used cache line to evict.

## Lab Files:
 - `cachelab.pdf`:  Lab writeup
- `csim.c`: contains the main function
- `cache.c`: contains my cache simulator
- `traces`: directory contains the valgrind memory trace files.
- `test-sim`: tests the correctness of your simulator.
