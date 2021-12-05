#ifndef SEMANT_H__
#define SEMANT_H__

#include "env.h"
// #include "translate.h"
#include "tradMepa.h"

struct expty {
	TRAD_exp exp;
	Ty_ty ty;
};

TRAD_expList SEMANT_tradProg(A_programa programa);

struct expty tradExp(TRAD_expList listAllExp, Escopo escopo, S_table venv, S_table tenv, A_exp exp);
struct expty tradVar(TRAD_expList listAllExp, Escopo escopo, S_table venv, S_table tenv, A_var var);
struct expty tradBloco(TRAD_expList listAllExp, Escopo escopo, S_table venv, S_table tenv, A_bloco bloco);
TRAD_exp tradDecVar(TRAD_expList listAllExp, Escopo escopo, int* endRelativo, S_table venv, S_table tenv, A_decVar dec);
TRAD_exp tradDecFunc(TRAD_expList listAllExp, Escopo escopo, int endRelativo, S_table venv, S_table tenv, A_lstDecFunc dec);
TRAD_exp tradDecProc(TRAD_expList listAllExp, Escopo escopo, int endRelativo, S_table venv, S_table tenv, A_lstDecProc dec);
static int is_equal_ty(Ty_ty tType, Ty_ty eType);
static Ty_ty S_look_ty(S_table tenv, S_symbol sym);

#endif