#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

#define TRUE 1
#define FALSE 0

typedef char* String;
void ajustaPosicao(void);
void *checked_malloc(int len);
String string(String);
#endif