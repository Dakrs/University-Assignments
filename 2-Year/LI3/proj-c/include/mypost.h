#ifndef __MYPOST_H__
#define __MYPOST_H__

#include "mydate.h"
#include "mytree.h"
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlwriter.h"
#include "/usr/include/libxml2/libxml/tree.h"
#include "/usr/include/libxml2/libxml/xmlmemory.h"
#include <stdlib.h>




typedef struct mypost * MYPOST;

typedef struct stackpost * STACKPOST;

//Stackpost
int 		trans_arr				(STACKPOST arr, int (*f_box)(void *, void *, void *, void *, void *), void * data1, void * data2, void * data3, void * data4);
int 		ordenaPOST_MYUSER		(const void * data1, const void * data2);
void 		order_STACKPOST			(STACKPOST st, void * func);
STACKPOST	initSTACKPOST			(long size);
void 		incCounter2_STACKPOST	(STACKPOST st,long i);
void 		incCounter1_STACKPOST	(STACKPOST st,long i);
long 		getCounter1_STACKPOST	(STACKPOST st);
long 		getCounter2_STACKPOST	(STACKPOST st);
void		insereSTACKPOST			(STACKPOST st, MYPOST post);
void 		insere_sem_rep_STACKPOST(STACKPOST st, MYPOST post);
long		get_NUM_eleSTACKPOST	(STACKPOST st);
MYPOST 		get_ele_index_STACKPOST	(STACKPOST st, long i);
void 		freeSTACKPOST_SEM_CLONE	(STACKPOST st);
void 		freeSTACKPOST_COM_CLONE	(STACKPOST st);

long 		getIdP					(MYPOST post);
int 		getPostTypeIdP			(MYPOST post);
int 		getTYPECLONEP			(MYPOST post);
long 		getOwnerIdP				(MYPOST post);
MYDATE 		getDateP				(MYPOST post);
char * 		getOwnerNameP			(MYPOST post);
char * 		getTitleP				(MYPOST post);
char **		getTagsP				(MYPOST post); // tem codigo a comm
int 		getAnswersP				(MYPOST post);
int 		getCommentsP			(MYPOST post);
int 		getFavsP				(MYPOST post);
long 		getPIdP					(MYPOST post);
int 		getScoreP				(MYPOST post);
STACKPOST	getFilhosP				(MYPOST post);
void 		setFilhosNoPost			(MYPOST post,MYPOST data);

MYPOST 		createpost				(int type);
void 		freepost				(MYPOST post);
void 		free_StringArray		(char ** arr);

MYPOST 		clone_MYPOST_NODEEP		(MYPOST post);
STACKPOST 	clone_STACKPOST			(STACKPOST st);
MYPOST 		clone_MYPOST_DEEP		(MYPOST post);
MYPOST 		search_POSTID			(TREE tree, long id);
void 		print_posts_MYPOST		(MYPOST post);
int			existeTag				(MYPOST post,char * tag); // tem printf
//XML
void 		xmltoMYPOST				(MYPOST post, xmlNodePtr xml, xmlDocPtr doc, TREE treeid, TREE treeusers);
MYDATE 		xmlToMYDATE				(char * value);
char ** 	xmlToStringArray		(char * value);
STACKPOST 	search_POSTDATA			(TREE tree,MYDATE data);
#endif
