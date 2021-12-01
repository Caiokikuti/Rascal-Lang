#ifndef TRANSLATE_H__
#define TRANSLATE_H__

#include "ast.h"
#include "util.h"
#include "tree.h"

typedef struct Tr_exp_ *Tr_exp;
typedef struct Tr_expList_ *Tr_expList;
typedef struct Tr_node_ *Tr_node;

typedef enum {
  global,
  local
} Escopo;

struct Tr_exp_ {
	enum {Tr_ex} kind;
	union {
		T_exp ex;
	} u;
};

struct Tr_expList_ {
	Tr_node head;
	Tr_node tail;
};

struct Tr_node_ {
	Tr_exp expr;
	Tr_node next;
};


Escopo Tr_global();
Escopo Tr_local();

Tr_expList Tr_ExpList();
void Tr_ExpList_append(Tr_expList list, Tr_exp expr);
void Tr_ExpList_prepend(Tr_expList list, Tr_exp expr);
int Tr_ExpList_empty(Tr_expList list);

Tr_exp Tr_noExp();
Tr_exp Tr_intExp(int n);
Tr_exp Tr_arithExp(A_oper op, Tr_exp left, Tr_exp right);
Tr_exp Tr_eqExp(A_oper op, Tr_exp left, Tr_exp right);
Tr_exp Tr_chamaExp(Escopo escopo, Escopo funcEscopo, S_symbol funLabel, Tr_expList argList);
Tr_exp Tr_atribExp(Tr_exp var, Tr_exp exp);
Tr_exp Tr_relExp(A_oper op, Tr_exp left, Tr_exp right);

#endif