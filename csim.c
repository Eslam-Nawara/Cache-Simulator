#include <stdio.h>
#include <stdlib.h>
#include "cachelab.h"
#include "cache.h"

int main(int argc, char *argv[]) {
    int hits = 0, misses = 0, evictions = 0;
    int setBits, blockBits, linesPerSet;
    struct Cache *cache;
    FILE *traceFile;

    parse_arguments(argc, argv, &setBits, &linesPerSet, &blockBits, &traceFile);

    init_cache(&cache, setBits, linesPerSet);

    process_trace_file(cache,traceFile, setBits, linesPerSet, blockBits,&hits, &misses, &evictions);

    printSummary(hits, misses, evictions);

    return 0;
}
