#include <stdlib.h>
#include <string.h>
#include "includes/util.h"
#include "includes/semant.h"
#include "includes/errormsg.h"
#include "includes/tradMepa.h"
#include "includes/label.h"

struct expty createExpTy(TRAD_exp exp, Ty_ty ty) {
	struct expty e;
	e.exp = exp;
	e.ty = ty;
	return e;
}

int is_equal_ty(Ty_ty firstType, Ty_ty secondType) {
	int firstTypeKind = firstType->kind;
	int secondTypeKind = secondType->kind;
	return ((firstType == secondType) || (firstTypeKind == secondTypeKind));
}

static Ty_ty S_look_ty(S_table tenv, S_symbol sym) {
	Ty_ty t = S_look(tenv, sym);
	if (t)
		return t;
	else
		return NULL;
}

static Ty_tyList makeParamsTipos(S_table tenv, A_lstDecVar params)
{
	Ty_tyList paramsTipos = NULL;
	Ty_tyList tailList = paramsTipos;
	A_lstDecVar paramList;
	for (paramList = params; paramList; paramList = paramList->prox) {
		Ty_ty t = S_look_ty(tenv, paramList->decVar->tipo);
		if (!t) {
			EM_error(paramList->decVar->pos, "Tipo indefinido %s",
				S_name(paramList->decVar->tipo));
		} else {
			if (paramsTipos) {
				tailList->tail = Ty_TyList(t, NULL);
				tailList = tailList->tail;
			} else {
				paramsTipos = Ty_TyList(t, NULL);
				tailList = paramsTipos;
			}
		}
	}
	return paramsTipos;
}

TRAD_expList SEMANT_tradProg(A_programa programa) {
	S_table tenv = E_base_tenv();
	S_table venv = E_base_venv();
  TRAD_expList listaAllExp = Trad_ExpList();

  Trad_ExpList_append(listaAllExp, InitProg());

	tradBloco(listaAllExp, Tr_global(), venv, tenv, programa->bloco, TRUE);

  Trad_ExpList_append(listaAllExp, EndProg());

  return listaAllExp;
}

void tradBloco(TRAD_expList listAllExp, Escopo escopo, S_table venv, S_table tenv, A_bloco bloco, bool escopoGlobal) {
  struct expty exp;
  A_lstDecVar listaDecVar = NULL;
  A_lstExp listaExp = NULL;
  int endRelativoVar = 0;
  int qntVar = 0;
  Label label = NULL;

  S_beginScope(venv);
  S_beginScope(tenv);

  for (listaDecVar = bloco->secDecVar; listaDecVar; listaDecVar = listaDecVar->prox, qntVar++) {
    tradDecVar(listAllExp, escopo, &endRelativoVar, venv, tenv, listaDecVar->decVar);
  }

  Trad_ExpList_append(listAllExp, Trad_Mem(AMEM, qntVar));


  if (bloco->secDecSub) {
    if(escopoGlobal) {
      label = newlabel();
      Trad_ExpList_append(listAllExp, Trad_CmdDesvio(DSVS, label));
    }
    if(bloco->secDecSub->lstDecProc) {
      tradDecProc(listAllExp, escopo, venv, tenv, bloco->secDecSub->lstDecProc);
    }

    if (bloco->secDecSub->lstDecFunc) {
      tradDecFunc(listAllExp, escopo, venv, tenv, bloco->secDecSub->lstDecFunc);
    }

    if(escopoGlobal) {
      InsertLabel(listAllExp, label, Trad_NoExp());
    }
  }
  
  if (bloco->cmdComp) {
    tradExp(listAllExp, escopo, venv, tenv, bloco->cmdComp);
  }

  TRAD_exp translation = Trad_Mem(DMEM, qntVar);
  Trad_ExpList_append(listAllExp, translation);

  S_endScope(venv);
  S_endScope(tenv);
}

