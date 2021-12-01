%{

  #include <stdio.h>
  #include <string.h>
  #include "includes/util.h"
  #include "includes/errormsg.h"
  #include "sintatico.tab.h"

  #define DEBUG 1

  #ifdef DEBUG
    #define IMPRIME(nome) printf("<%s, '%s'> (linha: %d)\n", nome, yytext, nl);
  #else
    #define IMPRIME(nome) {};
  #endif

  int nl = 1;

  int posicaoAtual = 1;
%}

%option noyywrap

pulo_linha [\n]
branco [\t ]+
ident [a-zA-Z][_a-zA-Z0-9]*
numero [0-9]+

%x COMENTARIO INLINE_COMENTARIO

%%

"program"                       { ajustaPosicao();  return T_PROGRAM; }
"var"                           { ajustaPosicao();  return T_VAR; }
"procedure"                     { ajustaPosicao();  return T_PROCEDURE; }
"function"                      { ajustaPosicao();  return T_FUNCTION; }
"begin"                         { ajustaPosicao();  return T_BEGIN; }
"end"                           { ajustaPosicao();  return T_END; }
"false"                         { ajustaPosicao();  return T_FALSE; }
"true"                          { ajustaPosicao();  return T_TRUE; }
"if"                            { ajustaPosicao();  return T_IF; }
"then"                          { ajustaPosicao();  return T_THEN; }
"else"                          { ajustaPosicao();  return T_ELSE; }
"while"                         { ajustaPosicao();  return T_WHILE; }
"do"                            { ajustaPosicao();  return T_DO; }
"read"                          { ajustaPosicao();  return T_READ; }
"write"                         { ajustaPosicao();  return T_WRITE; }
"and"                           { ajustaPosicao();  return T_AND; }
"or"                            { ajustaPosicao();  return T_OR; }
"not"                           { ajustaPosicao();  return T_NOT; }
"div"                           { ajustaPosicao();  return T_DIV; }
"("                             { ajustaPosicao();  return T_ABRE_PARENTESES; }
")"                             { ajustaPosicao();  return T_FECHA_PARENTESES; }
"."                             { ajustaPosicao();  return T_PONTO; }
","                             { ajustaPosicao();  return T_VIRGULA; }
";"                             { ajustaPosicao();  return T_PONTO_E_VIRGULA; }
"+"                             { ajustaPosicao();  return T_MAIS; }
"-"                             { ajustaPosicao();  return T_MENOS; }
"*"                             { ajustaPosicao();  return T_MULT; }
"="                             { ajustaPosicao();  return T_IGUAL; }
"<>"                            { ajustaPosicao();  return T_DIFERENTE; }
">"                             { ajustaPosicao();  return T_MAIOR; }
"<"                             { ajustaPosicao();  return T_MENOR; }
">="                            { ajustaPosicao();  return T_MAIOR_IGUAL; }
"<="                            { ajustaPosicao();  return T_MENOR_IGUAL; }
":="                            { ajustaPosicao();  return T_ATRIBUICAO; }
":"                             { ajustaPosicao();  return T_DOIS_PONTOS; }

{ident}                         { ajustaPosicao();  yylval.str = string(yytext); return T_IDENT; }
{numero}                        { ajustaPosicao();  yylval.num = atoi(yytext); return T_NUMERO; }

{branco}                        { ajustaPosicao(); }
{pulo_linha}                    { ajustaPosicao(); nl++; }

"//"                            { ajustaPosicao(); BEGIN(INLINE_COMENTARIO); }
<INLINE_COMENTARIO>{pulo_linha} { ajustaPosicao(); nl++; BEGIN(INITIAL); }
<INLINE_COMENTARIO>.            { ajustaPosicao(); continue; }

"{"                             { ajustaPosicao(); BEGIN(COMENTARIO); }
<COMENTARIO><<EOF>>             { ajustaPosicao(); fprintf(stderr, "Comentario nao fechado\n"); yyterminate(); }
<COMENTARIO>"}"                 { ajustaPosicao(); BEGIN(INITIAL); }
<COMENTARIO>.                   { ajustaPosicao(); continue; }
<COMENTARIO>{pulo_linha}        { ajustaPosicao(); nl++; }

.                               { ajustaPosicao(); fprintf(stderr, "Erro: Token ilegal: '%s'\n", yytext); }
%%

void ajustaPosicao(void) {
	EM_tokPos = posicaoAtual;
	posicaoAtual += yyleng;
}
