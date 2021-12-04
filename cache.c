#include "cache.h"
#include "getopt.h"
#include <stdlib.h>

typedef struct Cache_line {
    int valid;
    long long tag;
    int least_access_time;
} cacheLine;

typedef struct Cache_set {
    cacheLine *set;
    int to_add_to;
} cacheSet;

typedef struct Cache {
    cacheSet *sets;
} cache;


char operationString[2];
long long address;

static int find_lru_line(cache *, int, int);

/*
 * 
 */

 static void fetch(struct Cache *, int, int, long long, char, int, int *, int *, int *);

/*
 * 
 */


void init_cache(cache **c, int setBits, int linesPerSet) {
    int setsNum = 1 << (setBits);
    *c = malloc(sizeof(cache));
    (*c)->sets = malloc(sizeof(cacheSet) * setsNum);

    for (int i = 0; i < setsNum; ++i) {
        (*c)->sets[i].set = malloc(sizeof(cacheLine) * linesPerSet);
        (*c)->sets[i].to_add_to = 0;
        for (int j = 0; j < linesPerSet; ++j) {
            (*c)->sets[i].set[j].valid = 0;
            (*c)->sets[i].set[j].least_access_time = -1;
        }
    }
}

void parse_arguments(int argc, char *argv[], int *setBits, int *linesPerSet, int *blockBits, FILE **traceFile) {
    int opt;
    while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch (opt) {
            case 's': *setBits = atoi(optarg); break;
            case 'b': *blockBits = atoi(optarg); break;
            case 'E': *linesPerSet = atoi(optarg); break;
            case 't':
                char *file_path_ptr = optarg;
                *traceFile = fopen(file_path_ptr, "r");
                break;
            default: exit(1);
        }
    }
    if (*setBits <= 0 || *blockBits <= 0 || *linesPerSet <= 0 || *traceFile == NULL) exit(1);
}

void process_trace_file(cache *Cache, FILE *traceFile, int setBits, int linesPerSet, int blockBits, int *hits, int *misses, int *evictions) {
    char opS[2];
    long long address;
    int size, time = 0;
    long long setBitsOnes = (1 << setBits) - 1;
    while (fscanf(traceFile, "%s %llx,%d\n", opS, &address, &size) != EOF) {
        address >>= blockBits;
        fetch(Cache, linesPerSet, ((int) (address & setBitsOnes)), (address >> (setBits)), opS[0], time, hits, misses, evictions);
        time++;
    }
}


void fetch(cache *c, int linesPerSet, int set, long long tag, char op, int time,
                     int *hits, int *misses, int *evictions) {
    if (op == 'I') return;
    if (op == 'M') (*hits)++;
    cacheLine *currSet = c->sets[set].set;

    //check for hit
    for (int i = 0; i < linesPerSet; ++i) {
        if (currSet[i].valid == 1 && currSet[i].tag == tag) {
            (*hits)++;
            currSet[i].least_access_time = time;
            return;
        }
    }

    (*misses)++;

    //miss and no empty line
    if (c->sets[set].to_add_to == linesPerSet) {
        (*evictions)++;
        int leastAccPos = find_lru_line(c, set, linesPerSet);
        currSet[leastAccPos].tag = tag;
        (currSet[leastAccPos].least_access_time) = time;
        return;
    }

    //miss and empty line
    currSet[c->sets[set].to_add_to].valid = 1;
    currSet[c->sets[set].to_add_to].tag = tag;
    currSet[c->sets[set].to_add_to].least_access_time = time;
    c->sets[set].to_add_to++;
}

int find_lru_line(cache *c, int set, int linesPerSet) {
    unsigned int leastAcc = -1, pos = 0;
    for (int i = 0; i < linesPerSet; i++) {
        int temp = c->sets[set].set[i].least_access_time;
        if (temp < leastAcc) {
            leastAcc = temp;
            pos = i;
        }
    }
    return pos;
}
