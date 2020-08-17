/**
 * @file 	myheap.c
 * @brief	Ficheiro contendo funções utilizadas na construção de uma heap e de uma stack utilizadas no programa bem como todas as funcionalidades pelas mesmas suportadas.
 */
#include "myheap.h"


#define PARENT(n) (n-1)/2
#define LCHILD(n) (2*n +1)
#define RCHILD(n) (2*n +2)

struct generico {
	long key;
	long data;
};

struct heap{
	long size;
	long n_elem;
	GEN * array;
};

struct stack{
	long size;
	long n_elem;
	long * array;
};

/**
 * @brief			Função troca 2 elementos no array.
 * @param	v 		Array generico.
 * @param	i		Posição do 1 elemento.
 * @param 	d		Posição do 2 elemento.
*/
void swap(GEN * v,int i, int d){
	long aux1 = v[i].key;
	long aux2 = v[i].data;

	v[i].key = v[d].key;
	v[i].data = v[d].data;

	v[d].key = aux1;
	v[d].data = aux2;
}

/**
 * @brief			Função que inicializa um heap.
 * @param size		Tamanho original da heap.
 * @return 			Apontador para a HEAP.
*/
HEAP initHEAP(long size){
	HEAP aux = malloc(sizeof(struct heap));
	aux->size = size;
	aux->n_elem = 0;
	aux->array = malloc(size*sizeof(struct generico));

	return aux;
}

/**
 * @brief			Função que coloca o último elemento adicionado na sua posição correta.
 * @param main		Apontador para a heap.
*/
void bubble_up(HEAP * main){
	HEAP h = * main;
	int pai,i;
	if (h->n_elem > 0){
		i = h->n_elem -1;
		pai = PARENT(i);
		while((h->array[i].key>=h->array[pai].key || h->array[i].data<=h->array[pai].data) && i != pai){
			if (h->array[i].key>h->array[pai].key){
				swap(h->array,pai,i);
				i = pai;
				pai = PARENT(i);
			}
			else {
				if (h->array[i].data < h->array[pai].data && h->array[i].key == h->array[pai].key){
					swap(h->array,pai,i);
					i = pai;
					pai = PARENT(i);
				}
				else break;
			}
		}
	}
	*main = h;
}

/**
 * @brief			Função que realiza as operações necessárias depois de se retirar um elemento.
 * @param main		Apontador para a heap.
*/
void bubble_down(HEAP * main){
	HEAP h = * main;
	int fd,fe,i = 0;
	if (h->n_elem > 0){
		h->n_elem--;
		h->array[i].key = h->array[h->n_elem].key;
		h->array[i].data = h->array[h->n_elem].data;
		fd = RCHILD(i);
		fe = LCHILD(i);

		while(i < h->n_elem && fd < h->n_elem && fe < h->n_elem &&
											(	(h->array[i].key < h->array[fd].key) ||
								 				(h->array[i].key < h->array[fe].key) ||
												(h->array[i].data > h->array[fd].data && h->array[i].key == h->array[fe].key) ||
												(h->array[i].data > h->array[fe].data && h->array[i].key == h->array[fe].key) )
			){
			if (fd < h->n_elem && fe < h->n_elem){
				if (h->array[fe].key > h->array[fd].key){
					swap(h->array,i,fe);
					i = fe;
				}
				else if(h->array[fe].key < h->array[fd].key){
					swap(h->array,i,fd);
					i = fd;
				}
				else{
					if (h->array[fd].data < h->array[fe].data && h->array[fe].key == h->array[fd].key){
						swap(h->array,i,fd);
						i = fd;
					}
					else {
						swap(h->array,i,fe);
						i = fe;
					}
				}
			}
			else if (fd < h->n_elem){
				swap(h->array,i,fd);
				i = fd;
			}
			else {
				swap(h->array,i,fe);
				i = fe;
			}
			fd = RCHILD(i);
			fe = LCHILD(i);
		}
	}
	*main = h;
}

