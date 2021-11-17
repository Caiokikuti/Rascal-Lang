#ifndef AST_H
#define AST_H

#include "util.h"

typedef struct A_Programa_ *A_programa;
typedef struct A_Bloco_ *A_bloco;
typedef struct A_CmdComp *A_CmdComp;
typedef struct A_Exp_ *A_exp;
typedef struct A_Var_ *A_var;
typedef struct A_Dec_ *A_dec;
typedef struct A_LstIdent_ *A_lstIdent;
typedef struct A_LstDecVar_ *A_lstDecVar;
typedef struct A_LstDecFunc_ *A_lstDecFunc;
typedef struct A_LstDecSub_ *A_lstDecSub;
typedef struct A_FuncDec_ *A_funcDec;
typedef struct A_ProcDec_ *A_procDec;
typedef struct A_LstDecProc_ *A_lstDecProc;

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
    A_procedureDec,
    A_typeDec
  } tipo;
  union {
    A_funcDec func;
    A_procDec proc;
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

struct A_FuncDec_ {
	String id;
	A_lstDecVar params;
	String returnType;
	A_bloco bloco;
};

struct A_LstDecFunc_ {
  A_funcDec funcDec;
  A_lstDecFunc prox;
};

struct A_ProcDec_ {
	String id;
	A_lstDecVar params;
	A_bloco bloco;
};

struct A_LstDecProc_ {
  A_procDec procDec;
  A_lstDecProc prox;
};

struct A_LstDecSub_ {
  A_lstDecFunc lstDecFunc;
  A_lstDecProc lstDecProc;
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

struct A_CmdComp_ {
    // implementar...
};

A_programa A_Programa(String id, A_bloco bloco);
A_bloco A_Bloco(A_lstDecVar secDecVar, A_lstDecSub secDecSub, A_CmdComp cmdComp);
A_lstIdent A_LstIdent(String id, A_lstIdent lstIdent);
A_dec A_DecVar(String id, String tipo);
A_lstDecVar A_LstDecVar(A_dec decVar, A_lstDecVar lstDecVar);
A_lstDecVar concatLstDecVar(A_lstDecVar lst1, A_lstDecVar lst2);
A_funcDec A_FuncDec(String id, A_lstDecVar parametros, String returnTipo, A_bloco bloco);
A_lstDecFunc A_LstFuncDec(A_funcDec funcDec, A_lstDecFunc lstFuncDec);
A_procDec A_ProcDec(String id, A_lstDecVar parametros, A_bloco bloco);
A_lstDecProc A_LstProcDec(A_procDec procDec, A_lstDecProc lstProcDec);
A_lstDecSub A_LstSubDec(A_lstDecFunc lstFuncDec, A_lstDecProc lstProcDec);
A_exp A_OpExp(A_oper oper, A_exp left, A_exp right);

#endif /* AST_H */