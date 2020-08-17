#include "papi.h"
#include <stdio.h>
#include <stdlib.h>

#define EVENTS_RAM_INTS 2
#define EVENTS_CACHE_L1 2
#define EVENTS_CACHE_L2L3 4

int initPapi();
int createEventSet();
int startPAPI();
int stopPAPI();
int setRamINTS(int rep);
void getRamINST();
int setMissRatesL1(int rep);
void getMissRatesL1();

int setMissRatesL2L3(int rep);
void getMissRatesL2L3();
