#include <stdlib.h>
#include "ast.h"

A_programa A_Programa(String id, A_bloco bloco) {
  A_programa programa = malloc(sizeof(*programa));
  programa->id = id;
  programa->bloco = bloco;
  return programa;
}

A_bloco A_Bloco(A_lstDecVar secDecVar, A_lstDecSub secDecSub, A_CmdComp cmdComp) {
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