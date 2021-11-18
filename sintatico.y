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

extern A_programa raiz_ast;

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
  A_programa programa;
  A_bloco bloco;
  A_lstDecVar lstDecVar;
  A_lstIdent lstIdent;
  A_lstDecFunc lstDecFunc;
  A_lstDecProc lstDecProc;
  A_lstDecSub lstDecSub;
  A_CmdComp cmdComp;
  A_exp exp;
  A_lstExp lstExp;
  A_dec dec;
  A_var var;
  A_funcDec funcDec;
  A_procDec procDec;
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
%token T_VIRGULA T_PONTO_E_VIRGULA T_DOIS_PONTOS T_PONTO

%token <str> T_IDENT 
%token <num> T_NUMERO 
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

%nonassoc T_THEN T_DO T_FUNCTION T_IDENT
%nonassoc T_ATRIBUICAO T_ABRE_PARENTESES ELSE T_VIRGULA
%left T_OR
%left T_AND
%nonassoc T_IGUAL T_DIFERENTE T_MENOR_IGUAL T_MENOR T_MAIOR T_MAIOR_IGUAL
%left T_MAIS T_MENOS
%left T_MULT T_DIV

%type <programa> programa
%type <lstDecVar> secao_declara_vars lista_declara_vars declara_vars parametros parametros_formais declara_parametros
%type <lstIdent> lista_ident
%type <lstDecSub> secao_declara_subs list_declara_subs
%type <lstDecFunc> list_declara_funcs
%type <lstDecProc> list_declara_procs
// %type <cmdComp> comando_composto
%type <bloco> bloco
%type <str> tipo
%type <procDec> declara_proced
%type <funcDec> declara_func
%type <var> variavel
%type <exp> expressao expressao_simples termo fator chamada_func chamada_proc atribuicao comando logico leitura escrita
%type <lstExp> lista_expressoes comandos comando_composto 

%define parse.error verbose
%define parse.lac full

%start programa

%%

programa: T_PROGRAM T_IDENT T_PONTO_E_VIRGULA bloco T_PONTO { raiz_ast = A_Programa($2, $4); }
;

bloco: secao_declara_vars secao_declara_subs comando_composto { $$ = A_Bloco($1, $2, $3); }
;

secao_declara_vars: T_VAR lista_declara_vars { $$ = $2; }
                  | /* vazio */ { $$ = NULL; }
;

lista_declara_vars: declara_vars lista_declara_vars { $$ = concatLstDecVar($1, $2); }
                  | declara_vars                    { $$ = $1; }
;

declara_vars: lista_ident T_DOIS_PONTOS tipo T_PONTO_E_VIRGULA { 
                                                                  String tipo = $3;
                                                                  A_lstDecVar lstDecVar = NULL;
                                                                  A_lstIdent lstIdent = $1;
                                                                  
                                                                  while (lstIdent != NULL) {
                                                                     lstDecVar = A_LstDecVar(A_DecVar(lstIdent->id, tipo), lstDecVar);
                                                                     lstIdent = lstIdent->prox;
                                                                  }
                                                                  
                                                                  $$ = lstDecVar;
                                                               }
;

parametros_formais: /* vazio */ { $$ = NULL; }
                  | parametros { $$ = $1; }
;

parametros: parametros T_PONTO_E_VIRGULA declara_parametros { $$ = concatLstDecVar($1, $3); }
          | declara_parametros { $$ = $1; }
;

declara_parametros: lista_ident T_DOIS_PONTOS tipo { 
                                                      String tipo = $3;
                                                      A_lstDecVar lstDecVar = NULL;
                                                      A_lstIdent lstIdent = $1;
                                                      
                                                      while (lstIdent != NULL) {
                                                          lstDecVar = A_LstDecVar(A_DecVar(lstIdent->id, tipo), lstDecVar);
                                                          lstIdent = lstIdent->prox;
                                                      }
                                                      
                                                      $$ = lstDecVar;
                                                    }
                  | T_VAR lista_ident T_DOIS_PONTOS tipo { $$ = NULL; }
;

lista_ident: lista_ident T_VIRGULA T_IDENT { $$ = A_LstIdent($3, $1); }
           | T_IDENT                       { $$ = A_LstIdent($1, NULL); }
;

tipo: T_IDENT { $$ = $1; }
;

secao_declara_subs: list_declara_subs { $$ = $1; }
                  | /* vazio */ { $$ = NULL; }
;

list_declara_subs: list_declara_funcs list_declara_procs { $$ = A_LstSubDec($1, $2); }
                  | list_declara_funcs { $$ = A_LstSubDec($1, NULL); }
                  | list_declara_procs { $$ = A_LstSubDec(NULL, $1); }
;

list_declara_funcs: declara_func T_PONTO_E_VIRGULA list_declara_funcs { $$ = A_LstFuncDec($1, $3); }
                  | declara_func T_PONTO_E_VIRGULA  { $$ = A_LstFuncDec($1, NULL); }
