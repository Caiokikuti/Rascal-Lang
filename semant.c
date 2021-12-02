#include <stdlib.h>
#include <string.h>
#include "includes/util.h"
#include "includes/semant.h"
#include "includes/errormsg.h"
#include "includes/translate.h"

struct expty expTy(Tr_exp exp, Ty_ty ty) {
	struct expty e;
	e.exp = exp;
	e.ty = ty;
	return e;
}

int is_equal_ty(Ty_ty actualtType, Ty_ty eType) {
	int tyKind = actualtType->kind;
	int eKind = eType->kind;
	return ((actualtType == eType) || (tyKind == eKind));
}

static Ty_ty S_look_ty(S_table tenv, S_symbol sym) {
	Ty_ty t = S_look(tenv, sym);
	if (t)
		return t;
	else
		return NULL;
}

static Ty_tyList makeFormalTys(S_table tenv, A_lstDecVar params)
{
	Ty_tyList paramTys = NULL;
	Ty_tyList tailList = paramTys;
	A_lstDecVar paramList;
	for (paramList = params; paramList; paramList = paramList->prox) {
		Ty_ty t = S_look_ty(tenv, paramList->decVar->tipo);
		if (!t) {
			EM_error(paramList->decVar->pos, "Tipo indefinido %s",
				S_name(paramList->decVar->tipo));
		} else {
			if (paramTys) {
				tailList->tail = Ty_TyList(t, NULL);
				tailList = tailList->tail;
			} else {
				paramTys = Ty_TyList(t, NULL);
				tailList = paramTys;
			}
		}
	}
	return paramTys;
}

void SEMANT_tradProg(A_programa programa) {
	S_table tenv = E_base_tenv();
	S_table venv = E_base_venv();
	struct expty expr = tradBloco(Tr_global(), venv, tenv, programa->bloco);
}


struct expty tradBloco(Escopo escopo, S_table venv, S_table tenv, A_bloco bloco) {
  struct expty exp;
  A_lstDecVar listaDecVar = NULL;
  A_lstExp listaExp = NULL;
  Tr_expList list = Tr_ExpList();

  S_beginScope(venv);
  S_beginScope(tenv);

  for (listaDecVar = bloco->secDecVar; listaDecVar; listaDecVar = listaDecVar->prox) {
    Tr_ExpList_prepend(list, tradDecVar(escopo, venv, tenv, listaDecVar->decVar));
  }

  if (bloco->secDecSub){
    if(bloco->secDecSub->lstDecProc) {
      Tr_ExpList_prepend(list, tradDecProc(escopo, venv, tenv, bloco->secDecSub->lstDecProc));
    }

    if (bloco->secDecSub->lstDecFunc) {
      Tr_ExpList_prepend(list, tradDecFunc(escopo, venv, tenv, bloco->secDecSub->lstDecFunc));
    }
  }
  
  if (bloco->cmdComp) {
    exp = tradExp(escopo, venv, tenv, bloco->cmdComp);
  }

  S_endScope(venv);
  S_endScope(tenv);
  return exp;
}

