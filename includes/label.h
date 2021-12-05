#ifndef TEMP_H__
#define TEMP_H__

#include <stdio.h>
#include "symbol.h"
#include "util.h"

typedef S_symbol Label;
Label newlabel(void);
Label namedlabel(string s);

#endif