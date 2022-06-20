#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include "liste.h"

// #define VERSION 21.0

 /*********************************************************************/
// cr�e une nouvelle liste chain�e unilat�re vide et renvoie un pointeur sur cette liste
LinkedList* NewLinkedList() {
	LinkedList* tmp;
	tmp = (LinkedList*)malloc(sizeof(LinkedList));

	if (tmp != NULL) {
		tmp->head = NULL;
		tmp->tail = NULL;
		tmp->size = DOWN;
	}
	return tmp;
}
/*********************************************************************/
// cr�e un nouveau maillon qui contient la personne pass�e en param�tre
SingleLinkedListElem* NewLinkedListElement(Record pers) {
	SingleLinkedListElem* tmp;
	tmp = (SingleLinkedListElem*)malloc(sizeof(SingleLinkedListElem));
	if (tmp != NULL) {
		tmp->pers = pers;
		tmp->next = NULL;
	}
	return(tmp);
}
/*********************************************************************/
// renvoie un pointeur sur l'�l�ment en i�me position dans la liste
SingleLinkedListElem* GetElementAt(LinkedList* Liste, int i) {
	int currentIndex = DOWN;
	SingleLinkedListElem* element;
	if ((Liste == NULL) || (i < DOWN) || (i >= Liste->size)) return(NULL);

	if (i == DOWN) return(Liste->head);

	if (i == Liste->size - UP) return(Liste->tail);
	element = Liste->head;

	while (currentIndex != i && element != NULL)
	{
		element = element->next;
		currentIndex++;
	}
	return(element);
}
/*********************************************************************/
// Ajoute une nouvelle personne dans la liste cha�n�e en i�me position
// Cette fonction fait appel � la fonction NewLinkedListElement(Record pers) pour cr�er un maillon
int InsertElementAt(LinkedList* list, int i, Record pers) {

	// Ajouter code ici
	// Debut ajout code
	//je creer une nouvelle liste et je lui donne le parametre pers
	//ma liste chain� est cr�e et initialis�
	SingleLinkedListElem* newElement = NewLinkedListElement(pers);


	if (newElement == NULL) return DOWN;

	//je teste toutes les possiblilit�s
	if (list->tail == NULL)
	{
		list->tail = newElement;
		list->head = newElement;
		list->size = UP;
		return UP;
	}

	if (i == DOWN)
	{
		newElement->next = list->head;
		list->head = newElement;
		list->size += UP;
		return DOWN;
	}
	if (i >= list->size)
	{
		list->tail->next = newElement; //j'augmente ma liste chain�
		list->tail = newElement;
		list->size += UP;
		return UP;
	}

	SingleLinkedListElem* tmp = list->head; // trie � bulle
	SingleLinkedListElem* before = NULL;
	int compt = 0;

	while (compt != i && compt < list->size)
	{
		//trie � bulle
		before = tmp;
		tmp = tmp->next;
		compt += UP;

		// placer la valeur � la bonne case correspondant � i; 
		if (compt = i)
		{
			newElement->next = before->next;
			before->next = tmp;
			list->size += UP;
			return UP;
		}
	}


	// fin ajout de code
	// pas d'�l�ment ins�r� !
	return(0);
}
/*********************************************************************/
int DeleteLinkedListElem(LinkedList* list, SingleLinkedListElem* elem) {

	// on peut traiter un certain nombre de cas d'erreur

	// La liste n'existe pas
	if (list == NULL) return(0);

	// liste vide ou anomalie
	if ((list->head == NULL) || (list->tail == NULL)) return(0);

	// anomalie sur la taille
	if ((list->head == list->tail) && (list->size != 1)) return(0);

	// pas d'�l�ment dans la liste ou elem invalide
	if ((list->size == 0) || (elem == NULL)) return(0);

	// initialisation d'un pointeur sur l'�l�ment courant
	SingleLinkedListElem* tmp = list->head;
	// previous d�signe l'�l�ment qui pr�c�de l'�l�ment courant
	SingleLinkedListElem* previous = NULL;

	// l'�l�ment est en t�te et en queue, il y a donc 1 seul �l�ment dans la liste
	if ((elem == list->head) && (elem == list->tail)) {

		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		free(elem);
		return(1);
	}

	// il est en t�te, on supprime la t�te

	if (elem == list->head) {

		list->head = elem->next;
		list->size--;
		free(elem);
		return(1);
	}

	// Recherche du maillon dans le reste de la liste cha�n�e

	while ((tmp != NULL) && (tmp != elem)) {
		previous = tmp;
		tmp = tmp->next;
	}

	// s'il est en queue, on supprime la queue
	if ((previous != NULL) && (tmp == elem) && (tmp->next == NULL)) {

		list->tail = previous;
		previous->next = NULL;
		list->size--;
		free(elem);
		return(1);
	}
	// s'il est au milieu, on supprime l'�l�ment
	if ((previous != NULL) && (tmp == elem) && (tmp->next != NULL)) {

		previous->next = elem->next;
		list->size--;
		free(elem);
		return(1);
	}
	// l'�l�ment recherch� n'a pas �t� trouv�
	return(0);
}

