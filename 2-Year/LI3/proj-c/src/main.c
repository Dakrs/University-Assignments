	//#include <date.h>
	#include <stdio.h>
	#include <myuser.h>
	#include <mypost.h>
	#include <loading.h>
	#include <interface.h>
	#include <time.h>

	int inited 	= 0;
	int loaded 	= 0;
	char* ambA = "/Users/Ambrosiny/Desktop/dump/android";
	char* ambU = "/Users/Ambrosiny/Desktop/dump/ubuntu";
	char* randA = "/home/pedro/Desktop/dump/android";
	char* randU = "/home/pedro/Desktop/dump/ubuntu";
	char* megaA = "/home/alexrone/a/android";
	char* megaU = "/home/alexrone/a/ubuntu";


	char *gets(char *str);


int main(){
		char string[256];
		char string2[256];
		long* auxll;
		TAD_community com = NULL;
		clock_t t;
		long long1,long2;
		int int1,int2,int3,int4,i, z;
		STR_pair stp = NULL;
		LONG_list llis = NULL;
		USER user = NULL;
		LONG_pair lp = NULL;
		Date ini = NULL;
		Date fi = NULL;
		char* auxstring;
		char * auxstring2;
		while(1){
			gets(string);


			if(strcmp(string,"exit")==0){
				if (loaded != 0){
					printf("Falta dar clean(free) a struct \n");
					continue;
				}
				free(com);
				exit(0);
			}
			if(strcmp(string,"load") == 0|| strcmp(string,"q0") == 0){
				if(inited != 1){
					com = init();
					inited = 1;
					printf("Init done\n\n");
				}
				printf("Escrever Path\nExemplos \n1->%s \n2->%s \n3->%s \n4->%s \n5->%s \n6->%s \n",ambA,randA,megaA,ambU,randU,megaU);
				gets(string);
				int1 = atoi(string);


				printf("A dar load plz wait bitch\n\n");
				t = clock();



				if (int1 == 1)
					com = load(com,"/Users/Ambrosiny/Desktop/dump/android");
				else if (int1 == 2)
					com = load(com,"/home/pedro/Desktop/dump/android");
				else if (int1 == 3)
					com = load(com,"/home/alexrone/a/android");
				else if (int1 == 4)
					com = load(com,"/Users/Ambrosiny/Desktop/dump/ubuntu");
				else if (int1 == 5)
					com = load(com,"/home/pedro/Desktop/dump/ubuntu");
				else if (int1 == 6)
					com = load(com,"/home/alexrone/a/ubuntu");
				t = clock()-t;

				loaded = 1;
				printf("LOAD done\n\n");

				double a5 = ((double)t)/CLOCKS_PER_SEC;
				printf("LOAD->%fs\n\n\n",a5);



				continue;
			}
			if(strcmp(string,"q1")== 0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("Escrever id do Post\n");
				gets(string);
				long1 = atol(string);
				t = clock();
				stp = info_from_post(com, long1);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
				auxstring =get_fst_str(stp);
				auxstring2 = get_snd_str(stp);
				printf("Titulo %s  ||||| User  %s\n\n",auxstring,auxstring2);
				free(auxstring);
				free(auxstring2);
				free_str_pair(stp);
				printf("Q1->%fms\n\n\n",a5);
				continue;

			}
			if(strcmp(string,"q2")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("N max\n");
				gets(string);
				int1 = atoi(string);
				t = clock();
				llis = top_most_active(com, int1);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
							if(llis)
				for(i = 0; i < get_listsize(llis);i++)
					printf("%d -> %ld\n",i,get_list(llis,i) );
				printf("Q2->%fms\n\n\n",a5);
				free_list(llis);
				continue;

			}
			if(strcmp(string,"q3")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("Data inicial do tipo dia-mes-ano\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				ini = createDate(int1,int2,int3);
				printf("Data final\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				fi = createDate(int1,int2,int3);
				t = clock();
				lp = total_posts(com,ini,fi);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
					printf("Perguntas %ld ||||  Respotas  %ld\n\n",get_fst_long(lp),get_snd_long(lp));
				printf("Q3->%fms\n\n\n",a5);
				free_long_pair(lp);
				free_date(ini);

				free_date(fi);

				continue;

			}
			if(strcmp(string,"q4")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("Tag a verificar\n");
				gets(string2);
				printf("Data inicial do tipo dia-mes-ano\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				ini = createDate(int1,int2,int3);
				printf("Data final\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				fi = createDate(int1,int2,int3);
				t = clock();
				llis = questions_with_tag(com,string2,ini,fi);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
				if(llis)
				for(i = 0; i < get_listsize(llis);i++)
					printf("%d -> %ld\n",i,get_list(llis,i) );
				printf("Q4->%fms\n\n\n",a5);
				free_list(llis);
				free_date(ini);

				free_date(fi);

				continue;

			}
			if(strcmp(string,"q5")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("ID do user\n");
				gets(string);
				long1 = atol(string);
				t = clock();
				user = get_user_info(com,long1);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
				auxstring = get_bio(user);
				printf("User = %s   ",auxstring);
			//	free(auxstring);
				if(user){
					auxll = get_10_latest_posts(user);
				for(i=0;i<10;i++)
					printf("%d -> %ld\n",i,auxll[i] );
				free(auxll);
				}
				printf("Q5->%fms\n\n\n",a5);
				free_user(user);
				continue;
			}
			if(strcmp(string,"q6")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("N max\n");
				gets(string);
				int4 = atoi(string);

				printf("Data inicial do tipo dia-mes-ano\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				ini = createDate(int1,int2,int3);
				printf("Data final\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				fi = createDate(int1,int2,int3);
				t = clock();
				llis = most_voted_answers(com,int4,ini,fi);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
							if(llis)
				for(i = 0; i < get_listsize(llis);i++)
					printf("%d -> %ld\n",i,get_list(llis,i) );
				printf("Q6->%fms\n\n\n",a5);
				free_date(ini);
				free_list(llis);
				free_date(fi);

				continue;
			}
			if(strcmp(string,"q7")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("N MAX\n");
				gets(string);
				int4 = atoi(string);

				printf("Data inicial do tipo dia-mes-ano\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				ini = createDate(int1,int2,int3);
				printf("Data final\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				fi = createDate(int1,int2,int3);
				t = clock();
				llis = most_answered_questions(com,int4,ini,fi);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
							if(llis)
				for(i = 0; i < get_listsize(llis);i++)
					printf("%d -> %ld\n",i,get_list(llis,i) );
				printf("Q7->%fms\n\n\n",a5);
				free_date(ini);
				free_date(fi);
				free_list(llis);
				continue;
			}
			if(strcmp(string,"q8")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("N MAX\n");
				gets(string);
				int1 = atoi(string);
				printf("Palavra a verificar\n");
				gets(string);
				t = clock();
				llis = contains_word(com,string,int1);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
							if(llis)
				for(i = 0; i < get_listsize(llis);i++)
					printf("%d -> %ld\n",i,get_list(llis,i) );
				printf("Q8->%fms\n\n\n",a5);
				free_list(llis);
				continue;
			}
			if(strcmp(string,"q9")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("Escrever id1\n");
				gets(string);
				long1 = atol(string);
				printf("Escrever id2\n");
				gets(string);
				long2 = atol(string);
				printf("N MAX\n");
				gets(string);
				int1 = atoi(string);
				t = clock();
				llis = both_participated(com, long1,long2,int1);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
											if(llis)
				for(i = 0; i < get_listsize(llis);i++)
							if(llis)
					printf("%d -> %ld\n",i,get_list(llis,i) );
				printf("Q9->%fms\n\n\n",a5);
				free_list(llis);
				continue;
			}
			if(strcmp(string,"q10")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("ID DO POST\n");
				gets(string);
				long1 = atol(string);
				t = clock();
				long2 = better_answer(com,long1);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
				printf("R = %ld\n",long2 );
				printf("Q10->%fms\n\n\n",a5);
				continue;
			}
			if(strcmp(string,"q11")==0){
				if(loaded != 1){
					printf("Falta dar load\n");
					continue;
				}
				printf("N MAX\n");
				gets(string);
				int4 = atoi(string);
				printf("Data inicial do tipo dia-mes-ano\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				ini = createDate(int1,int2,int3);
				printf("Data final\n");
				z = scanf("%i-%i-%i",&int1,&int2,&int3);
				fi = createDate(int1,int2,int3);

				t = clock();
				llis = most_used_best_rep(com,int4,ini,fi);
				t = clock()-t;
				double a5 = ((double)t)/CLOCKS_PER_SEC*1000;
				for(i = 0; i < get_listsize(llis);i++)
							if(llis)
					printf("%d -> %ld\n",i,get_list(llis,i) );
				printf("Q11->%fms\n\n\n",a5);
				free_date(ini);
				free_list(llis);
				free_date(fi);

				continue;
			}
			if(strcmp(string,"clean")==0){
				com = clean(com);
				loaded = 0;
				printf("clean(free) done\n\n");

			}





			printf("NOPE\n");
		}

		z++;

}



/*
	int main1(){
		TAD_community com = init();
		com = load(com,"/Users/Ambrosiny/Desktop/dump exemplo/android");
		Date ini ,fi;
		ini = createDate(1,1,1);
		fi = createDate(1,1,3000);
		LONG_list llis;
		llis = questions_with_tag(com,"rooting",ini,fi);
		com = clean(com);
		free(com);

		return 1;
	}
*/