struct expty tradExp(Escopo escopo, S_table venv, S_table tenv, A_exp exp) {
	switch (exp->tipo) {
		case A_varExp: {
			return tradVar(escopo, venv, tenv, exp->u.var);
		}

		case A_intExp: {
			// return expTy(Tr_intExp(exp->u.intExp), Ty_Int());
			return expTy(NULL, Ty_Int());
		}
		
    case A_boolExp: {
			// return expTy(Tr_intExp(exp->u.intExp), Ty_Int());
			return expTy(NULL, Ty_Bool());
		}

		case A_opExp: {
			A_oper oper = exp->u.op.oper;
			struct expty left = tradExp(escopo, venv, tenv, exp->u.op.esquerda);
			struct expty right = tradExp(escopo, venv, tenv, exp->u.op.direita);
			// Tr_exp translation = Tr_noExp();

			switch (oper) {
				case A_somaOp:
				case A_subOp:
				case A_multOp:
				case A_divOp:
					if (left.ty->kind != Ty_int)
						EM_error(exp->u.op.esquerda->pos, "Tipo inteiro necessario");
					if (right.ty->kind != Ty_int)
						EM_error(exp->u.op.direita->pos, "Tipo inteiro necessario");
					// return expTy(Tr_arithExp(oper, left.exp, right.exp), Ty_Int());
					return expTy(NULL, Ty_Int());

				case A_eqOp:
				case A_neqOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            // translation = Tr_eqExp(oper, left.exp, right.exp);
          } else {	
            EM_error(exp->u.op.direita->pos, "Expressao inesperada na comparacao: %s", right.ty);
          }
					// return expTy(translation, Ty_Int());
					return expTy(NULL, Ty_Bool());
        }

				case A_gtOp:
				case A_ltOp:
				case A_leOp:
				case A_geOp: {
					if (right.ty->kind != left.ty->kind) {
						EM_error(exp->u.op.direita->pos, "%s dado; esperado %s", right.ty->kind, Ty_ToString(left.ty));
					}
					switch(left.ty->kind) {
						case Ty_int:
							// translation = Tr_relExp(oper, left.exp, right.exp);
							break;
						
						default: {
							EM_error(exp->u.op.direita->pos, "Tipo inesperado %s na comparacao", Ty_ToString(right.ty));
							// translation = Tr_noExp();
						}
					}
					// return expTy(translation, Ty_Int());
					return expTy(NULL, Ty_Bool());
				}
        
        case A_andOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            // translation = Tr_eqExp(oper, left.exp, right.exp);
          } else {	
            EM_error(exp->u.op.direita->pos, "Expressao inesperada na comparacao: %s", right.ty);
          }
					// return expTy(translation, Ty_Int());
					return expTy(NULL, Ty_Bool());
        }

        case A_orOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            // translation = Tr_eqExp(oper, left.exp, right.exp);
          } else {	
            EM_error(exp->u.op.direita->pos, "Expressao inesperada na comparacao: %s", right.ty);
          }
					// return expTy(translation, Ty_Int());
					return expTy(NULL, Ty_Bool());
        }
			}
			assert(0 && "Operador invalido na expressao");
			return expTy(Tr_noExp(), Ty_Int());
		}

		case A_chamaFuncExp: {
			A_lstExp args = NULL;
			Ty_tyList parametros;
			E_enventry x = S_look(venv, exp->u.chama_func.func);
			// Tr_exp translation = Tr_noExp();
			// Tr_expList argList = Tr_ExpList();
			
			if (x && x->tipo == E_funcEntry) {
				parametros = x->u.func.parametros;
				// for (args = exp->u.call.args; args && parametros; args = args->lstExp, parametros = parametros->tail) {
				for (args = exp->u.chama_func.lstExp; args && parametros; args = args->lstExp, parametros = parametros->tail) {
					struct expty arg = tradExp(escopo, venv, tenv, args->exp);
					if (!is_equal_ty(arg.ty, parametros->head))
						EM_error(args->exp->pos, "tipo incorreto %s; esperado: %s",
							Ty_ToString(arg.ty), Ty_ToString(parametros->head));
					// Tr_ExpList_append(argList, arg.exp);
				}

				if (args == NULL && parametros != NULL)
					EM_error(exp->pos, "Estao faltando argumentos!");
				else if (args != NULL && parametros == NULL)
					EM_error(exp->pos, "Muitos argumentos, esperado menos");
				// translation = Tr_chamaExp(escopo, x->u.fun.escopo, x->u.fun.label, argList);
				// return expTy(translation, x->u.fun.result);
				return expTy(NULL, x->u.func.returnTipo);
			} else {
				EM_error(exp->pos, "funcao indefinida: %s", S_name(exp->u.chama_func.func));
				// return expTy(translation, Ty_Int());
				return expTy(NULL, Ty_Int());
			}
		}

    case A_chamaProcExp: {
			A_lstExp args = NULL;
			Ty_tyList parametros;
			E_enventry x = S_look(venv, exp->u.chama_proc.proc);
			// Tr_exp translation = Tr_noExp();
			// Tr_expList argList = Tr_ExpList();
			
			if (x && x->tipo == E_procEntry) {
				parametros = x->u.proc.parametros;
				// for (args = exp->u.call.args; args && parametros; args = args->lstExp, parametros = parametros->tail) {
				for (args = exp->u.chama_proc.lstExp; args && parametros; args = args->lstExp, parametros = parametros->tail) {
					struct expty arg = tradExp(escopo, venv, tenv, args->exp);
					if (!is_equal_ty(arg.ty, parametros->head))
						EM_error(args->exp->pos, "tipo incorreto %s; esperado: %s",
							Ty_ToString(arg.ty), Ty_ToString(parametros->head));
					// Tr_ExpList_append(argList, arg.exp);
				}

				if (args == NULL && parametros != NULL)
					EM_error(exp->pos, "Estao faltando argumentos!");
				else if (args != NULL && parametros == NULL)
					EM_error(exp->pos, "Muitos argumentos, esperado menos");
				// translation = Tr_chamaExp(escopo, x->u.fun.escopo, x->u.fun.label, argList);
				// return expTy(translation, Ty_Void());
				return expTy(NULL, Ty_Void());
			} else {
				EM_error(exp->pos, "funcao indefinida: %s", S_name(exp->u.chama_func.func));
				// return expTy(translation, Ty_Int());
				return expTy(NULL, Ty_Void());
			}
		}

		case A_atribExp: {
      E_enventry x = S_look(venv, exp->u.atrib.var->id);
			struct expty var = tradVar(escopo, venv, tenv, exp->u.atrib.var);
			struct expty newExp = tradExp(escopo, venv, tenv, exp->u.atrib.exp);
			
			if (!is_equal_ty(var.ty, newExp.ty)) {
        if(x->tipo == E_funcEntry) {
          EM_error(exp->u.atrib.exp->pos, "tipo do retorno invalido: esperado o tipo %s, mas retornou o tipo %s",
              Ty_ToString(var.ty), Ty_ToString(newExp.ty));   
        } else {
          EM_error(exp->u.atrib.exp->pos, "atribuicao invalida: esperado o tipo %s, mas recebeu o tipo %s",
              Ty_ToString(var.ty), Ty_ToString(newExp.ty));   
        }
      }

			return expTy(NULL, Ty_Void());
		}

    case A_ifExp: {
			struct expty test, then, elsee;
			test = tradExp(escopo, venv, tenv, exp->u.iff.test);

			if (test.ty->kind != Ty_bool)
				EM_error(exp->u.iff.test->pos, "Boolean necessario!");

			then = tradExp(escopo, venv, tenv, exp->u.iff.then);

			if (exp->u.iff.elsee) {
				elsee = tradExp(escopo, venv, tenv, exp->u.iff.elsee);

				if (!is_equal_ty(then.ty, elsee.ty))
					EM_error(exp->u.iff.elsee->pos, "Branches do if-then-else devem retornar o mesmo tipo");

				// return expTy(Tr_ifExp(test.exp, then.exp, elsee.exp), then.ty);
				return expTy(NULL, then.ty);
			} else {
				// return expTy(Tr_ifExp(test.exp, then.exp, NULL), Ty_Void());
				return expTy(NULL, then.ty);
			}
		}

    case A_whileExp: {
			struct expty test = tradExp(escopo, venv, tenv, exp->u.whilee.test);
			if (test.ty->kind != Ty_bool)
				EM_error(exp->u.whilee.test->pos, "Boolean necessario!");
			struct expty body = tradExp(escopo, venv, tenv, exp->u.whilee.body);

			if (body.ty->kind != Ty_void)
				EM_error(exp->u.whilee.body->pos, "Nao deve ser produzido nenhum valor");
			// return expTy(Tr_whileExp(test.exp, body.exp), Ty_Void());
			return expTy(NULL, Ty_Void());
		}

    case A_cmdComp: {
      struct expty newExpty;
      A_lstExp listaExp = NULL;
			Tr_expList list = Tr_ExpList();

      for (listaExp = exp->u.cmd_cmp.lstCmd; listaExp; listaExp = listaExp->lstExp) {
        newExpty = tradExp(escopo, venv, tenv, listaExp->exp);
        Tr_ExpList_prepend(list, newExpty.exp);
      }

			// return expTy(Tr_seqExp(list), newExpty.ty);
      return newExpty;
		}

    assert(0);
  }
}

