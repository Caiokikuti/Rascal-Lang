#include <stdlib.h>
#include "ast.h"

A_programa A_Programa(String id, A_bloco bloco) {
  A_programa programa = malloc(sizeof(*programa));
  programa->id = id;
  programa->bloco = bloco;
  return programa;
}
// A_bloco A_Bloco(A_lstDecVar secDecVar, A_lstDecSub secDecSub, A_CmdComp cmdComp) {
A_bloco A_Bloco(A_lstDecVar secDecVar, A_lstDecSub secDecSub, A_lstExp cmdComp) {
  A_bloco bloco = malloc(sizeof(*bloco));
  bloco->secDecSub = secDecSub;
  bloco->secDecVar = secDecVar;
  bloco->cmdComp = cmdComp;
  return bloco;
}

A_lstIdent A_LstIdent(String id, A_lstIdent lstIdent) {
  A_lstIdent no = malloc(sizeof(*no));
  no->id = id;
  no->prox = lstIdent;
  return no;
}

A_dec A_DecVar(String id, String tipo) {
  A_dec decVar = malloc(sizeof(*decVar));
  decVar->u.var.id = id;
  decVar->u.var.tipo = tipo;
}

A_lstDecVar A_LstDecVar(A_dec decVar, A_lstDecVar lstDecVar) {
  A_lstDecVar no = malloc(sizeof(*no));
  no->decVar = decVar;
  no->prox = lstDecVar;
  return no;
}

A_lstDecVar concatLstDecVar(A_lstDecVar lst1, A_lstDecVar lst2) {
  A_lstDecVar lstConcat = lst1;
  if (lst1 != NULL) {
    while (lst1->prox != NULL) {
        lst1 = lst1->prox;
    }
    lst1->prox = lst2;
  }
  return lstConcat;
}

A_funcDec A_FuncDec(String id, A_lstDecVar parametros, String returnTipo, A_bloco bloco) {
  A_funcDec funcDec = malloc(sizeof(*funcDec));
  funcDec->id = id;
  funcDec->params = parametros;
  funcDec->returnType = returnTipo;
  funcDec->bloco = bloco;
  return funcDec;
}

A_lstDecFunc A_LstFuncDec(A_funcDec funcDec, A_lstDecFunc lstFuncDec) {
  A_lstDecFunc no = malloc(sizeof(*no));
  no->funcDec = funcDec;
  no->prox = lstFuncDec;
  return no;
}

A_procDec A_ProcDec(String id, A_lstDecVar parametros, A_bloco bloco) {
  A_procDec procDec = malloc(sizeof(*procDec));
  procDec->id = id;
  procDec->params = parametros;
  procDec->bloco = bloco;
  return procDec;
}

A_lstDecProc A_LstProcDec(A_procDec procDec, A_lstDecProc lstProcDec) {
  A_lstDecProc no = malloc(sizeof(*no));
  no->procDec = procDec;
  no->prox = lstProcDec;
  return no;
}

A_lstDecSub A_LstSubDec(A_lstDecFunc lstFuncDec, A_lstDecProc lstProcDec) {
  A_lstDecSub lstDecSub = malloc(sizeof(*lstDecSub));
  lstDecSub->lstDecFunc = lstFuncDec;
  lstDecSub->lstDecProc = lstProcDec;
  return lstDecSub;
}

A_var A_Var(String id) {
  A_var p = malloc(sizeof(*p));
  p->id = id;
  return p;
}

A_exp A_VarExp(A_var var) {
	A_exp p = checked_malloc(sizeof(*p));
	p->tipo = A_varExp;
	p->u.var = var;
	return p;
}

A_exp A_IntExp(int i) {
  A_exp p = malloc(sizeof(*p));
  p->tipo = A_intExp;
  p->u.intExp = i;
  return p;
}

A_exp A_StrExp(String s) {
  A_exp p = malloc(sizeof(*p));
  p->tipo = A_strExp;
  p->u.strExp = s;
  return p;
}

A_exp A_BoolExp(A_bool booll) {
  A_exp p = malloc(sizeof(*p));
  p->tipo = A_boolExp;
  p->u.boolExp = booll;
  return p;
}

A_exp A_LeituraExp(A_lstIdent lstIdent){
  A_exp p = malloc(sizeof(*p));
  p->tipo = A_leituraExp;
  p->u.leituraExp = lstIdent;
  return p;
}

A_exp A_EscritaExp(A_lstExp lstExp){
  A_exp p = malloc(sizeof(*p));
  p->tipo = A_escritaExp;
  p->u.escritaExp = lstExp;
  return p;
}

A_exp A_ChamaFuncExp(String func, A_lstExp args) {
  A_exp p = malloc(sizeof(*p));
  p->tipo = A_chamaFuncExp;
  p->u.chama_func.func = func;
  p->u.chama_func.lstExp = args;
  return p;
}

A_exp A_ChamaProcExp(String proc, A_lstExp args) {
  A_exp p = malloc(sizeof(*p));
  p->tipo = A_chamaProcExp;
  p->u.chama_proc.proc = proc;
  p->u.chama_proc.lstExp = args;
  return p;
}

A_exp A_OpExp(A_oper oper, A_exp left, A_exp right) {
	A_exp p = checked_malloc(sizeof(*p));
	p->tipo = A_opExp;
	p->u.op.oper = oper;
	p->u.op.esquerda = left;
	p->u.op.direita = right;
	return p;
}

A_exp A_AtribExp(A_var var, A_exp exp) {
  A_exp p = checked_malloc(sizeof(*p));
	p->tipo = A_atribExp;
	p->u.atrib.var = var;
	p->u.atrib.exp = exp;
	return p;
}

A_exp A_IfExp(A_exp test, A_exp then, A_exp elsee) {
  A_exp p = checked_malloc(sizeof(*p));
	p->tipo = A_ifExp;
	p->u.iff.test = test;
	p->u.iff.then = then;
	p->u.iff.elsee = elsee;
	return p;
}

A_exp A_WhileExp(A_exp test, A_exp body) {
  A_exp p = checked_malloc(sizeof(*p));
	p->tipo = A_whileExp;
	p->u.whilee.test = test;
	p->u.whilee.body = body;
	return p;
}

A_lstExp A_LstExp(A_exp exp, A_lstExp lstExp) {
  A_lstExp p = checked_malloc(sizeof(*p));
	p->exp = exp;
	p->lstExp = lstExp;
	return p;
}

A_lstExp concatLstExp(A_lstExp lstExp1, A_lstExp lstExp2) {
  A_lstExp lstConcat = lstExp1;
  if (lstExp1 != NULL) {
    while (lstExp1->lstExp != NULL) {
        lstExp1 = lstExp1->lstExp;
    }
    lstExp1->lstExp = lstExp2;
  }
  return lstConcat;
}
