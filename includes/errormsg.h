#ifndef ERRORMSG_H__
#define ERRORMSG_H__

#include "util.h"

extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;

void EM_error(int, string,...);
void EM_impossible(string,...);
void EM_reset(string filename);

#endif