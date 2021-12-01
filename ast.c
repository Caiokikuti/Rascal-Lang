#include <stdlib.h>
#include "includes/ast.h"
#include "includes/symbol.h"
#include "includes/util.h"

A_programa A_Programa(S_symbol id, A_bloco bloco) {
  A_programa programa = checked_malloc(sizeof(*programa));
  programa->id = id;
  programa->bloco = bloco;
  return programa;
}
A_bloco A_Bloco(A_lstDecVar secDecVar, A_lstDecSub secDecSub, A_exp cmdComp) {
  A_bloco bloco = checked_malloc(sizeof(*bloco));
  bloco->secDecSub = secDecSub;
  bloco->secDecVar = secDecVar;
  bloco->cmdComp = cmdComp;
  return bloco;
}

A_exp A_CmdCompExp(A_lstExp lstCmd) {
  A_exp p = checked_malloc(sizeof(*p));
  p->tipo = A_cmdComp;
  p->u.cmd_cmp.lstCmd = lstCmd;
  return p;
}

A_lstIdent A_LstIdent(S_symbol id, A_lstIdent lstIdent) {
  A_lstIdent no = checked_malloc(sizeof(*no));
  no->id = id;
  no->prox = lstIdent;
  return no;
}

A_decVar A_DecVar(A_pos pos, S_symbol id, S_symbol tipo) {
  A_decVar decVar = checked_malloc(sizeof(*decVar));
  decVar->pos = pos;
  decVar->id = id;
  decVar->tipo = tipo;
}

A_lstDecVar A_LstDecVar(A_decVar decVar, A_lstDecVar lstDecVar) {
  A_lstDecVar no = checked_malloc(sizeof(*no));
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

A_funcDec A_FuncDec(A_pos pos, S_symbol id, A_lstDecVar parametros, S_symbol returnTipo, A_bloco bloco) {
  A_funcDec funcDec = checked_malloc(sizeof(*funcDec));
  funcDec->pos = pos;
  funcDec->id = id;
  funcDec->params = parametros;
  funcDec->returnType = returnTipo;
  funcDec->bloco = bloco;
  return funcDec;
}

A_lstDecFunc A_LstFuncDec(A_funcDec funcDec, A_lstDecFunc lstFuncDec) {
  A_lstDecFunc no = checked_malloc(sizeof(*no));
  no->funcDec = funcDec;
  no->prox = lstFuncDec;
  return no;
}

A_procDec A_ProcDec(A_pos pos, S_symbol id, A_lstDecVar parametros, A_bloco bloco) {
  A_procDec procDec = checked_malloc(sizeof(*procDec));
  procDec->pos = pos;
  procDec->id = id;
  procDec->params = parametros;
  procDec->bloco = bloco;
  return procDec;
}

A_lstDecProc A_LstProcDec(A_procDec procDec, A_lstDecProc lstProcDec) {
  A_lstDecProc no = checked_malloc(sizeof(*no));
  no->procDec = procDec;
  no->prox = lstProcDec;
  return no;
}

A_lstDecSub A_LstSubDec(A_lstDecFunc lstFuncDec, A_lstDecProc lstProcDec) {
  A_lstDecSub lstDecSub = checked_malloc(sizeof(*lstDecSub));
  lstDecSub->lstDecFunc = lstFuncDec;
  lstDecSub->lstDecProc = lstProcDec;
  return lstDecSub;
}

A_var A_Var(A_pos pos, S_symbol id) {
  A_var p = checked_malloc(sizeof(*p));
  p->pos = pos;
  p->id = id;
  return p;
}

A_exp A_VarExp(A_pos pos, A_var var) {
	A_exp p = checked_malloc(sizeof(*p));
  p->pos = pos;
	p->tipo = A_varExp;
	p->u.var = var;
	return p;
}

A_exp A_IntExp(A_pos pos, int i) {
  A_exp p = checked_malloc(sizeof(*p));
  p->pos = pos;
  p->tipo = A_intExp;
  p->u.intExp = i;
  return p;
}

A_exp A_BoolExp(A_pos pos, A_bool booll) {
  A_exp p = checked_malloc(sizeof(*p));
  p->pos = pos;
  p->tipo = A_boolExp;
  p->u.boolExp = booll;
  return p;
}

A_exp A_LeituraExp(A_pos pos, A_lstIdent lstIdent){
  A_exp p = checked_malloc(sizeof(*p));
  p->pos = pos;
  p->tipo = A_leituraExp;
  p->u.leituraExp = lstIdent;
  return p;
}

A_exp A_EscritaExp(A_pos pos, A_lstExp lstExp){
  A_exp p = checked_malloc(sizeof(*p));
  p->pos = pos;
  p->tipo = A_escritaExp;
  p->u.escritaExp = lstExp;
  return p;
}

A_exp A_ChamaFuncExp(A_pos pos, S_symbol func, A_lstExp args) {
  A_exp p = checked_malloc(sizeof(*p));
  p->pos = pos;
  p->tipo = A_chamaFuncExp;
  p->u.chama_func.func = func;
  p->u.chama_func.lstExp = args;
  return p;
}

A_exp A_ChamaProcExp(A_pos pos, S_symbol proc, A_lstExp args) {
  A_exp p = checked_malloc(sizeof(*p));
  p->pos = pos;
  p->tipo = A_chamaProcExp;
  p->u.chama_proc.proc = proc;
  p->u.chama_proc.lstExp = args;
  return p;
}

A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right) {
	A_exp p = checked_malloc(sizeof(*p));
	p->pos = pos;
  p->tipo = A_opExp;
	p->u.op.oper = oper;
	p->u.op.esquerda = left;
	p->u.op.direita = right;
	return p;
}

A_exp A_AtribExp(A_pos pos, A_var var, A_exp exp) {
  A_exp p = checked_malloc(sizeof(*p));
	p->pos = pos;
  p->tipo = A_atribExp;
	p->u.atrib.var = var;
	p->u.atrib.exp = exp;
	return p;
}

A_exp A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp elsee) {
  A_exp p = checked_malloc(sizeof(*p));
	p->pos = pos;
  p->tipo = A_ifExp;
	p->u.iff.test = test;
	p->u.iff.then = then;
	p->u.iff.elsee = elsee;
	return p;
}

A_exp A_WhileExp(A_pos pos, A_exp test, A_exp body) {
  A_exp p = checked_malloc(sizeof(*p));
	p->pos = pos;
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
