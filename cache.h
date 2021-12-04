#ifndef CACHE_H
#define CACHE_H
#include <stdio.h>

struct Cache;

void init_cache(struct Cache**,int, int); 

/*
 * 
 */

void parse_arguments(int argc, char *[], int *, int *, int *, FILE **);

/*
 * 
 */

void process_trace_file(struct Cache *,FILE*,int,int ,int, int*, int*, int*);

/*
 * 
 */


#endif 
