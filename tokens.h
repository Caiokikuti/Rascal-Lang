#ifndef _TOKENS_
#define _TOKENS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

enum tokens {
  T_PROGRAM=1,
  T_VAR,
  T_PROCEDURE,
  T_FUNCTION,
  T_BEGIN,
  T_END,
  T_FALSE,
  T_TRUE,
  T_IF,
  T_THEN,
  T_ELSE,
  T_WHILE,
  T_DO,
  T_READ,
  T_WRITE,
  T_AND,
  T_OR,
  T_NOT,
  T_DIV,
  T_ABRE_PARENTESES,
  T_FECHA_PARENTESES,
  T_PONTO,
  T_VIRGULA,
  T_PONTO_E_VIRGULA,
  T_MAIS,
  T_MENOS,
  T_MULT,
  T_IGUAL,
  T_DIFERENTE,
  T_MAIOR,
  T_MENOR,
  T_MAIOR_IGUAL,
  T_MENOR_IGUAL,
  T_ATRIBUICAO,
  T_DOIS_PONTOS,
  T_IDENT,
  T_NUMERO
};


typedef union {
	int intval;
	String strval;
} Token;

extern Token yylval;

#endif 