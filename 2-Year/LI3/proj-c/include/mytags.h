#include "loading.h"
#include <glib.h>
#include <string.h>
#include <common.h>


void 			freeHASH_TAGS		(GHashTable * table);
gboolean	 	hash_equal			(const void * data1, const void * data2);
GHashTable * 	createMYTAGS_HASH	(char * path);
