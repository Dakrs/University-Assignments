#include "my_papi.h"

static int EventSet = PAPI_NULL;
static int * Events = NULL;
static int NumberOfEvents = 0;
static int n_repetitions = 0;
static int at_repetition = 0;

long long * values = NULL;

double * clearcache = NULL;

static unsigned long omp_get_thread_id(){
    unsigned long res = (unsigned long) omp_get_thread_num();
    return res;
}

int initPapi(int parallel){
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT){
		printf("Error at init");
        return 0;
    }
    if (parallel == 0 && PAPI_thread_init(omp_get_thread_id) != PAPI_OK){
        printf("Error at init threads");
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

int setMissRates(int n_rep){
    if (Events != NULL){
        free(Events);
    }
    Events = malloc(sizeof(int)*CACHE_EVENTS);
    Events[0] = PAPI_L1_DCM;
    Events[1] = PAPI_L2_TCM;
    Events[2] = PAPI_L3_TCM;

    NumberOfEvents = CACHE_EVENTS;
    int retval = PAPI_add_events(EventSet,Events,CACHE_EVENTS);

    if (retval != PAPI_OK){
        printf("Error while adding events.");
        return 0;
    }

    n_repetitions = n_rep;
    values = malloc(n_repetitions*CACHE_EVENTS*sizeof(long long));

    return 1;
}

int setMemoryStats(int n_rep){
    if (Events != NULL){
        free(Events);
    }
    Events = malloc(sizeof(int)*CACHE_EVENTS_INS);
    Events[0] = PAPI_L1_DCM;
    Events[1] = PAPI_L2_TCM;
    Events[2] = PAPI_L3_TCM;
    Events[3] = PAPI_TOT_INS;

    NumberOfEvents = CACHE_EVENTS_INS;
    int retval = PAPI_add_events(EventSet,Events,CACHE_EVENTS_INS);

    if (retval != PAPI_OK){
        printf("Error while adding events.");
        return 0;
    }

    n_repetitions = n_rep;
    values = malloc(n_repetitions*CACHE_EVENTS_INS*sizeof(long long));

    return 1;
}

int setLoadStoreCounter(int n_rep){
    if (Events != NULL){
        free(Events);
    }
    Events = malloc(sizeof(int)*LOAD_STORES);
    Events[0] = PAPI_LD_INS;
    Events[1] = PAPI_SR_INS;
    Events[2] = PAPI_TOT_INS;
    //Events[3] = PAPI_FP_INS;

    NumberOfEvents = LOAD_STORES;
    int retval = PAPI_add_events(EventSet,Events,LOAD_STORES);

    if (retval != PAPI_OK){
        printf("Error while adding events.");
        return 0;
    }

    n_repetitions = n_rep;
    values = malloc(n_repetitions*LOAD_STORES*sizeof(long long));

    return 1;
}

int setFloatCounter(int n_rep){
    if (Events != NULL){
        free(Events);
    }
    Events = malloc(sizeof(int)*FLOAT_INST);
    Events[0] = PAPI_FP_INS;
    Events[1] = PAPI_TOT_INS;


    NumberOfEvents = FLOAT_INST;
    int retval = PAPI_add_events(EventSet,Events,FLOAT_INST);

    if (retval != PAPI_OK){
        printf("Error while adding events.");
        return 0;
    }

    n_repetitions = n_rep;
    values = malloc(n_repetitions*FLOAT_INST*sizeof(long long));

    return 1;
}

int setInstructionCounter(int n_rep){
    if (Events != NULL){
        free(Events);
    }
    Events = malloc(sizeof(int)*INS_CYL_EVENTS);
    Events[0] = PAPI_TOT_INS;
    Events[1] = PAPI_TOT_CYC;

    NumberOfEvents = INS_CYL_EVENTS;
    int retval = PAPI_add_events(EventSet,Events,INS_CYL_EVENTS);

    if (retval != PAPI_OK){
        printf("Error while adding events.");
        return 0;
    }
    n_repetitions = n_rep;
    values = malloc(n_repetitions*INS_CYL_EVENTS*sizeof(long long));
    return 1;
}

int start(){
    int retval = PAPI_start(EventSet);

    if (retval != PAPI_OK){
        printf("Error while starting");
        return 0;
    }

    return 1;
}

int stop(){
    int retval = PAPI_stop(EventSet,values+NumberOfEvents*at_repetition);
    at_repetition++;

    if (retval != PAPI_OK){
        printf("Error while stoping");
        return 0;
    }
}

void getInstructionResults(){
    long long * n_instructions = malloc(sizeof(long long)*n_repetitions);
    float * runs_cpi = malloc(sizeof(float)*n_repetitions);

    for(int i = 0; i < n_repetitions; i++){
        float cpi = (float) values[INS_CYL_EVENTS*i] / (float) values[INS_CYL_EVENTS*i + 1];
        runs_cpi[i] = cpi;
        n_instructions[i] = values[INS_CYL_EVENTS*i];
    }

    float kbest_inst = meanKBestLongLong(n_instructions,n_repetitions,5,0);
    float kbest_cpi = meanKBestFloat(runs_cpi,n_repetitions,5,0);


    printf("KBEST %d - CPI = %.3f  INS_NUMBERS = %.3f\n",5,kbest_cpi,kbest_inst);

    free(values);
    n_repetitions = 0;
    at_repetition = 0;
    NumberOfEvents = 0;

    free(n_instructions);
    free(runs_cpi);
}

void getCacheResults(){
    float * mr_l2_array = malloc(sizeof(float)*n_repetitions);
    float * mr_l3_array = malloc(sizeof(float)*n_repetitions);
    long long * l2_miss_acesses = malloc(sizeof(long long)*n_repetitions);
    long long * l3_miss_acesses = malloc(sizeof(long long)*n_repetitions);

    for(int i = 0; i < n_repetitions; i++){
        float mr_l2 = (float) values[1 + CACHE_EVENTS*i] / (float) values[CACHE_EVENTS*i];
	    float mr_l3 = (float) values[2 + CACHE_EVENTS*i] / (float) values[1 + CACHE_EVENTS*i];

        l2_miss_acesses[i] = values[1 + CACHE_EVENTS*i];
        l3_miss_acesses[i] = values[2 + CACHE_EVENTS*i];

        mr_l2_array[i] = mr_l2;
        mr_l3_array[i] = mr_l3;
    }
    float kbest_l2 =  meanKBestFloat(mr_l2_array,n_repetitions,5,0);
    float kbest_l3 =  meanKBestFloat(mr_l3_array,n_repetitions,5,0);
    float l2_miss_mean = meanKBestLongLong(l2_miss_acesses,n_repetitions,5,0);
    float l3_miss_mean = meanKBestLongLong(l3_miss_acesses,n_repetitions,5,0);

    printf("KBEST %d - MR_L2 = %.3f  MR_L3 = %.3f Misses_L2 = %.3f Misses_L3 = %.3f\n",5,kbest_l2,kbest_l3,l2_miss_mean,l3_miss_mean);


    free(values);
    n_repetitions = 0;
    at_repetition = 0;
    NumberOfEvents = 0;

    free(mr_l2_array);
    free(mr_l3_array);
}

void getMemStats(){
    long long * l2_miss_acesses = malloc(sizeof(long long)*n_repetitions);
    long long * l3_miss_acesses = malloc(sizeof(long long)*n_repetitions);
    long long * n_instructions = malloc(sizeof(long long)*n_repetitions);

    for(int i = 0; i < n_repetitions; i++){

        l2_miss_acesses[i] = values[1 + CACHE_EVENTS_INS*i];
        l3_miss_acesses[i] = values[2 + CACHE_EVENTS_INS*i];
        n_instructions[i] = values[3 + CACHE_EVENTS_INS*i];
    }
    float l2_miss_mean = meanKBestLongLong(l2_miss_acesses,n_repetitions,5,0);
    float l3_miss_mean = meanKBestLongLong(l3_miss_acesses,n_repetitions,5,0);
    float kbest_inst = meanKBestLongLong(n_instructions,n_repetitions,5,0);

    printf("KBEST %d - Misses_L2 = %.3f Misses_L3 = %.3f N_inst=%.3f\n",5,l2_miss_mean,l3_miss_mean,kbest_inst);


    free(values);
    n_repetitions = 0;
    at_repetition = 0;
    NumberOfEvents = 0;

    free(l2_miss_acesses);
    free(l3_miss_acesses);
    free(n_instructions);
}

void getLoadStoreResults(){
    long long * load_inst = malloc(sizeof(long long)*n_repetitions);
    long long * store_inst = malloc(sizeof(long long)*n_repetitions);
    long long * n_instructions = malloc(sizeof(long long)*n_repetitions);
    long long * flop_inst = malloc(sizeof(long long)*n_repetitions);

    for(int i = 0; i < n_repetitions; i++){

        load_inst[i] = values[LOAD_STORES*i];
        store_inst[i] = values[LOAD_STORES*i + 1];
        n_instructions[i] = values[LOAD_STORES*i + 2];
        //flop_inst[i] = values[LOAD_STORES*i + 3];
    }
    float loads_mean = meanKBestLongLong(load_inst,n_repetitions,5,0);
    float stores_mean = meanKBestLongLong(store_inst,n_repetitions,5,0);
    float kbest_inst = meanKBestLongLong(n_instructions,n_repetitions,5,0);
    //float kbest_float = meanKBestLongLong(flop_inst,n_repetitions,5,0);

    printf("KBEST %d - Loads = %.3f Stores = %.3f N_inst=%.3f\n",5,loads_mean,stores_mean,kbest_inst);


    free(values);
    n_repetitions = 0;
    at_repetition = 0;
    NumberOfEvents = 0;

    free(load_inst);
    free(store_inst);
    free(n_instructions);
    free(flop_inst);
}

void getFloatInstResults(){
    long long * n_instructions = malloc(sizeof(long long)*n_repetitions);
    long long * flop_inst = malloc(sizeof(long long)*n_repetitions);

    for(int i = 0; i < n_repetitions; i++){

        n_instructions[i] = values[FLOAT_INST*i+1];
        flop_inst[i] = values[FLOAT_INST*i];
    }

    float kbest_inst = meanKBestLongLong(n_instructions,n_repetitions,5,0);
    float kbest_float = meanKBestLongLong(flop_inst,n_repetitions,5,0);

    printf("KBEST %d - N_inst=%.3f Float=%.3f\n",5,kbest_inst,kbest_float);


    free(values);
    n_repetitions = 0;
    at_repetition = 0;
    NumberOfEvents = 0;

    free(n_instructions);
    free(flop_inst);
}

void clearCache(){
    if (clearcache == NULL){
        clearcache = malloc(sizeof(double)*30000000);
    }

    for (unsigned i = 0; i < 30000000; ++i)
		clearcache[i] = i;
}



