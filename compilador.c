#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "includes/ast.h"
#include "includes/semant.h"
#include "includes/errormsg.h"
#include "includes/tradMepa.h"
#include "includes/geraCodigoMepa.h"
#include "sintatico.tab.h"

A_programa raiz_ast;

/* Compilar:
   $ make
   
   Executar:
   $ ./compilador arquivo.ras
*/

static void read_params(int argc, char **argv, string inFile, string outFile) {
	int param;

	while ((param = getopt(argc, argv, "p:s:h")) != -1) {
		switch(param) {
			case 'p':
				strcpy(inFile, optarg);
			break;

			case 's':
				strcpy(outFile, optarg);
			break;
			
			case 'h':
				printf("  -p <nomeArquivoEntrada.ras> -> Nome do programa de entrada.\n");
				printf("  -s <nomeArquivoSaida> -> Arquivo de saída para o código MEPA.\n");
				exit(EXIT_SUCCESS);
			break;
		}
	}
}

int main(int argc, char** argv) {
  FILE* fp;
  FILE* out;
  extern FILE* yyin;
  TRAD_expList listaExp;
  string inFile = checked_malloc(30);
  string outFile = checked_malloc(30);
  strcpy(outFile, "out.MEPA");

  read_params(argc, argv, inFile, outFile);
  fp = fopen(inFile, "r");

  if (fp == NULL) {
    if (strlen(inFile)==0) {
      printf("Para executar, é necessário no mínimo o nome do arquivo de entrada.\nPara isso, utilize a flag -p <nomeArquivoEntrada.ras>.\n");
    } else {
      printf("Erro: não foi possível ler o arquivo de entrada '%s'\n", inFile);
    }
    return EXIT_FAILURE;
  }
  
  out = fopen(outFile, "w");

  yyin = fp;
  if (yyparse() == 0) {
    fprintf(stderr, "\nAnálise sintática feita com sucesso!\n");
  } else {
    fprintf(stderr, "\nAnálise sintática com erros!\n");
    return EXIT_FAILURE;
  }

  if (!EM_anyErrors) {
    listaExp = SEMANT_tradProg(raiz_ast);
  }
  
  if (!EM_anyErrors) {
    fprintf(stderr, "\nAnálise semântica feita com sucesso!\n");
  } else {
    fprintf(stderr, "\nAnálise semântica com erros!\n");
    return EXIT_FAILURE;
  }

  geraCodigoMepa(out, listaExp);
  fprintf(stderr, "\nCódigo MEPA gerado com sucesso!\nNome do arquivo de saída: %s\n", outFile);

  fclose(out);

  // raiz_ast está apontando para o nó raiz da AST (programa) caso o parsing foi bem sucedido.
  return EXIT_SUCCESS;
}