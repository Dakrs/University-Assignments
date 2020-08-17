#include "/usr/include/libxml2/libxml/parser.h"
#include "/usr/include/libxml2/libxml/xmlwriter.h"
#include "/usr/include/libxml2/libxml/tree.h"
#include "/usr/include/libxml2/libxml/xmlmemory.h"

int main(){
	xmlDocPtr doc;
	xmlNodePtr ptr;
	xmlNodePtr cur;
	xmlNodePtr aux;


	doc = xmlParseFile("Posts.xml");

	if( !doc) {
		fprintf(stderr, "Document not parsed successfully\n");
		return -1;
	}

	ptr = xmlDocGetRootElement(doc);

	if (!ptr) {
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return -2;
	}

	aux = ptr->children->next;
	xmlChar *key;

	while (aux != NULL){
		cur = aux->properties;
		 	while (cur != NULL) {
				if ((!xmlStrcmp(cur->name, (const xmlChar *)"CreationDate"))) {
						key = xmlNodeListGetString(doc,cur->children,1);
						printf("%s\n",key );
					}
						cur = cur->next;
			}
		aux=aux->next;
	}
	return 0;
}
