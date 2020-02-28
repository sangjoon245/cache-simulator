# cache-simulator
This cache simulator simulates the behavior of processes caches.

The program will be given the specification of the cache itself (size, block size, associativity, etc.), along with a trace file that reads and writes from memory.

Initially, all cache lines are invalid. However, as cache misses occur, the lines will become valid. Eventually, the cache will have no valid lines.
When it comes to a cache having no valid lines, there must be some replacement policy. For this code, FIFO (first-in and first-out) will be implementeed.

When running the program, it will take in six arguments :
1. Thecache size, in bytes. This will be a power of two.
2. The associativity, which will be “direct” for a direct-mapped cache, “assoc:n” for ann-wayassociative cache, or “assoc” for a fully associative cache.
3. The prefetching policy, which will be “p0” (no prefetching) or “p1” (prefetch one block ahead).
4. The replacement policy, which will be "lru".
5. The block size, in bytes. This will be a power of two.
6. The trace file

For example, we can run our program by running :

./cachesim 512 direct p0 lru 8 trace1.txt

When it is complete, it will print the number of cache hits, cache misses, memory reads, and memory writes.
