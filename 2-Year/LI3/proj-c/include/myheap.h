#ifndef __MYHEAP_H__
#define __MYHEAP_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct generico GEN;
typedef struct heap * HEAP;
typedef struct stack * STACK;

void 	swap				(GEN * v,int i, int d);
HEAP 	initHEAP			(long size);
void 	bubble_up			(HEAP * main);
void 	bubble_down			(HEAP * main);
void 	insereHEAP			(HEAP h, long key, long data);
void 	pop 				(HEAP h, long * key, long * data);
long 	get_NUM_eleHEAP		(HEAP a);
void 	freeMYHEAP			(HEAP h);
int  	teste_heap			(HEAP a);

STACK 	initSTACK			(long size);
void  	freeSTACK			(STACK a);
STACK 	insereSTACK			(STACK a,long id);
long  	get_NUM_eleSTACK	(STACK a);
long  	get_ELE_index		(STACK a,int index);
void 	trans_stack			(STACK arr, void (*f_box)(long , void *, void *, void *), void * data1, void * data2, void * data3);
#endif
