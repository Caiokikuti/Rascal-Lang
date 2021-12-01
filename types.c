#include <stdio.h>
#include "includes/util.h"
#include "includes/symbol.h"
#include "includes/types.h"


static struct Ty_ty_ tyint = {Ty_int};
Ty_ty Ty_Int(void) { return &tyint; }

static struct Ty_ty_ tybool = {Ty_bool};
Ty_ty Ty_Bool(void) { return &tybool; }

static struct Ty_ty_ tyvoid = {Ty_void};
Ty_ty Ty_Void(void) { return &tyvoid; }

Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail) {
	Ty_tyList p = checked_malloc(sizeof(*p));
	p->head = head;
	p->tail = tail;
	return p;
}

static char error_str_ty[][12] = {
	"int", "bool",};

char *Ty_ToString(Ty_ty t)
{
	return error_str_ty[t->kind];
}