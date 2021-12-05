// #include "includes/translate.h"
#include "includes/translate.h"

Escopo Tr_global() {
  return global;
}

Escopo Tr_local() {
  return local;
}

Tr_expList Tr_ExpList() {
	Tr_expList list = checked_malloc(sizeof(*list));
	list->head = NULL;
	list->tail = NULL;

	return list;
}

void Tr_ExpList_append(Tr_expList list, Tr_exp expr) {
	if (list->head) {
		Tr_node node = checked_malloc(sizeof(*node));
		node->expr = expr;
		node->next = NULL;
		list->tail->next = node;
		list->tail = list->tail->next;
	} else Tr_ExpList_prepend(list, expr);
}

void Tr_ExpList_prepend(Tr_expList list, Tr_exp expr) {
	if (list->head) {
		Tr_node node = checked_malloc(sizeof(*node));
		node->expr = expr;
		node->next = list->head;
		list->head = node;
	} else {
		list->head = checked_malloc(sizeof(*list->head));
		list->head->expr = expr;
		list->head->next = NULL;
		list->tail = list->head;
	}
}