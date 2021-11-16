#ifndef AST_H
#define AST_H

#include "util.h"

typedef struct A_Programa_ *A_programa;
typedef struct A_Bloco_ *A_bloco;
typedef struct A_LstDecSub_ *A_lstDecSub;
typedef struct A_CmdComp *A_CmdComp;
typedef struct A_Exp_ *A_exp;
typedef struct A_Var_ *A_var;
typedef struct A_Dec_ *A_dec;
typedef struct A_LstDec_ *A_lstDec;
typedef struct A_LstIdent_ *A_lstIdent;
typedef struct A_LstDecVar_ *A_lstDecVar;

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

struct A_Dec_ {
  enum {
    A_varDec,
    A_functionDec,
    A_procDec,
    A_typeDec
  } tipo;
  
  union {
    struct {
      String id;
      String tipo;
    } var;
  } u;
};

struct A_LstDecVar_ {
  A_dec decVar;
  A_lstDecVar prox;
};

struct A_LstDec_ {
  A_lstDec listDec;
  String tipo;
};

struct A_Programa_ {
    String id;
    A_bloco bloco;
};

struct A_Bloco_ {
    A_lstDecSub secDecSub;
    A_lstDecVar secDecVar;
    A_CmdComp cmdComp;
};

struct A_Var_ {
  String id;
};

struct A_LstIdent_ {
  String id;
  A_lstIdent prox;
};

struct A_Exp_ {
  enum {
    A_opExp,
  } tipo;
  union {
    int intExp;
    struct {
      A_oper oper;
      A_exp esquerda;
      A_exp direita;
    } op;
    struct {
      A_var var;
      A_exp exp;
    } atrib;
    struct {
      String func;
      
    } chama;
    
  } u;
};

struct A_LstDecSub_ {
    // implementar...
};



struct A_CmdComp_ {
    // implementar...
};

A_programa A_Programa(String id, A_bloco bloco);
A_bloco A_Bloco(A_lstDecVar secDecVar, A_lstDecSub secDecSub, A_CmdComp cmdComp);
A_lstIdent A_LstIdent(String id, A_lstIdent lstIdent);
A_dec A_DecVar(String id, String tipo);
A_lstDecVar A_LstDecVar(A_dec decVar, A_lstDecVar lstDecVar);
A_lstDecVar concatLstDecVar(A_lstDecVar lst1, A_lstDecVar lst2);
A_exp A_OpExp(A_oper oper, A_exp left, A_exp right);

#endif /* AST_H */