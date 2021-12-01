/*
 * util.c - commonly used utility functions.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "includes/util.h"

void *checked_malloc(int len)
{void *p = malloc(len);
 if (!p) {
    fprintf(stderr,"\nRan out of memory!\n");
    exit(1);
 }
 return p;
}

String string(char *s) {
  String p = checked_malloc(strlen(s)+1);
  strcpy(p,s);
  return p;
}
