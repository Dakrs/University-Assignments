#ifndef __LOADING__
#define __LOADING__

#include <mypost.h>
#include <myuser.h>
#include <mydate.h>
#include <glib.h>
#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlwriter.h"
#include "/usr/include/libxml2/libxml/tree.h"
#include "/usr/include/libxml2/libxml/xmlmemory.h"




int 	xml_file_to_struct		(xmlDocPtr * doc, xmlNodePtr * ptr, char * filepath);
int 	createMYPOST_TREES		(char * path, TREE * tree_id, TREE * tree_date, TREE treeusers);

#endif
