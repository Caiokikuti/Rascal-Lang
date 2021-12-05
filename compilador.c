#include <stdio.h>
#include <stdlib.h>
#include "includes/ast.h"
#include "includes/semant.h"
#include "includes/errormsg.h"
#include "includes/tradMepa.h"
#include "sintatico.tab.h"

A_programa raiz_ast;

/* Compilar:
   $ make
   
   Executar:
   $ ./compilador arquivo.ras
*/

int main(int argc, char** argv) {
  FILE* fp;
  extern FILE* yyin;
  TRAD_expList listaExp;

  if (argc < 2 || argc > 2) {
      fprintf(stderr, "Erro: número inválido de parâmetros\n");
      fprintf(stderr, "Uso: compilador <arquivo>\n");
      return EXIT_FAILURE;
  }

  fp = fopen(argv[1], "r");
  if (fp == NULL) {
      printf("Erro: não foi possível ler o arquivo '%s'\n", argv[1]);
      return EXIT_FAILURE;
  }

  yyin = fp;
  if (yyparse() == 0) {
      fprintf(stderr, "\nAnálise sintática feita com sucesso!\n");
  } else {
      fprintf(stderr, "\nAnálise sintática com erros!\n");
  }

  listaExp = SEMANT_tradProg(raiz_ast);

  if (!EM_anyErrors) {
    fprintf(stderr, "\nAnálise semântica feita com sucesso!\n");
  } else {
    fprintf(stderr, "\nAnálise semântica com erros!\n");
  }

  // raiz_ast está apontando para o nó raiz da AST (programa) caso o parsing foi bem sucedido.
  return EXIT_SUCCESS;
}