struct expty tradExp(TRAD_expList listAllExp, Escopo escopo, S_table venv, S_table tenv, A_exp exp) {
	switch (exp->tipo) {
		case A_varExp: {
      struct expty varExp = tradVar(listAllExp, escopo, venv, tenv, exp->u.var);
      if(varExp.exp->u.simpleVar->tipo == E_varEntry) {
        TRAD_exp translation = Trad_CmdManipValor(CRVL, varExp.exp->u.simpleVar->u.var.escopo, varExp.exp->u.simpleVar->u.var.endRelativo);
        Trad_ExpList_append(listAllExp, translation);
      }
			return varExp;
		}

		case A_intExp: {
      TRAD_exp translation = Trad_CarregaCT(CRCT, exp->u.intExp);
      Trad_ExpList_append(listAllExp, translation);
			return createExpTy(translation, Ty_Int());
		}
		
    case A_boolExp: {
      TRAD_exp translation = Trad_CarregaCT(CRCT, exp->u.boolExp);
      Trad_ExpList_append(listAllExp, translation);
			return createExpTy(translation, Ty_Bool());
		}

		case A_opExp: {
			A_oper oper = exp->u.op.oper;
			struct expty left = tradExp(listAllExp, escopo, venv, tenv, exp->u.op.esquerda);
			struct expty right = tradExp(listAllExp, escopo, venv, tenv, exp->u.op.direita);
      TRAD_exp translation = Trad_NoExp();

			switch (oper) {
				case A_somaOp: {
					if (left.ty->kind != Ty_int)
						EM_error(exp->u.op.esquerda->pos, "Tipo inteiro necessario");
					if (right.ty->kind != Ty_int)
						EM_error(exp->u.op.direita->pos, "Tipo inteiro necessario");
          translation = Trad_CmdSimples(SOMA);
          Trad_ExpList_append(listAllExp, translation);
          return createExpTy(translation, Ty_Int());
        }
				case A_subOp: {
					if (left.ty->kind != Ty_int)
						EM_error(exp->u.op.esquerda->pos, "Tipo inteiro necessario");
					if (right.ty->kind != Ty_int)
						EM_error(exp->u.op.direita->pos, "Tipo inteiro necessario");
          translation = Trad_CmdSimples(SUBT);
          Trad_ExpList_append(listAllExp, translation);
          return createExpTy(translation, Ty_Int());
        }
				case A_multOp: {
					if (left.ty->kind != Ty_int)
						EM_error(exp->u.op.esquerda->pos, "Tipo inteiro necessario");
					if (right.ty->kind != Ty_int)
						EM_error(exp->u.op.direita->pos, "Tipo inteiro necessario");
          translation = Trad_CmdSimples(MULT);
          Trad_ExpList_append(listAllExp, translation);
          return createExpTy(translation, Ty_Int());
        }
				case A_divOp: {
					if (left.ty->kind != Ty_int)
						EM_error(exp->u.op.esquerda->pos, "Tipo inteiro necessario");
					if (right.ty->kind != Ty_int)
						EM_error(exp->u.op.direita->pos, "Tipo inteiro necessario");
          translation = Trad_CmdSimples(DIVI);
          Trad_ExpList_append(listAllExp, translation);
          return createExpTy(translation, Ty_Int());
        }

				case A_eqOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            translation = Trad_CmdSimples(CMIG);
            Trad_ExpList_append(listAllExp, translation);
          } else {	
            EM_error(exp->u.op.direita->pos, "Expressao inesperada na comparacao: %s", right.ty);
          }
					return createExpTy(translation, Ty_Bool());
        }
				case A_neqOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            translation = Trad_CmdSimples(CMDG);
            Trad_ExpList_append(listAllExp, translation);
          } else {	
            EM_error(exp->u.op.direita->pos, "Expressao inesperada na comparacao: %s", right.ty);
          }
					return createExpTy(translation, Ty_Bool());
        }

				case A_gtOp: {
					if (is_equal_ty(right.ty, left.ty)) {
            translation = Trad_CmdSimples(CMMA);
            Trad_ExpList_append(listAllExp, translation);
          } else {
						EM_error(exp->u.op.direita->pos, "%s dado; esperado %s", Ty_ToString(right.ty), Ty_ToString(left.ty));
            translation = Trad_NoExp();
          }
					return createExpTy(translation, Ty_Bool());
				}

				case A_ltOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            translation = Trad_CmdSimples(CMME);
            Trad_ExpList_append(listAllExp, translation);
          } else {
						EM_error(exp->u.op.direita->pos, "%s dado; esperado %s", Ty_ToString(right.ty), Ty_ToString(left.ty));
            translation = Trad_NoExp();
          }
					return createExpTy(translation, Ty_Bool());
        }
				case A_leOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            translation = Trad_CmdSimples(CMEG);
            Trad_ExpList_append(listAllExp, translation);
          } else {
						EM_error(exp->u.op.direita->pos, "%s dado; esperado %s", Ty_ToString(right.ty), Ty_ToString(left.ty));
            translation = Trad_NoExp();
          }
					return createExpTy(translation, Ty_Bool());
        }
				case A_geOp: {
				 if (is_equal_ty(right.ty, left.ty)) {
            translation = Trad_CmdSimples(CMAG);
            Trad_ExpList_append(listAllExp, translation);
          } else {
						EM_error(exp->u.op.direita->pos, "comparacao entre tipos diferentes: %s dado; esperado %s", Ty_ToString(right.ty), Ty_ToString(left.ty));
            translation = Trad_NoExp();
          }
					return createExpTy(translation, Ty_Bool());
				}
        
        case A_andOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            translation = Trad_CmdSimples(CONJ);
            Trad_ExpList_append(listAllExp, translation);
          } else {
						EM_error(exp->u.op.direita->pos, "comparacao entre tipos diferentes: %s dado; esperado %s", Ty_ToString(right.ty), Ty_ToString(left.ty));
            translation = Trad_NoExp();
          }
					return createExpTy(translation, Ty_Bool());
        }

        case A_orOp: {
          if (is_equal_ty(right.ty, left.ty)) {
            translation = Trad_CmdSimples(DISJ);
            Trad_ExpList_append(listAllExp, translation);
          } else {
						EM_error(exp->u.op.direita->pos, "comparacao entre tipos diferentes: %s dado; esperado %s", Ty_ToString(right.ty), Ty_ToString(left.ty));
            translation = Trad_NoExp();
          }
					return createExpTy(translation, Ty_Bool());
        }
			}
			assert(0 && "Operador invalido na expressao");
			return createExpTy(Trad_NoExp(), Ty_Int());
		}

    case A_leituraExp: {
      TRAD_exp translation = Trad_NoExp();
      A_lstIdent listaIdent = exp->u.leituraExp;

      for (listaIdent; listaIdent; listaIdent = listaIdent->prox) {
        E_enventry x = S_look(venv, listaIdent->id);
        if (!x) {
          EM_error(exp->pos, "parametro %s indefinido", S_name(listaIdent->id));
        } else if (x->tipo == E_varEntry) {
          translation = Trad_CmdSimples(LEIT);
          Trad_ExpList_append(listAllExp, translation);
          translation = Trad_CmdManipValor(ARMZ, x->u.var.escopo, x->u.var.endRelativo);
          Trad_ExpList_append(listAllExp, translation);
        } else {
          EM_error(exp->pos, "parametro %s invalido", S_name(listaIdent->id));
        }
      }

			return createExpTy(Trad_NoExp(), Ty_Void());
		}
    
    case A_escritaExp: {
			TRAD_exp translation = Trad_NoExp();
      A_lstExp listaExp = exp->u.escritaExp;

      for (listaExp; listaExp; listaExp = listaExp->lstExp) {
        struct expty escritaExp = tradExp(listAllExp, escopo, venv, tenv, listaExp->exp);

        translation = Trad_CmdSimples(IMPR);
        Trad_ExpList_append(listAllExp, translation);
      }

			return createExpTy(Trad_NoExp(), Ty_Void());
		}

		case A_chamaFuncExp: {
			A_lstExp args = NULL;
			Ty_tyList parametros;
			E_enventry x = S_look(venv, exp->u.chama_func.func);
      TRAD_exp translation = Trad_NoExp();
			
			if (x && x->tipo == E_funcEntry) {
				parametros = x->u.func.parametros;
        translation = Trad_Mem(AMEM, 1);
        Trad_ExpList_append(listAllExp, translation);

				for (args = exp->u.chama_func.lstExp; args && parametros; args = args->lstExp, parametros = parametros->tail) {
					struct expty arg = tradExp(listAllExp, escopo, venv, tenv, args->exp);
					if (!is_equal_ty(arg.ty, parametros->head))
						EM_error(args->exp->pos, "tipo incorreto %s; esperado: %s",
							Ty_ToString(arg.ty), Ty_ToString(parametros->head));
				}

        translation = Trad_ChamaProc(CHPR, x->u.func.label, escopo);
        Trad_ExpList_append(listAllExp, translation);

				if (args == NULL && parametros != NULL)
					EM_error(exp->pos, "Estao faltando argumentos!");
				else if (args != NULL && parametros == NULL)
					EM_error(exp->pos, "Muitos argumentos, esperado menos");
				return createExpTy(NULL, x->u.func.returnTipo);
			} else {
				EM_error(exp->pos, "funcao indefinida: %s", S_name(exp->u.chama_func.func));
				return createExpTy(NULL, Ty_Int());
			}
		}

    case A_chamaProcExp: {
			A_lstExp args = NULL;
			Ty_tyList parametros;
			E_enventry x = S_look(venv, exp->u.chama_proc.proc);
      TRAD_exp translation = Trad_NoExp();

			if (x && x->tipo == E_procEntry) {
				parametros = x->u.proc.parametros;
				for (args = exp->u.chama_proc.lstExp; args && parametros; args = args->lstExp, parametros = parametros->tail) {
					struct expty arg = tradExp(listAllExp, escopo, venv, tenv, args->exp);
					if (!is_equal_ty(arg.ty, parametros->head))
						EM_error(args->exp->pos, "tipo incorreto %s; esperado: %s",
							Ty_ToString(arg.ty), Ty_ToString(parametros->head));
				}

        translation = Trad_ChamaProc(CHPR, x->u.proc.label, escopo);
        Trad_ExpList_append(listAllExp, translation);

				if (args == NULL && parametros != NULL)
					EM_error(exp->pos, "Estao faltando argumentos!");
				else if (args != NULL && parametros == NULL)
					EM_error(exp->pos, "Muitos argumentos, esperado menos");
				return createExpTy(Trad_NoExp(), Ty_Void());
			} else {
				EM_error(exp->pos, "procedimento indefinido: %s", S_name(exp->u.chama_func.func));
				return createExpTy(Trad_NoExp(), Ty_Void());
			}
		}

		case A_atribExp: {
      E_enventry x = S_look(venv, exp->u.atrib.var->id);
			struct expty var = tradVar(listAllExp, escopo, venv, tenv, exp->u.atrib.var);
			struct expty newExp = tradExp(listAllExp, escopo, venv, tenv, exp->u.atrib.exp);
      TRAD_exp translation = Trad_NoExp();
			
			if (!is_equal_ty(var.ty, newExp.ty)) {
        if(x->tipo == E_funcEntry) {
          EM_error(exp->u.atrib.exp->pos, "tipo do retorno invalido: esperado o tipo %s, mas retornou o tipo %s",
              Ty_ToString(var.ty), Ty_ToString(newExp.ty));   
        } else {
          EM_error(exp->u.atrib.exp->pos, "atribuicao invalida: esperado o tipo %s, mas recebeu o tipo %s",
              Ty_ToString(var.ty), Ty_ToString(newExp.ty));   
        }
      }

      if(x->tipo == E_varEntry) {
        translation = Trad_CmdManipValor(ARMZ, x->u.var.escopo, x->u.var.endRelativo);
        Trad_ExpList_append(listAllExp, translation);
      } else if (x->tipo == E_funcEntry) {
        translation = Trad_CmdManipValor(ARMZ, x->u.func.escopo, x->u.func.endRelativo);
        Trad_ExpList_append(listAllExp, translation);
      }

			return createExpTy(translation, Ty_Void());
		}

    case A_ifExp: {
			struct expty test, then, elsee;
			test = tradExp(listAllExp, escopo, venv, tenv, exp->u.iff.test);
      TRAD_exp translation = Trad_NoExp();
      Label labelAfterIf = newlabel();
      Label elseLabel = NULL;

			if (test.ty->kind != Ty_bool) {
				EM_error(exp->u.iff.test->pos, "Boolean necessario!");
      }
      if (exp->u.iff.elsee) {
        elseLabel = newlabel();
        translation = Trad_CmdDesvio(DSVF, elseLabel);
        Trad_ExpList_append(listAllExp, translation);
      }

			then = tradExp(listAllExp, escopo, venv, tenv, exp->u.iff.then);

      translation = Trad_CmdDesvio(DSVS, labelAfterIf);
      Trad_ExpList_append(listAllExp, translation); 

			if (exp->u.iff.elsee) {
        InsertLabel(listAllExp, elseLabel, Trad_NoExp());
				elsee = tradExp(listAllExp, escopo, venv, tenv, exp->u.iff.elsee);

				if (!is_equal_ty(then.ty, elsee.ty)) {
					EM_error(exp->u.iff.elsee->pos, "Branches do if-then-else devem retornar o mesmo tipo");
        }
			} else {
			}

      InsertLabel(listAllExp, labelAfterIf, Trad_NoExp());
			return createExpTy(NULL, then.ty);
		}

    case A_whileExp: {
      TRAD_exp translation = Trad_NoExp();
      Label whileLabel = newlabel();
      Label labelAfterWhile = newlabel();

      InsertLabel(listAllExp, whileLabel, Trad_NoExp());
			struct expty test = tradExp(listAllExp, escopo, venv, tenv, exp->u.whilee.test);
      translation = Trad_CmdDesvio(DSVF, labelAfterWhile);
      Trad_ExpList_append(listAllExp, translation);

			if (test.ty->kind != Ty_bool)
				EM_error(exp->u.whilee.test->pos, "Boolean necessario!");

			struct expty body = tradExp(listAllExp, escopo, venv, tenv, exp->u.whilee.body);

      translation = Trad_CmdDesvio(DSVS, whileLabel);
      Trad_ExpList_append(listAllExp, translation);

      InsertLabel(listAllExp, labelAfterWhile, Trad_NoExp());

			if (body.ty->kind != Ty_void)
				EM_error(exp->u.whilee.body->pos, "Nao deve ser produzido nenhum valor");
			return createExpTy(NULL, Ty_Void());
		}

    case A_cmdComp: {
      struct expty newExpty;
      A_lstExp listaExp = NULL;
			TRAD_expList list = Trad_ExpList();

      for (listaExp = exp->u.cmd_cmp.lstCmd; listaExp; listaExp = listaExp->lstExp) {
        newExpty = tradExp(listAllExp, escopo, venv, tenv, listaExp->exp);
      }

      return newExpty;
		}

    assert(0);
  }
}

