#include "includes/env.h"

E_enventry E_VarEntry(Escopo escopo, S_symbol id, Ty_ty ty){
E_enventry varEntry = checked_malloc(sizeof(*varEntry));
	varEntry->tipo = E_varEntry;
  varEntry->u.var.id = id;
	varEntry->u.var.varTipo = ty;
	varEntry->u.var.escopo = escopo;
	return varEntry;
}

E_enventry E_FuncEntry(Escopo escopo, S_symbol label, Ty_tyList formals, Ty_ty result){
	E_enventry funEntry = checked_malloc(sizeof(*funEntry));
	funEntry->tipo = E_funcEntry;
	funEntry->u.func.parametros = formals;
	funEntry->u.func.returnTipo = result;
	funEntry->u.func.escopo = escopo;
	funEntry->u.func.label = label;
	return funEntry;
}

E_enventry E_ProcEntry(Escopo escopo, S_symbol label, Ty_tyList formals){
	E_enventry procEntry = checked_malloc(sizeof(*procEntry));
	procEntry->tipo = E_procEntry;
	procEntry->u.proc.parametros = formals;
	procEntry->u.proc.escopo = escopo;
	procEntry->u.proc.label = label;
	return procEntry;
}

S_table E_base_tenv(void){
	S_table tenv = S_empty();
	S_enter(tenv, S_Symbol("integer"), Ty_Int());
	S_enter(tenv, S_Symbol("boolean"), Ty_Bool());
	return tenv;
}

S_table E_base_venv(void){
  S_table venv = S_empty();
	// Create function environment entries
	// E_enventry env_write = E_FunEntry(Tr_global(), S_Symbol("write"),
	// 	Ty_TyList(Ty_String(), NULL), Ty_Void());
	// E_enventry env_read = E_FunEntry(Tr_global(), S_Symbol("read"),
	// 	Ty_TyList(Ty_String(), NULL), Ty_Void());
	
	// // Add the functions to the environment
	// S_enter(venv, S_Symbol("write"), env_write);
	// S_enter(venv, S_Symbol("read"), env_read);
	return venv;
}
