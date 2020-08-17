#include "papi.h"
#include "measures.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CACHE_EVENTS 3
#define CACHE_EVENTS_INS 4
#define INS_CYL_EVENTS 2
#define LOAD_STORES 3
#define FLOAT_INST 2

int initPapi(int parallel);

int createEventSet();

int setMissRates(int n_rep);

int setInstructionCounter(int n_rep);

int start();

int stop();

void getInstructionResults();

void getCacheResults();

void clearCache();

int setMemoryStats(int n_rep);

void getMemStats();

int setLoadStoreCounter(int rep);

void getLoadStoreResults();

int setFloatCounter(int n_rep);

void getFloatInstResults();