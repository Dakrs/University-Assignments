%{
#include "y.tab.h"
#include <ctype.h>
#include <string.h>
%}
%%
(?i:START) 				{return START;}
(?i:NEXT) 				{return NEXT;}
(?i:END) 				{return END;}
(?i:LINK)				{return LINK;}
(?i:LAST)				{return LAST;}
(?i:VIDEO)				{return VIDEO;}
(?i:AUDIO)				{return AUDIO;}
(?i:BODY)				{return BODY;}
(?i:HEAD)				{return HEAD;}
(?i:IMAGEM)				{return IMAGEM;}
(?i:TD)					{return TD;}
(?i:TH)					{return TH;}
(?i:TAB)				{return TAB;}
(?i:ENDTAB)				{return ENDTAB;}
(?i:TITULO)				{return TITULO;}
(?i:LIST)				{return LIST;}
(?i:TEXT)				{return TEXT;}
(?i:BOLD)				{return BOLD;}
(?i:IT)					{return IT;}
(?i:MARK)				{return MARK;}
(?i:EM)					{return EM;}
(?i:STRONG)				{return STRONG;}
(?i:ENDBOLD)			{return ENDBOLD;}
(?i:SMALL)				{return SMALL;}
(?i:ENDSMALL)			{return ENDSMALL;}
(?i:ENDIT)				{return ENDIT;}
(?i:ENDMARK)			{return ENDMARK;}
(?i:ENDEM)				{return ENDEM;}
(?i:ENDSTRONG)			{return ENDSTRONG;}
(?i:ENDTEXT)			{return ENDTEXT;}
(?i:ENDLIST)			{return ENDLIST;}
(?i:BACKGROUND)			{return BACKGROUND;}
(?i:PAGINI)				{return PAGINI;}
[a-zA-Z]+				{yylval.str = strdup(yytext); return WORD; }
[0-9]+      			{yylval.num = atoi(yytext); return NUM;}
[;\:|\{\}\+]			{return yytext[0];}
\'[^\']+\'  			{yytext[yyleng-1] = '\0'; yylval.sentence = strdup(yytext+1); return SENTENCE; }
.|\n {;}
%%

int yywrap() {return 1;}
