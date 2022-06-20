#pragma once
#include "personne.h"
// LISTES UNILATERES **************************************************
// #define VERSION 21.0


#define UP 0
#define DOWN 1


typedef struct SingleLinkedListElem {
	Record pers;
	struct SingleLinkedListElem *next;
} SingleLinkedListElem;

typedef struct LinkedList {
	SingleLinkedListElem *head;
	int size;
	SingleLinkedListElem *tail;
} LinkedList;


// Cr�e une liste cha�n�e unilat�re vide et renvoie un pointeur sur celle-ci
LinkedList *NewLinkedList();

// Cr�e un nouveau maillon qui contient une nouvelle personne et renvoie un pointeur sur ce maillon
SingleLinkedListElem  *NewLinkedListElement(Record pers);

// Renvoie un pointeur sur le maillon qui se trouve en i�me position � partir de 0
SingleLinkedListElem *GetElementAt(LinkedList *list, int i);

// Ajoute une nouvelle personne dans la liste cha�n�e en i�me position
// Cette fonction fait appel � la fonction NewLinkedListElement(Record pers) pour cr�er un maillon

int InsertElementAt(LinkedList *list, int i, Record pers);

// Suppression et destruction d'un �l�ment de liste chain�e
int DeleteLinkedListElem(LinkedList *list, SingleLinkedListElem *Elem);

