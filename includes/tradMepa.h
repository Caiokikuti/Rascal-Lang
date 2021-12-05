#ifndef TRADMEPA_H__
#define TRADMEPA_H__

#include "label.h"
#include "env.h"

typedef struct TRAD_exp_ *TRAD_exp;
typedef struct TRAD_node_ *TRAD_node;
typedef struct TRAD_expList_ *TRAD_expList;
struct TRAD_expList_ {TRAD_node head; TRAD_node tail;};

static char MEPA_cmdSimples[][12] = {
  "INPP",
  "PARA",
  "NADA",
  "LEIT",
  "IMPR",
	"SOMA",
  "SUBT",
  "MULT",
  "DIVI",
  "CONJ",
  "DISJ",
  "CMME",
  "CMMA",
  "CMIG",
  "CMDG",
  "CMEG",
  "CMAG",
};

static char MEPA_mem[][12] = {
  "AMEM",
  "DMEM",
};

static char MEPA_cmdManipValor[][12] = {
  "CRVL",
  "ARMZ",
};

static char MEPA_chamaProc[][12] = {
  "CHPR",
  "ENPR",
  "RTPR",
};

static char MEPA_carregaCT[][12] = {
  "CRCT",
};

static char MEPA_cmdDesvio[][12] = {
  "DSVS",
  "DSVF",
};

typedef enum {
  INPP,
  PARA,
  NADA,
  LEIT,
  IMPR,
	SOMA,
  SUBT,
  MULT,
  DIVI,
  CONJ,
  DISJ,
  CMME,
  CMMA,
  CMIG,
  CMDG,
  CMEG,
  CMAG,
} TRAD_cmdSimples ;

typedef enum  {
  AMEM,
  DMEM,
} TRAD_mem;

typedef enum  {
  CRVL,
  ARMZ,
} TRAD_cmdManipValor;

typedef enum  {
  CHPR,
  ENPR,
  RTPR,
} TRAD_chamaProc;

typedef enum  {
  CRCT,
} TRAD_carregaCT;

typedef enum  {
  DSVS,
  DSVF,
} TRAD_cmdDesvio;

struct TRAD_exp_ {
	enum {
		TRAD_CMDSIMPLES,
    TRAD_MEM,
    TRAD_MANIPVALOR,
    TRAD_CARREGACT,
    TRAD_DESVIO,
    TRAD_NOEXP,
    TRAD_CHAMAPROC,
    SIMPLEVAR,
    NEWLABEL,
	} tipo;
	union {
    TRAD_cmdSimples cmdSimples;
		E_enventry simpleVar;

    struct {
      Label newLabel;
      TRAD_exp exp;
    } INSERTLABEL;

		struct {
			TRAD_mem cmdMem;
      int qtdMem;
    } MEM;

		struct {
			TRAD_cmdManipValor cmdManip;
      Escopo escopo;
      int endRelativo;
    } MANIPVALOR;

    struct {
      TRAD_carregaCT cmdCRCT;
      int constante;
    } CARREGACT;

    struct {
      TRAD_cmdDesvio cmdDesvio;
      Label label;
    } DESVIO;

		struct {
			TRAD_chamaProc cmdChamaProc;
      Escopo escopo;
      int endRelativo;
    } CHAMAPROC; // not ready
  } u;
};

struct TRAD_node_ {
	TRAD_exp expr;
	TRAD_node next;
};

Escopo Tr_global();
Escopo Tr_local();

TRAD_exp Trad_NoExp();
TRAD_exp InitProg();
TRAD_exp EndProg();
TRAD_exp Trad_SimpleVar(E_enventry simpleVar);
TRAD_exp Trad_CmdSimples(TRAD_cmdSimples cmd);
TRAD_exp Trad_Mem(TRAD_mem cmd, int qtdMem);
TRAD_exp Trad_CmdManipValor(TRAD_cmdManipValor cmd, Escopo escopo, int endRelativo);
TRAD_exp Trad_CarregaCT(TRAD_carregaCT cmd, int constante);
TRAD_exp Trad_CmdDesvio(TRAD_cmdDesvio cmd, Label label);
TRAD_exp Trad_ChamaProc(); // not ready
TRAD_expList Trad_ExpList();
void InsertLabel(TRAD_expList list, Label label, TRAD_exp expr);
void Trad_ExpList_append(TRAD_expList list, TRAD_exp expr);
void Trad_ExpList_prepend(TRAD_expList list, TRAD_exp expr);

#endif