#include <stdio.h>
#include <stdlib.h>
#include "includes/ast.h"
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
        fprintf(stderr, "\nSucesso!");
    } else {
        fprintf(stderr, "\nAnálise com erros!");
    }

    // raiz_ast está apontando para o nó raiz da AST (programa) caso o parsing foi bem sucedido.

    return EXIT_SUCCESS;
}