#ifndef UTIL_H
#define UTIL_H

#include <assert.h>
#include <stddef.h>

typedef char bool;

#define TRUE 1
#define FALSE 0

typedef char* string;
void ajustaPosicao(void);
void *checked_malloc(int len);
string String(string);
#endif