/**
 * @brief			Função que insere um novo elemento na heap.
 * @param h			Heap.
 * @param key		Valor da key a inserir.
 * @param data		Valor da data a inserir.
*/
void insereHEAP(HEAP h, long key, long data){
	if (h->n_elem >= h->size){
		h->array = realloc(h->array,sizeof(struct generico)*h->size*2);
		h->size *= 2;
	}
	h->array[h->n_elem].key = key;
	h->array[h->n_elem].data = data;
	h->n_elem++;
	bubble_up(&h);

}

/**
 * @brief			Função que retira um elemento na heap.
 * @param h			Heap.
 * @param key		Apontador para o sítio onde a key fica guardada.
 * @param data		Apontador para o sítio onde a data fica guardada.
*/
void pop (HEAP h, long * key, long * data){
	if (h->n_elem > 0){
		*key =h->array[0].key;
		*data = h->array[0].data;
		bubble_down(&h);
	}
}

/**
 * @brief			Função dá free à heap.
 * @param	h		Heap.
*/
void freeMYHEAP(HEAP h){
	if (h){
		free(h->array);
		free(h);
	}
}

/**
 * @brief			Função que inicializa uma stack.
 * @param size		Tamanho da stack a inicializar.
 * @return 			Apontador para STACK inicializa.
*/
STACK initSTACK(long size){
	STACK aux = malloc(sizeof(struct stack));
	aux->n_elem = 0;
	aux->size = size;
	aux->array = malloc(size*sizeof(long));

	return aux;
}

/**
 * @brief			Função dá free à stack.
 * @param	a		Stack.
*/
void freeSTACK(STACK a){
	if(a){
		free(a->array);
		free(a);
	}
}

/**
 * @brief			Função insere um elemento na stack.
 * @param	a		Stack.
 * @param	id		Valor a inserir.
 * @return 			Apontador para a stack apos ser inserido o elemento, returna NULL caso a stack seja nula.
*/
STACK insereSTACK(STACK a,long id){
	if (!a)
		return NULL;

	if (a->size == a->n_elem){
		a->array = realloc(a->array,sizeof(long)*a->size*2);
		a->size *= 2;
	}
	a->array[a->n_elem++] = id;

	return a;
}

/**
 * @brief			Função que calcula o número de elementos na stack.
 * @param	a		Stack.
 * @return 			Número de elementos da stack.
*/
long get_NUM_eleSTACK(STACK a){
	if(a){
		return a->n_elem;
	}
	return 0;
}

/**
 * @brief			Função que devolve o elemento numa dada posição.
 * @param	a		Stack.
 * @param	index	Indíce.
 * @return 			Elemento na posicao dada.
*/
long get_ELE_index(STACK a,int index){
	if ((!a) || a->n_elem <= index)
		return -1;
	return a->array[index];
}

/**
 * @brief			Função que calcula o número de elementos na heap.
 * @param	a		HEAP.
 * @return 			Numero de elementos na heap.
*/
long get_NUM_eleHEAP(HEAP a){
	long x;
	return (x = a ? a->n_elem : 0);
}


/**
 * @brief			Função percorre uma STACK de longs e aplica a cada nodo uma função.
 * @param	arr		STACK  a ser percorrida.
 * @param	f_box	Função a ser aplicada (cujo o primeiro argumento é o nodo a que ela é aplicada).
 * @param	data1	Segundo argumento da função a ser aplicada.
 * @param	data2	Terceiro argumento da função a ser aplicada.
 * @param	data3	Quarto argumento da função a ser aplicada.
*/
void trans_stack(STACK arr, void (*f_box)(long , void *, void *, void *), void * data1, void * data2, void * data3){
	if(arr == NULL)
		return;

	for(int i = 0;i < arr->n_elem; i++)
		f_box(arr->array[i],data1, data2, data3);
}
