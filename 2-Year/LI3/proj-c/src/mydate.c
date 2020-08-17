/**
 * @file 	mydate.c
 * @brief	Ficheiro contendo funções relativa ao modulo MYDATE e funções de transformação de MYDATE para date.
 */
#include "mydate.h"
#include <stdio.h>

struct mydate {
  int day;
  int month;
  int year;
};

/**
 * @brief			Função inicializa a estrutura de data.
 * @param	day		Variável para o dia.
 * @param	month	Variável para o mês.
 * @param	year	Variável para o ano.
 * @return 			Apontador para MYDATE criada.
*/
MYDATE createMYDate(int day, int month, int year) {
    MYDATE d = malloc(sizeof(struct mydate));
    d->day = day;
    d->month = month;
    d->year = year;
    return d;
}

/**
 * @brief			Função que devolve a informação do dia da data.
 * @param	d		Apontador para a data.
 * @return 			Inteiro com o valor do dia.
*/
int get_MYday(MYDATE d) {
    return d->day;
}


/**
 * @brief			Função que devolve a informação do mês da data.
 * @param	d		Apontador para a data.
 * @return 			Inteiro com o valor do mes.
*/
int get_MYmonth(MYDATE d) {
    return d->month; //change to enum? (Acho que sim. Desta forma já garantimos a limitação necessária)
}


/**
 * @brief			Função que devolve a informação do ano da data.
 * @param	d		Apontador para a data.
 * @return 			Inteiro com o valor do ano.
*/
int get_MYyear(MYDATE d) {
    return d->year;
}


/**
 * @brief			Função liberta a memória da data.
 * @param	d		Apontador para a data.
*/
void free_MYdate(MYDATE d) {
	if(d != NULL)
    	free(d);
}

/**
 * @brief			Função calcula o tamanho de uma data.
 * @return 			Tamanho da estrutura .
*/
int MYdate_size() {
	return sizeof(struct mydate);
}

/**
 * @brief			Função compara duas datas para árvores.
 * @param	data1	Apontador para a data 1.
 * @param	data2	Apontador para a data 2.
 * @return 			Inteiro usado como boolean.
*/
int compare_MYDATE_AVL(void * data1, void * data2){

    MYDATE a = (MYDATE) data1;
    MYDATE b = (MYDATE) data2;

    if (a->year == b->year && a->month == b->month && a->day==b->day)
        return 0;
    if (a->year == b->year && a->month == b->month && a->day < b->day)
        return 1;
    if (a->year == b->year && a->month == b->month && a->day > b->day)
        return -1;
    if (a->year == b->year && a->month > b->month)
        return -1;
    if (a->year == b->year && a->month < b->month)
        return 1;
    if (a->year > b->year)
        return -1;

    return 1;

}

/**
 * @brief			Função compara duas datas para listas.
 * @param	data1	Apontador para a data 1.
 * @param	data2	Apontador para a data 2.
 * @return 			Inteiro usado como boolean.
*/
int compare_MYDATE_LIST(void * data1, void * data2){

	MYDATE a = (MYDATE) data1;
	MYDATE b = (MYDATE) data2;

	if (a->year == b->year && a->month == b->month && a->day==b->day)
		return 0;
	if (a->year == b->year && a->month == b->month && a->day < b->day)
		return 0;
	if (a->year == b->year && a->month == b->month && a->day > b->day)
		return 1;
	if (a->year == b->year && a->month > b->month)
		return 1;
	if (a->year == b->year && a->month < b->month)
		return 0;
	if (a->year > b->year)
		return 1;

	return 0;

}

/**
 * @brief			Função converte um MYDATE para Date.
 * @param	a		Apontador para MYDATE.
 * @return 			Apontador para Date.
*/
Date MYDATEtoDate(MYDATE a){
	Date aux;
	aux = createDate(a->day,a->month,a->year);
	return aux;
}

/**
 * @brief			Função converte um Date para MYDATE.
 * @param	a		Apontador para Date.
 * @return 			Apontador para MYDATE.
*/
MYDATE DatetoMYDATE(Date a){
	MYDATE aux;
	int dia,mes,ano;
	dia = get_day(a);
	mes = get_month(a);
	ano = get_year(a);

	aux = createMYDate(dia,mes,ano);

	return aux;
}

/**
 * @brief			Imprimir uma data.
 * @param	d		Apontador para a data.
*/
void printMyDate(MYDATE d){
	printf("(%d-%d-%d)", get_MYday(d), get_MYmonth(d), get_MYyear(d));
}
