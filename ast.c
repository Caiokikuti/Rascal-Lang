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