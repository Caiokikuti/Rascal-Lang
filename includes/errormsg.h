#ifndef ERRORMSG_H__
#define ERRORMSG_H__

#include "util.h"

extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;

void EM_error(int, String,...);
void EM_impossible(String,...);
void EM_reset(String filename);

#endif