struct expty tradVar(TRAD_expList listAllExp, Escopo escopo, S_table venv, S_table tenv, A_var var) {
  TRAD_exp translation = Trad_NoExp();
  E_enventry x = S_look(venv, var->id);

  if (x && x->tipo == E_varEntry) {
    translation = Trad_SimpleVar(x);
    return createExpTy(translation, x->u.var.varTipo);
  } else if (x && x->tipo == E_funcEntry) {
    return createExpTy(NULL, x->u.func.returnTipo);
  } else {
    EM_error(var->pos, "Variavel indefinida %s", S_name(var->id));
    return createExpTy(translation, Ty_Int());
  }

  assert(0);
}

void tradDecVar(TRAD_expList listAllExp, Escopo escopo, int* endRelativo, S_table venv, S_table tenv, A_decVar dec) {
			if (dec->tipo) {
				Ty_ty type = S_look_ty(tenv, dec->tipo);
				if (!type) {
					EM_error(dec->pos, "Tipo invalido %s", S_name(dec->tipo));
					type = Ty_Void();
				}
							
				S_enter(venv, dec->id, E_VarEntry(escopo, *endRelativo, dec->id, type));

        *endRelativo = *endRelativo + 1;
			} else {
				EM_error(dec->pos, "Tipo indefinido para a variavel %s", S_name(dec->id));
			}
}

