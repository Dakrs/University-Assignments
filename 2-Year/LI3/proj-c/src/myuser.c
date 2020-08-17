/**
 * @file 	myuser.c
 * @brief	Ficheiro contendo todas a funções relativa à estrutura MYUSER.
*/
#include "myuser.h"

struct myuser{
    long id;
    int rep;
    char * username;
    char * bio;
	int type;
	STACKPOST posts;
};

/**
 * @brief				Função que devolve o id do user.
 * @param use			Apontador para o user.
 * @return 				ID do user.
*/
long getIdMYUSER(MYUSER use){
	return use->id;
}

/**
 * @brief			Função que devolve a rep do user.
 * @param  use		Apontador para o user.
 * @return 			REP do user.
*/
int getREPMYUSER(MYUSER use){
	return use->rep;
}

/**
 * @brief			Função que devolve o Username do user.
 * @param  use		Apontador para o user.
 * @return 			Username do user.
*/
char * getUsername(MYUSER use){
	if (use)
		return mystrdup(use->username);
	return NULL;
}

/**
 * @brief			Função que devolve a biografia do user.
 * @param	use		Apontador para o user.
 * @return 			Biografia do user.
*/
char * getBiography(MYUSER use){
	if (use)
		return mystrdup(use->bio);
	return NULL;
}

/**
 * @brief			Função que devolve a lista de posts de um USER.
 * @param  use		Apontador para o user.
 * @return 			Lista de posts feito pelo user.
*/
STACKPOST getMYLISTuser(MYUSER use){
	if (use) return use->posts;
	return NULL;
}

/**
 * @brief			Função que devolve os ultimos N posts de um dado utilizador.
 * @param use		Apontador para o user.
 * @param n			Número de posts.
 * @param n_elem	Apontador para onde se colocam o número de valores inseridos.
 * @return 			Array com os ultimos N posts do user.
*/
long * getNposts(MYUSER use,int n,int * n_elem){
	long * r = malloc(n*sizeof(long));
	int i = 0;
	MYPOST post = NULL;
	long k = get_NUM_eleSTACKPOST(use->posts), t = 0;

	for(t=0; i < n && t < k; t++){
		post = get_ele_index_STACKPOST(use->posts,t);
		if (post != NULL && (getPostTypeIdP(post) == 2 || getPostTypeIdP(post) == 1)){
			r[i++] = getIdP(post);
		}
	}
	*n_elem = i;
	return r;
}

/**
 * @brief			Função que devolve o número de posts de um utilizador.
 * @param use		Apontador para o user.
 * @return 			Numero de posts feitos pelo user.
*/
long getNUM_POST_MYUSER(MYUSER use){
	if (use)
		return get_NUM_eleSTACKPOST(use->posts);
	return 0;
}

/**
 * @brief			Função que imprime os id dos post de um utilizador.
 * @param use		Apontador para o user.
*/
void print_post_MYUSER(MYUSER use){
	if (!use){
		printf("error\n");
	}
	MYPOST post = NULL;
	MYDATE data;
	int i = 0;
	long k = get_NUM_eleSTACKPOST(use->posts);
	printf("Ne:%ld\n",k);
	for(i = 0; i < k;i++){
		post = get_ele_index_STACKPOST(use->posts,i);
		if (post != NULL){
			data = getDateP(post);
			if (data != NULL){
				printf("Data-> D:%d M:%d A:%d || ID:%ld \n",get_MYday(data),get_MYmonth(data),get_MYyear(data), getIdP(post));
				free_MYdate(data);
			}
		}

	}
	printf("I=%d\n",i);
}

/**
 * @brief 			Função que altera o Id de um user.
 * @param use		Apontador para a struct do user.
 * @param id		Id do user a colocar.
 */
static void setIdUSER(MYUSER use, long id){
    use->id = id;
}

/**
 * @brief 			Função que altera a reputação de um user.
 * @param use		Apontador para a struct do user.
 * @param rep		Reputação do user a colocar.
 */
static void setRp(MYUSER use, int rep){
    use->rep = rep;
}

/**
 * @brief 			Função que altera a biografia de um user.
 * @param use		Apontador para a struct do user.
 * @param bio		Biografia a colocar no user.
 */
static void setBio(MYUSER use, char * bio){
	if (bio)
    	use->bio = mystrdup(bio);
	else use->bio = NULL;
}

/**
 * @brief 			Função que obtém altera o nome de um user.
 * @param use		Apontador para a struct do user.
 * @param nome		Nome do user a colocar.
*/
static void setUsername(MYUSER use, char * nome){
    use->username = mystrdup(nome);
}

/**
 * @brief 			Função que aloca memória para um user.
 * @param type		Tipo a criar.
 * @return 			Apontador para a struct myuser.
 */
MYUSER createMYUSER(int type){
    MYUSER conta = malloc(sizeof(struct myuser));
	conta->bio = NULL;
	conta->username = NULL;
	conta->type = type;
	if (type)
		conta->posts = initSTACKPOST(1);
	else conta->posts = NULL;
    return conta;
}

