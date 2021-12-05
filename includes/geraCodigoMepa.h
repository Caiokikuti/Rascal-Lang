#ifndef GERACODIGOMEPA_H__
#define GERACODIGOMEPA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "tradMepa.h"
#include "label.h"

void geraCodigoMepa(FILE *out, TRAD_expList listaExp);
void geraCodigoMepaCases(FILE *out, TRAD_exp exp);

void geraCmdSimples(FILE *out, TRAD_cmdSimples cmd);
void geraCmdMem(FILE *out, TRAD_mem cmd, int qtdMem);
void geraCmdManipValor(FILE *out, TRAD_cmdManipValor cmd, Escopo escopo, int endRelativo);
void geraCmdInitProc(FILE *out, TRAD_cmdProc cmd, Escopo escopo);
void geraCmdEndProc(FILE *out, TRAD_cmdProc cmd, Escopo escopo, int qntParams);
void geraCmdChamaProc(FILE *out, TRAD_cmdProc cmd, Escopo escopo, Label label);
void geraCmdCarregaCT(FILE *out, TRAD_carregaCT cmd, int constante);
void geraCmdDesvio(FILE *out, TRAD_cmdDesvio cmd, Label label);
void geraCmdNewLabel(FILE *out, Label label);

#endif