;

list_declara_procs: declara_proced T_PONTO_E_VIRGULA list_declara_procs { $$ = A_LstProcDec($1, $3); }
                  | declara_proced T_PONTO_E_VIRGULA  { $$ = A_LstProcDec($1, NULL); }
;

declara_proced: T_PROCEDURE T_IDENT T_ABRE_PARENTESES parametros_formais T_FECHA_PARENTESES T_PONTO_E_VIRGULA bloco { $$ = A_ProcDec($2, $4, $7); }
;

declara_func: T_FUNCTION T_IDENT T_ABRE_PARENTESES parametros_formais T_FECHA_PARENTESES T_DOIS_PONTOS tipo T_PONTO_E_VIRGULA bloco { $$ = A_FuncDec($2, $4, $7, $9); }
;

comando_composto: T_BEGIN comandos T_END { $$ = $2; }
;

comandos: comando T_PONTO_E_VIRGULA { $$ = A_LstExp($1, NULL); }
        | comando T_PONTO_E_VIRGULA comandos { $$ = A_LstExp($1, $3); }
;

comando: atribuicao { $$ = $1; }
        | chamada_proc { $$ = $1; }
        | leitura { $$ = $1; }
        | escrita { $$ = $1; }
;
        // | if { $$ = NULL; }
        // | while { $$ = NULL; }

atribuicao: T_IDENT T_ATRIBUICAO expressao { $$ = A_AtribExp(A_Var($1), $3); }
;

chamada_proc: T_IDENT T_ABRE_PARENTESES lista_expressoes T_FECHA_PARENTESES { $$ = A_ChamaProcExp($1, $3); }
;

// if: T_IF expressao T_THEN comando { $$ = NULL; }
//   | T_IF expressao T_THEN comando T_ELSE comando { $$ = NULL; }
// ;

// while: T_WHILE expressao T_DO comando { $$ = NULL; }
// ;

leitura: T_READ T_ABRE_PARENTESES lista_ident T_FECHA_PARENTESES { $$ = A_LeituraExp($3); }
;

escrita: T_WRITE T_ABRE_PARENTESES lista_expressoes T_FECHA_PARENTESES { $$ = A_EscritaExp($3); }
;

lista_expressoes: expressao { $$ = A_LstExp($1, NULL); }
                | lista_expressoes T_VIRGULA expressao { $$ = A_LstExp($3, $1); }
                | /* vazio */ { $$ = NULL; }
;

expressao: expressao_simples { $$ = $1; }
          | expressao_simples T_IGUAL expressao_simples { $$ = A_OpExp(A_eqOp, $1, $3); }
          | expressao_simples T_DIFERENTE expressao_simples { $$ = A_OpExp(A_neqOp, $1, $3); }
          | expressao_simples T_MENOR expressao_simples { $$ = A_OpExp(A_ltOp, $1, $3); }
          | expressao_simples T_MENOR_IGUAL expressao_simples { $$ = A_OpExp(A_leOp, $1, $3); }
          | expressao_simples T_MAIOR expressao_simples { $$ = A_OpExp(A_gtOp, $1, $3); }
          | expressao_simples T_MAIOR_IGUAL expressao_simples { $$ = A_OpExp(A_geOp, $1, $3); }
;

expressao_simples: termo { $$ = $1; }
                  | termo T_MAIS termo { $$ = A_OpExp(A_somaOp, $1, $3); }
                  | termo T_MENOS termo { $$ = A_OpExp(A_subOp, $1, $3); }
                  | termo T_OR termo { $$ = A_OpExp(A_orOp, $1, $3); }
;

termo: fator { $$ = $1; }
      | fator T_MULT fator { $$ = A_OpExp(A_multOp, $1, $3); }
      | fator T_DIV fator { $$ = A_OpExp(A_divOp, $1, $3); }
      | fator T_AND fator { $$ = A_OpExp(A_andOp, $1, $3); }
;

fator: variavel { $$ = A_VarExp($1); }
      | T_NUMERO { $$ = A_IntExp($1); }
      | chamada_func { $$ = $1; }
      | logico { $$ = $1; }
      // | T_ABRE_PARENTESES expressao T_FECHA_PARENTESES { $$ = NULL; }
      // | T_NOT fator { $$ = NULL; }
      // | T_MENOS fator { $$ = NULL; }
;

variavel: T_IDENT { $$ = A_Var($1); }
;

logico: T_TRUE { $$ = A_BoolExp(A_true); }
      | T_FALSE { $$ = A_BoolExp(A_false); }
;

chamada_func: T_IDENT T_ABRE_PARENTESES lista_expressoes T_FECHA_PARENTESES { $$ = A_ChamaFuncExp($1, $3); }
;

%%

/* Aqui poderia ser construída a função main com a lógica do compilador, que
   executaria a análise sintática e seguiria com o processo de compilação.
   Entretanto, para deixar nosso código mais organizado, o programa principal
   foi construído separadamente no arquivo compilador.c
*/