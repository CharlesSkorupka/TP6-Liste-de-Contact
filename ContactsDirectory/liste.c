#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include "liste.h"

// #define VERSION 21.0

 /*********************************************************************/
// crée une nouvelle liste chainée unilatère vide et renvoie un pointeur sur cette liste
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
// crée un nouveau maillon qui contient la personne passée en paramètre
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
// renvoie un pointeur sur l'élément en ième position dans la liste
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
// Ajoute une nouvelle personne dans la liste chaînée en ième position
// Cette fonction fait appel à la fonction NewLinkedListElement(Record pers) pour créer un maillon
int InsertElementAt(LinkedList* list, int i, Record pers) {

	// Ajouter code ici
	// Debut ajout code
	//je creer une nouvelle liste et je lui donne le parametre pers
	//ma liste chainé est crée et initialisé
	SingleLinkedListElem* newElement = NewLinkedListElement(pers);


	if (newElement == NULL) return DOWN;

	//je teste toutes les possiblilités
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
		list->tail->next = newElement; //j'augmente ma liste chainé
		list->tail = newElement;
		list->size += UP;
		return UP;
	}

	SingleLinkedListElem* tmp = list->head; // trie à bulle
	SingleLinkedListElem* before = NULL;
	int compt = 0;

	while (compt != i && compt < list->size)
	{
		//trie à bulle
		before = tmp;
		tmp = tmp->next;
		compt += UP;

		// placer la valeur à la bonne case correspondant à i; 
		if (compt = i)
		{
			newElement->next = before->next;
			before->next = tmp;
			list->size += UP;
			return UP;
		}
	}


	// fin ajout de code
	// pas d'élément inséré !
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

	// pas d'élément dans la liste ou elem invalide
	if ((list->size == 0) || (elem == NULL)) return(0);

	// initialisation d'un pointeur sur l'élément courant
	SingleLinkedListElem* tmp = list->head;
	// previous désigne l'élément qui précède l'élément courant
	SingleLinkedListElem* previous = NULL;

	// l'élément est en tête et en queue, il y a donc 1 seul élément dans la liste
	if ((elem == list->head) && (elem == list->tail)) {

		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		free(elem);
		return(1);
	}

	// il est en tête, on supprime la tête

	if (elem == list->head) {

		list->head = elem->next;
		list->size--;
		free(elem);
		return(1);
	}

	// Recherche du maillon dans le reste de la liste chaînée

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
	// s'il est au milieu, on supprime l'élément
	if ((previous != NULL) && (tmp == elem) && (tmp->next != NULL)) {

		previous->next = elem->next;
		list->size--;
		free(elem);
		return(1);
	}
	// l'élément recherché n'a pas été trouvé
	return(0);
}

