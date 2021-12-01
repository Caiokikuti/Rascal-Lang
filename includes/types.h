#ifndef TYPES_H__
#define TYPES_H__

#include "util.h"

typedef struct Ty_ty_ *Ty_ty;
typedef struct Ty_tyList_ *Ty_tyList;

struct Ty_ty_ {
	enum {
		Ty_int,
		Ty_bool,
		Ty_void,
	} kind;
};

struct Ty_tyList_ {
	Ty_ty head;
	Ty_tyList tail;
};

Ty_ty Ty_Int(void);
Ty_ty Ty_Bool(void);
Ty_ty Ty_Void(void);

Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail);
char *Ty_ToString(Ty_ty t);

#endif