void tradDecFunc(TRAD_expList listAllExp, Escopo escopo, S_table venv, S_table tenv, A_lstDecFunc dec) {
  A_lstDecFunc funList;
  Ty_tyList tiposParams;
  Ty_ty resultTy;
  TRAD_exp translation = Trad_NoExp();
  Label funcLabel = newlabel();

  for (funList = dec; funList; funList = funList->prox) {
    resultTy = NULL;
    int qntParams = 0;
    A_lstDecVar parametros;

    if (funList->funcDec->returnType) {
      resultTy = S_look(tenv, funList->funcDec->returnType);
      if (!resultTy)
        EM_error(funList->funcDec->pos, "Tipo do retorno invalido");
    } else {
        EM_error(funList->funcDec->pos, "Tipo do retorno indefinido");
    }
    if (!resultTy) resultTy = Ty_Void();

    tiposParams = makeParamsTipos(tenv, funList->funcDec->params);
    for (parametros = funList->funcDec->params; parametros; parametros = parametros->prox) {
      qntParams++;
    }

    int endRelativo = -(4 + qntParams - 1);

    Escopo funEscopo = local;
    S_enter(venv, funList->funcDec->id, E_FuncEntry(funEscopo, (-qntParams-4), funcLabel, tiposParams, resultTy));

    translation = Trad_InitProc(ENPR, funEscopo);
    InsertLabel(listAllExp, funcLabel, translation);

    E_enventry funEntry = S_look(venv, funList->funcDec->id);

    S_beginScope(venv);
    Ty_tyList paramTys = funEntry->u.func.parametros;
    
    for (parametros = funList->funcDec->params; parametros; 
      parametros = parametros->prox,
      paramTys = paramTys->tail) {
      S_enter(venv, parametros->decVar->id, E_VarEntry(local, endRelativo, parametros->decVar->id, paramTys->head));
      endRelativo++;
    }

    tradBloco(listAllExp, funEntry->u.func.escopo, venv, tenv, funList->funcDec->bloco, FALSE);

    translation = Trad_EndProc(RTPR, funEntry->u.func.escopo, qntParams);
    Trad_ExpList_append(listAllExp, translation);

    S_endScope(venv);
  }
}