/**
 * @brief 			Função que clona um user.
 * @param user		Apontador para o utilizador.
 * @return 			Clone da estruturao myuser.
 */
MYUSER cloneMYUSER(MYUSER use){
	MYUSER novo = createMYUSER(0);
	novo->bio = use->bio;
	novo->username = use->username;
	novo->rep = use->rep;
	novo->id = use->id;
	novo->posts = clone_STACKPOST(use->posts);

	return novo;
}

/**
 * @brief				Função que liberta a memória de um user.
 * @param aux			Memória a libertar.
*/
void freeMYUSER(void * aux){
	MYUSER conta;
    if (aux != NULL){
		conta = (MYUSER) aux;
		if (conta->type == 1){
			if (conta->bio)
        		free(conta->bio);
			if (conta->username)
        		free(conta->username);
			freeSTACKPOST_SEM_CLONE(conta->posts);
		}
		else freeSTACKPOST_COM_CLONE(conta->posts);
        free(conta);
    }
}

/**
 * @brief				Função que compara 2 keys de user diferentes.
 * @param key1			Apontador para a primeira key.
 * @param key2			Apontador para a segunda key.
 * @return 				Inteiro a ser usado como booelan.
*/
int compare_user(void * key1,void * key2){
    long id1,id2;
    int result;

    id1 = *((long *) key1);
    id2 = *((long *) key2);

    if (id1 == id2)
        result = 0;
    else result = id2 > id1 ? 1 : -1 ;

    return result;
}

/**
 * @brief				Função que liberta a memória de um key.
 * @param 	a			Apontador para a key.
*/
void freeKey(void * a){
	long * b;
    if (a){
        b = (long *) a;
		free(b);
	}
}

/**
 * @brief				Função que liberta a memória da arvóre alocada.
 * @param	tree 		Apontador para a estrutura da árvore.
*/
void freeTreeUSER(TREE tree){
	freeTREE_AVL(tree);
}

/**
 * @brief				Função que procura um user na estrutura com clone.
 * @param tree			Estrutura da árvore.
 * @param id			Id do user a procurar.
 * @return 				Estrutura clonada apos feita procura, retorna NULL caso a procura falhe.
*/
MYUSER search_USER(TREE tree,long id){
	int valid;

	MYUSER use = search_AVL(tree,&id,&valid);
	if (valid)
		return cloneMYUSER(use);
	return NULL;
}

/**
 * @brief				Função que procura um user na estrutura sem clone.
 * @param tree			Estrutura da árvore.
 * @param id			Id do user a procurar.
 * @return 				Estrutura sem ser clonada apos feita procura, retorna NULL caso a procura falhe.
*/
static MYUSER search_USER_internal(TREE tree,long id){
	int valid;

	MYUSER use = search_AVL(tree,&id,&valid);
	if (valid)
		return use;
	return NULL;
}


/**
 * @brief				Função mete um post no correspondete user.
 * @param tree			Árvore de users.
 * @param id			Identificador do user.
 * @param data			Informação do post.
 * @return 				Inteiro a ser usado como booelan.
*/
int setPostToUSER(TREE tree,long id,MYPOST data){
	MYUSER use;
	if (!getTYPECLONEP(data))
		return -1;
	use = search_USER_internal(tree,id);
	if (use == NULL)
		return -1;
	insereSTACKPOST(use->posts,data);


	return 1;
}

/**
 * @brief				Função lê o ficheiro User.xml e cria uma arvore.
 * @param path 			Path para o ficheiro a dar parse.
 * @return 				Apontador para a arvore criada.
*/
TREE createMYUSERS_TREE(char * path){

    xmlDocPtr doc;
	xmlNodePtr ptr;
	struct _xmlAttr * cur;
	xmlNodePtr aux;

	MYUSER use = NULL;
	TREE tree = createTREE(&compare_user,&freeKey,&freeMYUSER,NULL);

	if (!(xml_file_to_struct(&doc,&ptr,path)))
		return tree;



    long id = 0;
	long * keyid = NULL;
    int rep;

    aux = ptr->children->next;
    xmlChar *key;
	while (aux != NULL){
		if (strcmp((char*)aux->name,"row")==0){
        	use = createMYUSER(1);
			cur = aux->properties;
		 		while (cur != NULL) {
					key = xmlNodeListGetString(doc,cur->children,1);

			    	if (strcmp((char*)cur->name,"Id")==0){
                    	id = atol((char*)key);
                    	setIdUSER(use,id);
                	}
                	if (strcmp((char*)cur->name,"Reputation")==0){
                    	rep = atoi((char*)key);
                    	setRp(use,rep);
                	}
                	if (strcmp((char*)cur->name,"DisplayName")==0){
                    	setUsername(use,(char*)key);
                	}
                	if (strcmp((char*)cur->name,"AboutMe")==0){
						if (key == NULL)
							setBio(use,NULL);
						else setBio(use,(char*)key);
                	}

					xmlFree(key);
					cur = cur->next;
				}
				keyid = malloc(sizeof(long));
				*keyid = id;
				tree = insere_tree(tree,keyid,use);
		}
		aux = aux->next;
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();


	return tree;

}
