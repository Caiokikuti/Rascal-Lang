%code requires {
#include "util.h"
#include "ast.h"
}

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "ast.h"

/* Esta é a função que opera o Analisador Léxico (AL) e já foi construída pelo Flex
   dentro dele. Como o Analisador Sintático (AS) é quem irá chamá-la no decorrer da análise, 
   ela também precisa estar declarada aqui.
*/
int yylex(void);

/* Esta função será executada caso algum erro sintático ocorra. 
   Caso esta função não tivesse sido definida pelo usuário, o Bison iria construí-la
   praticamente da forma com que a definimos aqui. Essa é uma oportunidade
   de customizá-la para as suas necessidades. Talvez criar um módulo único e independente
   para controlar as mensagens de erro também seja interessante.
*/
void yyerror(const char *s) {
  fprintf(stderr, "Erro sintático: %s", s);
}

extern A_Programa raiz_ast;

%}

/* A definição abaixo cria um tipo de dados (no formato de uma union)
   para os valores semânticos que podem ser associados aos símbolos da
   gramática, tanto terminais (tokens) quanto não-terminais.
   No caso dos tokens, o valor semântico pode ser preenchido durante a 
   própria análise léxica, por meio da variável global yylval.
   Novos campos deverão ser adicionados nesta union, permitindo
   armazenar os valores semânticos de outros tipos de tokens ou de símbolos
   não terminais da gramática. 
*/
%union {
   String str;
   int num;
   A_Programa programa;
   A_Bloco bloco;
   A_LstDecSub secDecVar;
   A_LstDecVar secDecSub;
   A_CmdComp cmdComp;
}

/* Os nomes associados aos tokens definidos aqui serão armazenados um uma 
   enum dentro do arquivo compilador.tab.h. Este arquivo deve ser incluido 
   no código do analisador léxico para que os tokens retornados por ele tenham 
   exatamente os mesmos códigos numéricos que foram associados pela enum.
*/

%token T_PROGRAM
%token T_BEGIN
%token T_END
%token T_PROCEDURE
%token T_FUNCTION
%token T_FALSE T_TRUE
%token T_IF T_THEN T_ELSE T_WHILE T_DO
%token T_READ T_WRITE
%token T_AND T_OR T_NOT
%token T_DIV T_MAIS T_MENOS T_MULT T_IGUAL T_DIFERENTE
%token T_MAIOR T_MENOR T_MAIOR_IGUAL T_MENOR_IGUAL
%token T_VAR
%token T_ABRE_PARENTESES T_FECHA_PARENTESES
%token T_ATRIBUICAO
%token T_NUMERO
%token T_VIRGULA T_PONTO_E_VIRGULA T_DOIS_PONTOS T_PONTO

%token <str> T_IDENT 
/* str é o nome do campo semântico que será utilizado pelo token T_IDENT (identificador).
   Este campo foi definido na union acima e seu objetivo é armazenar uma
   string (char *), que no nosso caso será o valor semântido do identificador
   (seu próprio lexema), o qual foi carregado pelo analizador léxico por meio
   da ação que construimos: yylval.str = String(yytext);
   A definição do token T_NUMERO seguirá a mesma ideia, mas seu valor semântico se refere
   ao campo num da union.
*/

/* Também podemos associar uma string literal ao nome de um token escrevendo esta string no fim
   da declaração %token. Com isso você poderá utilizar a string associada na gramática em vez
   de ter que escrever o nome do token. Por exemplo, o token T_PROGRAM poderia ser definido assim:
   
   %token T_PROGRAM "program"
   
   Dessa forma, podemos referenciar este token dentro da gramática como "program" em vez de T_PROGRAM.
   
   Veja mais em:
   https://www.gnu.org/software/bison/manual/html_node/Token-Decl.html
*/

%type <programa> programa
%type <secDecVar> secao_declara_vars
%type <secDecSub> secao_declara_subrotinas
%type <cmdComp> comando_composto declara_vars declara_var lista_identificadores declara_proced proced declara_func func parametros_formais parametros declara_parametros comandos comando atribuicao chamada_procedimento condicional repeticao leitura escrita lista_expressoes expressao relacao expressao_simples termo fator variavel logico chamada_funcao 
%type <bloco> bloco
%type <str> tipo

%define parse.error verbose
%define parse.lac full

%start programa

%%

programa: T_PROGRAM T_IDENT T_PONTO_E_VIRGULA bloco T_PONTO { $$ = NULL }
;

bloco: secao_declara_vars secao_declara_subrotinas comando_composto { $$ = NULL }
;

secao_declara_vars: T_VAR declara_vars T_PONTO_E_VIRGULA { $$ = NULL; }
                  | secao_declara_vars declara_vars T_PONTO_E_VIRGULA { $$ = NULL; }
                  | /* vazio */ { $$ = NULL; }
;

declara_vars: declara_vars declara_var { $$ = NULL; }
            | declara_var { $$ = NULL; }
;

