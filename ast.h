#ifndef AST_H
#define AST_H

#include "util.h"

typedef struct A_Programa_ *A_Programa;
typedef struct A_Bloco_ *A_Bloco;
typedef struct A_LstDecSub *A_LstDecSub;
typedef struct A_LstDecVar *A_LstDecVar;
typedef struct A_CmdComp *A_CmdComp;

A_Programa A_programa(String id, A_Bloco bloco);
A_Bloco A_bloco(A_LstDecSub secDecSub, A_LstDecVar secDecVar, A_CmdComp cmdComp);

struct A_Programa_ {
    String id;
    A_Bloco bloco;
};

struct A_Bloco_ {
    A_LstDecSub secDecSub;
    A_LstDecVar secDecVar;
    A_CmdComp cmdComp;
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