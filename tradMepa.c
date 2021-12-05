#include <stdio.h>
#include "includes/util.h"
#include "includes/symbol.h"
#include "includes/label.h"
#include "includes/tradMepa.h"

Escopo Tr_global() {
  return global;
}

Escopo Tr_local() {
  return local;
}

TRAD_exp Trad_NoExp() {
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_NOEXP;
  tradExp->u.cmdSimples = NADA;
  
  return tradExp;
}

TRAD_exp InitProg() {
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_CMDSIMPLES;
  tradExp->u.cmdSimples = INPP;

  return tradExp;
}

TRAD_exp EndProg() {
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_CMDSIMPLES;
  tradExp->u.cmdSimples = PARA;

  return tradExp;
}

TRAD_exp Trad_CmdSimples(TRAD_cmdSimples cmd){
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_CMDSIMPLES;
  tradExp->u.cmdSimples = cmd;
  
  return tradExp;
}

TRAD_exp Trad_Mem(TRAD_mem cmd, int qtdMem){
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_MEM;
  tradExp->u.MEM.cmdMem = cmd;
  tradExp->u.MEM.qtdMem = qtdMem;

  return tradExp;
}

TRAD_exp Trad_CmdManipValor(TRAD_cmdManipValor cmd, Escopo escopo, int endRelativo){
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_MANIPVALOR;
  tradExp->u.MANIPVALOR.cmdManip = cmd;
  tradExp->u.MANIPVALOR.escopo = escopo;
  tradExp->u.MANIPVALOR.endRelativo = endRelativo;

  return tradExp;
}

TRAD_exp Trad_CarregaCT(TRAD_carregaCT cmd, int constante){
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_CARREGACT;
  tradExp->u.CARREGACT.cmdCRCT = cmd;
  tradExp->u.CARREGACT.constante = constante;

  return tradExp;
}

TRAD_exp Trad_CmdDesvio(TRAD_cmdDesvio cmd, Label label){
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_DESVIO;
  tradExp->u.DESVIO.cmdDesvio = cmd;
  tradExp->u.DESVIO.label = label;

  return tradExp;
}

TRAD_exp Trad_ChamaProc(){
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = TRAD_CHAMAPROC;

  return tradExp;
}

TRAD_expList Trad_ExpList() {
	TRAD_expList p = (TRAD_expList) checked_malloc (sizeof *p);
	p->head = NULL;
  p->tail = NULL;
  
	return p;
}

TRAD_exp Trad_SimpleVar(E_enventry simpleVar) {
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = SIMPLEVAR;
  tradExp->u.simpleVar = simpleVar;

  return tradExp;
}

void Trad_ExpList_append(TRAD_expList list, TRAD_exp expr) {
	if (list->head) {
		TRAD_node node = checked_malloc(sizeof(*node));
		node->expr = expr;
		node->next = NULL;
		list->tail->next = node;
		list->tail = list->tail->next;
	} else Trad_ExpList_prepend(list, expr);
}

void Trad_ExpList_prepend(TRAD_expList list, TRAD_exp expr) {
	if (list->head) {
		TRAD_node node = checked_malloc(sizeof(*node));
		node->expr = expr;
		node->next = list->head;
		list->head = node;
	} else {
		list->head = checked_malloc(sizeof(*list->head));
		list->head->expr = expr;
		list->head->next = NULL;
		list->tail = list->head;
	}
}

void InsertLabel(TRAD_expList list, Label label, TRAD_exp expr) {
  TRAD_exp tradExp = checked_malloc(sizeof(*tradExp));
  tradExp->tipo = NEWLABEL;
  tradExp->u.INSERTLABEL.newLabel = label;
  tradExp->u.INSERTLABEL.exp = expr;
  
  Trad_ExpList_append(list, tradExp);
}