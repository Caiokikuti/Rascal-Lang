#include <stdio.h>
#include "util.h"
#include "errormsg.h"
#include "tokens.h"

int yylex(void); /* prototype for the lexing function */

String toknames[] = {
"T_PROGRAM",
"T_VAR",
"T_PROCEDURE",
"T_FUNCTION",
"T_BEGIN",
"T_END",
"T_FALSE",
"T_TRUE",
"T_IF",
"T_THEN",
"T_ELSE",
"T_WHILE",
"T_DO",
"T_READ",
"T_WRITE",
"T_AND",
"T_OR",
"T_NOT",
"T_DIV",
"T_ABRE_PARENTESES",
"T_FECHA_PARENTESES",
"T_PONTO",
"T_VIRGULA",
"T_PONTO_E_VIRGULA",
"T_MAIS",
"T_MENOS",
"T_MULT",
"T_IGUAL",
"T_DIFERENTE",
"T_MAIOR",
"T_MENOR",
"T_MAIOR_IGUAL",
"T_MENOR_IGUAL",
"T_ATRIBUICAO",
"T_DOIS_PONTOS",
"T_IDENT",
"T_NUMERO"
};


String tokname(int tok) {
  return tok<1 || tok>37 ? "BAD_TOKEN" : toknames[tok-1];
}

int main(int argc, char **argv) {
	String fname; int tok;

	if (argc!=2) {fprintf(stderr,"usage: a.out filename\n"); exit(1);}
	fname=argv[1];

	EM_reset(fname);

	for(;;) {
   		tok=yylex();
   		if (tok==0) break;

   		switch(tok) {
   			case T_IDENT: 
     			printf("%10s %4d %s\n",tokname(tok),EM_tokPos,yylval.strval);
        break;
			  case T_NUMERO:
     			printf("%10s %4d %d\n",tokname(tok),EM_tokPos,yylval.intval);
     		break;

   			default:
     			printf("%10s %4d\n",tokname(tok),EM_tokPos);
		}
	}
	return 0;
} 