#include "includes/geraCodigoMepa.h"

void geraCmdSimples(FILE *out, TRAD_cmdSimples cmd) {
  fprintf(out, "%s\n", MEPA_cmdSimples[cmd]);
}

void geraCmdMem(FILE *out, TRAD_mem cmd, int qtdMem) {
  char cmdMem[4] = "";

  strcpy(cmdMem, MEPA_mem[cmd]);

  fprintf(out, "%s %d\n", cmdMem, qtdMem);
}

void geraCmdManipValor(FILE *out, TRAD_cmdManipValor cmd, Escopo escopo, int endRelativo) {
  char cmdManipValor[4] = "";

  strcpy(cmdManipValor, MEPA_cmdManipValor[cmd]);

  fprintf(out, "%s %d %d\n", cmdManipValor, escopo, endRelativo);
}

void geraCmdCarregaCT(FILE *out, TRAD_carregaCT cmd, int constante) {
  char cmdCarregaCt[4] = "";

  strcpy(cmdCarregaCt, MEPA_carregaCT[cmd]);

  fprintf(out, "%s %d\n", cmdCarregaCt, constante);
}

void geraCmdDesvio(FILE *out, TRAD_cmdDesvio cmd, Label label) {
  char cmdDesvio[4] = "";

  strcpy(cmdDesvio, MEPA_cmdDesvio[cmd]);

  fprintf(out, "%s %s\n", cmdDesvio, S_name(label));
}

void geraCmdInitProc(FILE *out, TRAD_cmdProc cmd, Escopo escopo) {
  char cmdProc[4] = "";

  strcpy(cmdProc, MEPA_cmdProc[cmd]);

  fprintf(out, "%s %d\n", cmdProc, escopo);
}

void geraCmdEndProc(FILE *out, TRAD_cmdProc cmd, Escopo escopo, int qntParams) {
  char cmdProc[4] = "";

  strcpy(cmdProc, MEPA_cmdProc[cmd]);

  fprintf(out, "%s %d %d\n", cmdProc, escopo, qntParams);
}

void geraCmdChamaProc(FILE *out, TRAD_cmdProc cmd, Escopo escopo, Label label) {
  char cmdProc[4] = "";

  strcpy(cmdProc, MEPA_cmdProc[cmd]);

  fprintf(out, "%s %s %d\n", cmdProc, S_name(label), escopo);
}

void geraCmdNewLabel(FILE *out, Label label) {
  fprintf(out, "%s:\n", S_name(label));
}

void geraCodigoMepaCases(FILE *out, TRAD_exp exp) {
  switch (exp->tipo) {
    case TRAD_CMDSIMPLES:
      geraCmdSimples(out, exp->u.cmdSimples);
    break;

    case TRAD_MEM:
      geraCmdMem(out, exp->u.MEM.cmdMem, exp->u.MEM.qtdMem);
    break;

    case TRAD_MANIPVALOR:
      geraCmdManipValor(out, exp->u.MANIPVALOR.cmdManip, exp->u.MANIPVALOR.escopo, exp->u.MANIPVALOR.endRelativo);
    break;

    case TRAD_CARREGACT:
      geraCmdCarregaCT(out, exp->u.CARREGACT.cmdCRCT, exp->u.CARREGACT.constante);
    break;

    case TRAD_DESVIO:
      geraCmdDesvio(out, exp->u.DESVIO.cmdDesvio, exp->u.DESVIO.label);
    break;

    case TRAD_INITPROC:
      geraCmdInitProc(out, exp->u.INITPROC.cmdProc, exp->u.INITPROC.escopo);
    break;

    case TRAD_ENDPROC:
      geraCmdEndProc(out, exp->u.ENDPROC.cmdProc, exp->u.ENDPROC.escopo, exp->u.ENDPROC.qntParams);
    break;

    case TRAD_CHAMAPROC:
      geraCmdChamaProc(out, exp->u.CHAMAPROC.cmdProc, exp->u.CHAMAPROC.escopo, exp->u.CHAMAPROC.label);
    break;
   
    case NEWLABEL:
      geraCmdNewLabel(out, exp->u.INSERTLABEL.newLabel);
      geraCodigoMepaCases(out, exp->u.INSERTLABEL.exp);
    break;

    case TRAD_NOEXP:
      geraCmdSimples(out, exp->u.cmdSimples);
    break;
    
    default:

    break;

    }
}

void geraCodigoMepa(FILE *out, TRAD_expList listaExp) {
  for (TRAD_node lista = listaExp->head; lista; lista = lista->next) {
    geraCodigoMepaCases(out, lista->expr);
  }
}