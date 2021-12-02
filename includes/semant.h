#ifndef SEMANT_H__
#define SEMANT_H__

#include "env.h"
#include "translate.h"

struct expty {
	Tr_exp exp;
	Ty_ty ty;
};

void SEMANT_tradProg(A_programa programa);

struct expty tradExp(Escopo escopo, S_table venv, S_table tenv, A_exp exp);
struct expty tradVar(Escopo escopo, S_table venv, S_table tenv, A_var var);
struct expty tradBloco(Escopo escopo, S_table venv, S_table tenv, A_bloco bloco);
Tr_exp tradDecVar(Escopo escopo, S_table venv, S_table tenv, A_decVar dec);
Tr_exp tradDecFunc(Escopo escopo, S_table venv, S_table tenv, A_lstDecFunc dec);
Tr_exp tradDecProc(Escopo escopo, S_table venv, S_table tenv, A_lstDecProc dec);
static int is_equal_ty(Ty_ty tType, Ty_ty eType);
static Ty_ty S_look_ty(S_table tenv, S_symbol sym);

#endif