void tradDecProc(TRAD_expList listAllExp, Escopo escopo, S_table venv, S_table tenv, A_lstDecProc dec) {
  A_lstDecProc procList;
  Ty_tyList paramsTipos;
  Ty_ty resultTy;
  TRAD_exp translation = Trad_NoExp();
  Label procLabel = newlabel();

  for (procList = dec; procList; procList = procList->prox) {
    int qntParams = 0;
    A_lstDecVar parametros;

    paramsTipos = makeParamsTipos(tenv, procList->procDec->params);
    Escopo procEscopo = local;
    S_enter(venv, procList->procDec->id, E_ProcEntry(procEscopo, procLabel, paramsTipos));

    for (parametros = procList->procDec->params; parametros; parametros = parametros->prox) {
      qntParams++;
    }

    int endRelativo = -(4 + qntParams - 1);

    translation = Trad_InitProc(ENPR, procEscopo);
    InsertLabel(listAllExp, procLabel, translation);

    E_enventry procEntry = S_look(venv, procList->procDec->id);

    S_beginScope(venv);
    Ty_tyList paramTys = procEntry->u.proc.parametros;

    for (parametros = procList->procDec->params; parametros; parametros = parametros->prox, paramTys = paramTys->tail) {
      S_enter(venv, parametros->decVar->id, E_VarEntry(local, endRelativo, parametros->decVar->id, paramTys->head));
      endRelativo++;
    }

    tradBloco(listAllExp, procEntry->u.proc.escopo, venv, tenv, procList->procDec->bloco, FALSE);

    translation = Trad_EndProc(RTPR, procEntry->u.proc.escopo, qntParams);
    Trad_ExpList_append(listAllExp, translation);

    S_endScope(venv);
  }
}