declara_var: lista_identificadores T_DOIS_PONTOS tipo { $$ = NULL; }
;

lista_identificadores: lista_identificadores T_VIRGULA T_IDENT { $$ = NULL; }
           | T_IDENT { $$ = NULL; }
;

tipo: T_IDENT { $$ = $1; } /* caso não fosse especificada, esta já seria a ação default */
;

secao_declara_subrotinas: declara_proced T_PONTO_E_VIRGULA { $$ = NULL; }
                        | declara_func T_PONTO_E_VIRGULA { $$ = NULL; }
                        | /* vazio */ { $$ = NULL; }
;

declara_proced: proced T_PONTO_E_VIRGULA bloco { $$ = NULL; }
;

proced: T_PROCEDURE T_IDENT { $$ = NULL; }
      | T_PROCEDURE T_IDENT parametros_formais { $$ = NULL; }
;

declara_func: func T_DOIS_PONTOS tipo T_PONTO_E_VIRGULA bloco { $$ = NULL; }
;

func: T_FUNCTION T_IDENT { $$ = NULL; }
    | T_FUNCTION T_IDENT parametros_formais { $$ = NULL; }
;

parametros_formais: T_ABRE_PARENTESES parametros T_FECHA_PARENTESES { $$ = NULL; }
;

parametros: parametros T_PONTO_E_VIRGULA declara_parametros { $$ = NULL; }
          | declara_parametros { $$ = NULL; }
;

declara_parametros: lista_identificadores T_DOIS_PONTOS tipo { $$ = NULL; }
                  | T_VAR lista_identificadores T_DOIS_PONTOS tipo { $$ = NULL; }
;

comando_composto: T_BEGIN comandos T_END { $$ = NULL; }
;

comandos: comandos T_PONTO_E_VIRGULA comando { $$ = NULL; }
        | comando { $$ = NULL; }
;

comando: atribuicao { $$ = NULL; }
        | chamada_procedimento { $$ = NULL; }
        | condicional { $$ = NULL; }
        | repeticao { $$ = NULL; }
        | leitura { $$ = NULL; }
        | escrita { $$ = NULL; }
        | comando_composto { $$ = NULL; }
;

atribuicao: T_IDENT T_ATRIBUICAO expressao { $$ = NULL; }
;

chamada_procedimento: T_IDENT { $$ = NULL; }
                    | T_IDENT T_ABRE_PARENTESES lista_expressoes T_FECHA_PARENTESES { $$ = NULL; }
;

condicional: T_IF expressao T_THEN comando { $$ = NULL; }
          | T_IF expressao T_THEN comando T_ELSE comando { $$ = NULL; }
;

repeticao: T_WHILE expressao T_DO comando { $$ = NULL; }
;

leitura: T_READ T_ABRE_PARENTESES lista_identificadores T_FECHA_PARENTESES { $$ = NULL; }
;

escrita: T_WRITE T_ABRE_PARENTESES lista_expressoes T_FECHA_PARENTESES { $$ = NULL; }
;

lista_expressoes: lista_expressoes T_VIRGULA expressao { $$ = NULL; }
                | expressao { $$ = NULL; }
;

expressao: expressao_simples { $$ = NULL; }
        | expressao_simples relacao expressao_simples { $$ = NULL; }
;

relacao: T_IGUAL { $$ = NULL; }
      | T_DIFERENTE { $$ = NULL; }
      | T_MENOR { $$ = NULL; }
      | T_MENOR_IGUAL { $$ = NULL; }
      | T_MAIOR { $$ = NULL; }
      | T_MAIOR_IGUAL { $$ = NULL; }
;

expressao_simples: expressao_simples T_MAIS termo { $$ = NULL; }
                | expressao_simples T_MENOS termo { $$ = NULL; }
                | expressao_simples T_OR termo { $$ = NULL; }
                | termo { $$ = NULL; }
;

termo: termo T_MULT fator { $$ = NULL; }
    | termo T_DIV fator { $$ = NULL; }
    | termo T_AND fator { $$ = NULL; }
    | fator { $$ = NULL; }
;

fator: variavel { $$ = NULL; }
      | T_NUMERO { $$ = NULL; }
      | logico { $$ = NULL; }
      | chamada_funcao { $$ = NULL; }
      | T_ABRE_PARENTESES expressao T_FECHA_PARENTESES { $$ = NULL; }
      | T_NOT fator { $$ = NULL; }
      | T_MENOS fator { $$ = NULL; }
;

variavel: T_IDENT { $$ = NULL; }
;

logico: T_FALSE { $$ = NULL; }
      | T_TRUE { $$ = NULL; }
;

chamada_funcao: T_IDENT T_ABRE_PARENTESES lista_expressoes T_FECHA_PARENTESES { $$ = NULL; }
;

%%

/* Aqui poderia ser construída a função main com a lógica do compilador, que
   executaria a análise sintática e seguiria com o processo de compilação.
   Entretanto, para deixar nosso código mais organizado, o programa principal
   foi construído separadamente no arquivo compilador.c
*/