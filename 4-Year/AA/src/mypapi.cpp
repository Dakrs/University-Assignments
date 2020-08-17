#include "mypapi.h"

static int EventSet = PAPI_NULL;
static int * Events = NULL;
static int NumberOfEvents = 0;
static int n_repetitions = 0;
static int at_repetition = 0;

long long * values = NULL;

int initPapi(){
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT){
		printf("Error at init");
        return 0;
    }
    return 1;
}

int createEventSet(){
    int retval = PAPI_create_eventset(&EventSet);

    if (retval != PAPI_OK){
        printf("Error while creating EventSet");
        return 0;
    }

    return 1;
}

int startPAPI(){
    int retval = PAPI_start(EventSet);

    if (retval != PAPI_OK){
        printf("Error while starting");
        return 0;
    }

    return 1;
}

int stopPAPI(){
    int retval = PAPI_stop(EventSet,values+NumberOfEvents*at_repetition);
    at_repetition++;

    if (retval != PAPI_OK){
        printf("Error while stoping");
        return 0;
    }
}

int setRamINTS(int n_rep){
    if (Events != NULL){
        free(Events);
    }
    Events = (int *) malloc(sizeof(int)*((int)EVENTS_RAM_INTS));
    //Events = new int[EVENTS_RAM_INTS];
    Events[0] = PAPI_L3_TCM;
    Events[1] = PAPI_TOT_INS;

    NumberOfEvents = EVENTS_RAM_INTS;
    int retval = PAPI_add_events(EventSet,Events,((int)EVENTS_RAM_INTS));

    if (retval != PAPI_OK){
        printf("Error while adding events.");
        return 0;
    }

    n_repetitions = n_rep;
    //values = new long long[EVENTS_RAM_INTS];
    values = (long long *) malloc(n_repetitions*((int)EVENTS_RAM_INTS)*sizeof(long long));

    return 1;
}

int setMissRatesL1(int n_rep){
    if (Events != NULL){
        free(Events);
    }
    Events = (int *) malloc(sizeof(int)*((int)EVENTS_CACHE_L1));
    //Events = new int[EVENTS_CACHE];
    Events[0] = PAPI_L1_DCM;
    Events[1] = PAPI_LD_INS;

    NumberOfEvents = EVENTS_CACHE_L1;
    int retval = PAPI_add_events(EventSet,Events,EVENTS_CACHE_L1);

    if (retval != PAPI_OK){
        printf("Error while adding events.");
        return 0;
    }

    n_repetitions = n_rep;
    values = (long long *) malloc(n_repetitions*((int)EVENTS_CACHE_L1)*sizeof(long long));
    //values = new long long[EVENTS_CACHE];

    return 1;
}

int setMissRatesL2L3(int n_rep){
    if (Events != NULL){
        free(Events);
    }
    Events = (int *) malloc(sizeof(int)*((int)EVENTS_CACHE_L2L3));
    //Events = new int[EVENTS_CACHE];
    Events[0] = PAPI_L1_DCM;
    Events[1] = PAPI_L2_TCM;
    Events[2] = PAPI_L3_TCM;
    Events[3] = PAPI_LD_INS;

    NumberOfEvents = EVENTS_CACHE_L2L3;
    int retval = PAPI_add_events(EventSet,Events,EVENTS_CACHE_L2L3);

    if (retval != PAPI_OK){
        printf("Error while adding events.");
        return 0;
    }

    n_repetitions = n_rep;
    values = (long long *) malloc(n_repetitions*((int)EVENTS_CACHE_L2L3)*sizeof(long long));
    //values = new long long[EVENTS_CACHE];

    return 1;
}

void getMissRatesL2L3(){
    //float * missrates = malloc(sizeof(float)*n_repetitions*3);
    float * missrates = new float[n_repetitions*2];

    for(int i = 0; i < n_repetitions; i++){
        float mr_l2 = (float) values[EVENTS_CACHE_L2L3 * i + 1] / (float) ((float) values[EVENTS_CACHE_L2L3 * i]);
        float mr_l3 = (float) values[EVENTS_CACHE_L2L3 * i + 2] / (float) values[EVENTS_CACHE_L2L3 * i + 3];

        missrates[i*2] = mr_l2;
        missrates[i*2 + 1] = mr_l3;

        printf("RUN %d: MR_L2: %2.4f MR_L3: %2.4f \n",i,mr_l2,mr_l3);
    }

    free(values);
    n_repetitions = 0;
    at_repetition = 0;
    NumberOfEvents = 0;

    //delete(missrates);
}

void getMissRatesL1(){
    //float * missrates = malloc(sizeof(float)*n_repetitions*3);
    float * missrates = new float[n_repetitions];

    for(int i = 0; i < n_repetitions; i++){
        float mr_l1 = (float) values[EVENTS_CACHE_L1 * i] / (float) (values[EVENTS_CACHE_L1 * i + 1]);

        missrates[i] = mr_l1;

        printf("RUN %d: MR_L1: %2.4f \n",i,mr_l1);
    }

    free(values);
    n_repetitions = 0;
    at_repetition = 0;
    NumberOfEvents = 0;

    delete(missrates);
}

void getRamINST(){
    float * results = new float[n_repetitions];

    for(int i = 0; i < n_repetitions; i++){
        float perc = (float) values[EVENTS_RAM_INTS*i] / (float) values[EVENTS_RAM_INTS*i + 1];
        results[i] = perc;

        printf("RUN %d: %2.9f\n",i,perc);
    }

    free(values);
    n_repetitions = 0;
    at_repetition = 0;
    NumberOfEvents = 0;

    delete(results);
}

