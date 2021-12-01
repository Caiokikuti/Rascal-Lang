#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "includes/util.h"
#include "includes/errormsg.h"

bool EM_anyErrors = FALSE;

static String fileName = "";

static int lineNum = 1;

int EM_tokPos = 0;

extern FILE *yyin;

typedef struct intList {
	int i;
	struct intList *rest;
} * IntList;

static IntList intList(int i, IntList rest) {
	IntList l = checked_malloc(sizeof *l);
	l->i = i;
	l->rest = rest;
	return l;
}

static IntList linePos = NULL;

void EM_newline(void) {
	lineNum++;
	linePos = intList(EM_tokPos, linePos);
}

void EM_error(int pos, char *message, ...) {
	va_list ap;
	IntList lines = linePos;
	int num = lineNum;

	EM_anyErrors = TRUE;
	while (lines && lines->i >= pos) {
		lines = lines->rest;
		num--;
	}

	if (fileName)
		fprintf(stderr, "%s", fileName);
	if (lines)
		fprintf(stderr, "lin:%d col:%d -> ", num, pos - lines->i);

	va_start(ap, message);
	vfprintf(stderr, message, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}

void EM_reset(String fname) {
	EM_anyErrors = FALSE;
	fileName = fname;
	lineNum = 1;
	linePos = intList(0, NULL);
	yyin = fopen(fname, "r");
	if (!yyin) {
		EM_error(0, "Nao foi possivel abrir");
		exit(1);
	}
}