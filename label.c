#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/label.h"
#include "includes/symbol.h"

static int labels = 0;

Label newlabel(void) {
	char buf[100];
	sprintf(buf,"L%d",labels++);
	return namedlabel(String(buf));
}

Label namedlabel(string s) {
	return S_Symbol(s);
}