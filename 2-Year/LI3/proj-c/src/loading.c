/**
 * @file 	loading.c
 * @brief	Ficheiro contendo funções utilizadas na gestão do carregamento do dump para memória.
 */
#include <loading.h>



/**
 * @brief 			Função que (recorrendo à biblioteca libxml2) efetua o parsing de um ficheiro xml.
 * @param doc		O apontador do ficheiro xml.
 * @param ptr 		O apontador da estrutura resultante do parsing do ficheiro xml.
 * @param filepath 	O filepath do ficheiro xml a ser lido e carregado.
 * @return			Inteiro usado como boolean
 */
int xml_file_to_struct(xmlDocPtr * doc, xmlNodePtr * ptr, char * filepath) {

	*doc = xmlParseFile(filepath);

	if (!(*doc)) {
		fprintf(stderr, "Document %s not parsed successfully\n", filepath);
		return -1;
	}

	*ptr = xmlDocGetRootElement(*doc);

	if (!(*ptr)) {
		fprintf(stderr, "%s is an empty document\n", filepath);
		return -2;
	}

	return 1;
}

/**
 * @brief 			Função que tira o primeiro elemento de um GArray e o coloca noutro.
 * @param 			GArray 1
 * @param 			GArray 2
 */
static void * concat_post(void * data1, void * data2){

	STACKPOST a1 = (STACKPOST) data1;
	STACKPOST a2 = (STACKPOST) data2;
	MYPOST a = get_ele_index_STACKPOST(a2,0);
	insereSTACKPOST(a1,a);

	freeSTACKPOST_SEM_CLONE(a2);

	return a1;
}

/**
 * @brief 			Função que cria as àrvores balanceadas de posts segundo id e data de criação.
 * @param path		O ficheiro Posts.xml.
 * @param tree_id 	O apontador onde ficará apontada a àrvore ordenada segundo id's.
 * @param tree_date	O apontador onde ficará apontada a àrvore ordenada segundo datas de criação.
 * @param treeusers A árvore de users.
 * @return 			Inteiro usado como boolean.
 */
int createMYPOST_TREES(char * path, TREE * tree_id, TREE * tree_date, TREE treeusers) {
	xmlDocPtr 	doc;
	xmlNodePtr	cur = NULL;

	if( !xml_file_to_struct(&doc, &cur, path)) {							//Load para a estrutura do libxml2 a partir do ficheiro.
		fprintf(stderr, "Could not create user tree from %s\n", path);
		return -1;
	}
	STACKPOST list;
	MYPOST	post;
	long * 	keyid;
	TREE 	treeid 		= createTREE(&compare_user, &freeKey, &freepost,NULL);
	TREE	treedate	= createTREE(&compare_MYDATE_AVL, &free_MYdate, &freeSTACKPOST_SEM_CLONE,&concat_post); //é preciso dar free á data xD pensar como fazer isso.

	for(cur = cur->children; cur; cur = cur->next) {						// Percorre os posts todos.
		if(strcmp("row", (char *) cur->name) == 0) {
			post = createpost(1);											//Cria um nodo post.
			xmltoMYPOST(post, cur, doc, treeid, treeusers);					//Preenche essa struct post.

			keyid 	= malloc(sizeof(long));
			*keyid = getIdP(post);
			setPostToUSER(treeusers, getOwnerIdP(post), post);

			list = initSTACKPOST(1);
			insereSTACKPOST(list,post);


			insere_tree(treeid, keyid, post);								//Insere este nodo na árvore ordenada por id's.
			insere_tree(treedate, getDateP(post), list);							//Insere este nodo na árvore ordenada cronológicamente.
		}
	}

	*tree_id 	= treeid;
	*tree_date 	= treedate;

	xmlFreeDoc(doc);
	xmlFreeNode(cur);
	xmlCleanupParser();

	return 1;
}
