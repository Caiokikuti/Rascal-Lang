#ifndef ENV_H__
#define ENV_H__

#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "ast.h"
#include "types.h"

typedef enum {
  global,
  local
} Escopo;

typedef struct E_enventry_ *E_enventry;

struct E_enventry_ {
	enum {
		E_varEntry,
		E_funcEntry,
		E_procEntry,
	} tipo;
	union {
		struct {
      Escopo escopo;
      int endRelativo;
      S_symbol id;
			Ty_ty varTipo;
		} var;
		struct {
			Ty_tyList parametros;
			Ty_ty returnTipo;
			S_symbol label;
			Escopo escopo;
		} func;
		struct {
			Ty_tyList parametros;
			S_symbol label;
			Escopo escopo;
		} proc;
	} u;
};

E_enventry E_VarEntry(Escopo escopo, int endRelativo, S_symbol id, Ty_ty varTipo);
E_enventry E_FuncEntry(Escopo escopo, S_symbol label, Ty_tyList parametros, Ty_ty returnTipo);
E_enventry E_ProcEntry(Escopo escopo, S_symbol label, Ty_tyList parametros);

S_table E_base_tenv(void); /* Ty_ ty environment */
S_table E_base_venv(void); /* E_ enventry environment */

#endif