struct expty tradVar(Escopo escopo, S_table venv, S_table tenv, A_var var) {
  // Tr_exp translation = Tr_noExp();
  E_enventry x = S_look(venv, var->id);
  if (x && x->tipo == E_varEntry) {
    // translation = Tr_simpleVar(x->u.var.escopo, escopo);
    // return expTy(translation, x->u.var.ty);
    return expTy(NULL, x->u.var.varTipo);
  } else if (x && x->tipo == E_funcEntry) {
    return expTy(NULL, x->u.func.returnTipo);
  } else {
    EM_error(var->pos, "Variavel indefinida %s", S_name(var->id));
    // return expTy(translation, Ty_Int());
    return expTy(NULL, Ty_Int());
  }

  assert(0);
}

Tr_exp tradDecVar(Escopo escopo, S_table venv, S_table tenv, A_decVar dec) {
			// struct expty var = tradExp(escopo, venv, tenv, dec->u.var.init);
			// Tr_access access = Tr_allocLocal(escopo, dec->u.var.escape);
			if (dec->tipo) {
				Ty_ty type = S_look_ty(tenv, dec->tipo);
				if (!type) {
					EM_error(dec->pos, "Tipo invalido %s", S_name(dec->tipo));
					type = Ty_Void();
				}
							
				S_enter(venv, dec->id, E_VarEntry(local, dec->id, type));
			} else {
				EM_error(dec->pos, "Tipo indefinido para a variavel %s", S_name(dec->id));
			}
			return NULL;
}

