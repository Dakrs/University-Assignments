/**
 * @file 	mytags.c
 * @brief	Ficheiro contendo funções utilizadas na gestão da estrutura que armazena Id's de Tags.
 */
#include "mytags.h"

/**
 * @brief			Função dá free a uma hashtable.
 * @param table		Apontador a hashtable.
*/
void freeHASH_TAGS(GHashTable * table){
	g_hash_table_destroy(table);
}

/**
 * @brief			Função que ve se duas keys são iguais.
 * @param data1		Apontador para a key1.
 * @param data2		Apontador para a key2.
 * @return 			boolean (TRUE caso encontre FALSE caso nao).
*/
gboolean hash_equal(const void * data1, const void * data2){
	char * aux1 = (char *) data1;
	char * aux2 = (char *) data2;

	int x = strcmp(aux1,aux2);

	return x == 0 ? TRUE : FALSE;
}

/**
 * @brief			Função que efetua o parsing do ficheiro das Tags e as coloca numa tabela de hash do glib.
 * @param path		Path para o ficheiro xml.
 * @return 			Tabela de HASH das tags.
*/
GHashTable * createMYTAGS_HASH(char * path){

	xmlDocPtr doc;
	xmlNodePtr ptr;
	struct _xmlAttr * cur;
	xmlNodePtr aux;

	GHashTable * table = g_hash_table_new_full(&g_str_hash,&hash_equal,&free,&free);

	if (!(xml_file_to_struct(&doc,&ptr,path)))
		return table;

	long id = -2;
	long * keyid = NULL;
	char * tag = NULL;

	aux = ptr->children->next;
    xmlChar *key;

	while (aux) {
		if (strcmp( (char*) aux->name,"row") == 0){
			cur = aux->properties;

			while (cur) {
				key = xmlNodeListGetString(doc,cur->children,1);

				if (strcmp( (char *) cur->name,"Id") == 0)
					id = atol((char*)key);
				if (strcmp( (char *) cur->name,"TagName") == 0){
					tag = mystrdup((char*)key);
				}

				xmlFree(key);
				cur = cur->next;
			}

			if ((id != -2) && tag){
				keyid = malloc(sizeof(long));
				*keyid = id;
				g_hash_table_insert(table,tag,keyid);
			}
			id = -2;
			tag = NULL;
		}
		aux = aux->next;
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return table;
}
