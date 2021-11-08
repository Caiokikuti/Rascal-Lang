#ifndef AST_H
#define AST_H

#include "util.h"

typedef struct A_Programa_ *A_Programa;
typedef struct A_Bloco_ *A_Bloco;
typedef struct A_LstDecSub *A_LstDecSub;
typedef struct A_LstDecVar *A_LstDecVar;
typedef struct A_CmdComp *A_CmdComp;
typedef struct A_Exp_ *A_Exp;
typedef struct A_Var_ *A_Var;

A_Programa A_programa(String id, A_Bloco bloco);
A_Bloco A_bloco(A_LstDecSub secDecSub, A_LstDecVar secDecVar, A_CmdComp cmdComp);
A_Exp A_opExp(A_oper oper, A_Exp esquerda, A_Exp direita);
A_Exp A_atribExp(A_Var var, A_Exp exp);

typedef enum {
  A_eqOp,
  A_neqOp,
  A_ltOp,
  A_leOp,
  A_gtOp,
  A_geOp,
  A_somaOp,
  A_subOp,
  A_divOp,
  A_multOp,
  A_orOp,
  A_andOp
} A_oper;

struct A_Programa_ {
    String id;
    A_Bloco bloco;
};

struct A_Bloco_ {
    A_LstDecSub secDecSub;
    A_LstDecVar secDecVar;
    A_CmdComp cmdComp;
};

struct A_Var_ {
  String id;
};

struct A_Exp_ {
  enum {
    A_opExp,
  } tipo;
  union {
    struct {
      A_oper oper;
      A_Exp esquerda;
      A_Exp direita;
    } op;
    struct {
      A_Var var;
      A_Exp exp;
    } atrib;
    struct {
      String func;
      
    } chama;
  } u;
};

struct A_LstDecSub_ {
    // implementar...
};

struct A_LstDecVar {
    // implementar...
};

struct A_CmdComp_ {
    // implementar...
};

#endif /* AST_H */