Tr_exp tradDecFunc(Escopo escopo, S_table venv, S_table tenv, A_lstDecFunc dec) {
  A_lstDecFunc funList;
  Ty_tyList formalTys;
  Ty_ty resultTy;

  for (funList = dec; funList; funList = funList->prox) {
    resultTy = NULL;

    if (funList->funcDec->returnType) {
      resultTy = S_look(tenv, funList->funcDec->returnType);
      if (!resultTy)
        EM_error(funList->funcDec->pos, "Tipo do retorno invalido");
    } else {
        EM_error(funList->funcDec->pos, "Tipo do retorno indefinido");
    }
    if (!resultTy) resultTy = Ty_Void();

    formalTys = makeFormalTys(tenv, funList->funcDec->params);
    Escopo funEscopo = local;
    S_enter(venv, funList->funcDec->id, E_FuncEntry(funEscopo, funList->funcDec->id, formalTys, resultTy));
  }

  E_enventry funEntry = NULL;
  for (funList = dec; funList; funList = funList->prox) {
    funEntry = S_look(venv, funList->funcDec->id);
    S_beginScope(venv);
    Ty_tyList paramTys = funEntry->u.func.parametros;
    A_lstDecVar parametros;

    for (parametros = funList->funcDec->params; parametros; 
      parametros = parametros->prox,
      paramTys = paramTys->tail) {
      S_enter(venv, parametros->decVar->id, E_VarEntry(local, parametros->decVar->id, paramTys->head));
    }

    // struct expty e = tradExp(funEntry->u.func.escopo, venv, tenv, funList->funcDec->bloco->cmdComp);
    struct expty e = tradBloco(funEntry->u.func.escopo, venv, tenv, funList->funcDec->bloco);

    S_endScope(venv);
  }
  // return Tr_noExp();
  return NULL;
}

Tr_exp tradDecProc(Escopo escopo, S_table venv, S_table tenv, A_lstDecProc dec) {
  A_lstDecProc procList;
  Ty_tyList formalTys;
  Ty_ty resultTy;

  for (procList = dec; procList; procList = procList->prox) {
    formalTys = makeFormalTys(tenv, procList->procDec->params);
    Escopo procEscopo = local;
    S_enter(venv, procList->procDec->id, E_ProcEntry(procEscopo, dec->procDec->id, formalTys));
  }

  E_enventry procEntry = NULL;
  for (procList = dec; procList; procList = procList->prox) {
    procEntry = S_look(venv, procList->procDec->id);
    S_beginScope(venv);
    Ty_tyList paramTys = procEntry->u.proc.parametros;
    A_lstDecVar parametros;

    for (parametros = procList->procDec->params; parametros; parametros = parametros->prox, paramTys = paramTys->tail) {
      S_enter(venv, parametros->decVar->id, E_VarEntry(local, parametros->decVar->id, paramTys->head));
    }

    struct expty procBloco = tradBloco(procEntry->u.proc.escopo, venv, tenv, procList->procDec->bloco);

    S_endScope(venv);
  }
  // return Tr_noExp();
  return NULL;
}

