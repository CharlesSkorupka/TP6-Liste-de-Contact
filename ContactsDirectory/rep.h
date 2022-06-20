#pragma once
#include <stdbool.h>
#include "liste.h"


// #define VERSION 21.0



//#define IMPL_TAB  // si d�fini, on compile pour une impl�mentation tableau du r�pertoire
#define IMPL_LIST // si d�fini, on compile pour un impl�mentation Liste Cha�n�e du r�pertoire

#ifdef IMPL_LIST
#undef IMPL_TAB
#endif

#define MAX_RECORDS 1000  /* nbre max d'enregistrements */
#define MAX_INPUT_CHAR 1024 /* longueur max pour la saisie d'une cha�ne
ATTENTION, il faut que MAX_SAISIE > MAX_NOM et
MAX_SAISIE > MAX_TEL */

#define OK    1
#define ERROR 0

#define SEPARATOR ';'
#define LINES_PER_PAGE 5
#define MAX_FILE_NAME 80


/**********************************************************************/
/*** D�clarations des types                                         ***/
/***                                                                ***/
/**********************************************************************/



/* Directory : structure qui contient :                          */
/* le nombre de contacts                                         */
/* le tableau de contacts,                                       */
/* sorted : qui vaut VRAI si le tableau est tri� et FAUX sinon   */
typedef struct Directory
{
	int elts_count;
#ifdef IMPL_TAB
	Record *tab;
#else
#ifdef IMPL_LIST
	LinkedList *list;
#endif
#endif
	bool sorted;
} Directory;

// initialise le r�pertoire pass� en param�tre et fait appel � charger() pour charger le fichier de contacts
int init_rep(Directory *rep);

// lit les lignes dans le fichier de contacts et les analyse avec la fonction lire_champ_suivant()
int charger(Directory *rep, char nom_fichier[]);

// Affiche le menu utilisateur principal
void afficher_menu_principal();

void option_ajouter(Directory *dir); // Traitement de l'option ajout d'un contact depuis le menu principal

// Ajout d'un contact dans le r�pertoire stock� en m�moire
int ajouter_un_contact_dans_rep(Directory *rep, Record enr); 

// Suppression d'un contact, utilis� notamment par traiter_recherche() afin de supprimer un contact recherch� et trouv�
void supprimer_un_contact_dans_rep(Directory *dir, int index); 


// Suppression d'un contact pour impl�mentation avec une liste cha�n�e
int supprimer_un_contact_dans_rep_liste(Directory *dir, SingleLinkedListElem *elem);


int saisie_chaine(unsigned char c[], int max);  // entree d'une chaine de caract�res quelconque au clavier, un nom par exemple
int saisie_enreg(Record *rec); // fait appel � saisie_chaine pour l'entree d'un nouveau contact
void affichage_enreg(Record rec); // affiche un seul enregistrement sur une ligne
void affichage_enreg_frmt(Record rec); // affiche un seul enregistrement avec des largeurs de colonnes fixes
void affichage_repertoire(Directory *dir); // affichage de tout le r�pertoire en paginant les lignes � la fa�on de la commande 'MORE'
bool est_sup(Record rec1, Record rec2); // test si un contact se trouve apr�s dans l'ordre alphab�tique par rapport � un autre contact
void trier(Directory *dir);  // tri des contacts du r�pertoire en m�moire

void compact(char *s); // suppression de tous les carat�res non num�riques dans une chaine (0..9) afin de faciliter la recherche par no de tel par ex.
int lire_champ_suivant(char *ligne, int *idx, char *champ, int field_size, char separator); // analyse d'une ligne lue dans le fichier et stock�e dans le buffer 'ligne' 

void option_rechercher(Directory *rep); //traitement de l'option de recherche du menu principal
void afficher_menu_recherche();  //affiche un menu afin de proposer une recherche par nom ou par no tel ou, d'abandonner la recherche
int rechercher_nom(Directory *dir, char lastname[], int ind); // recherche l'existance d'un contact et renvoie l'indice de sa position si trouv� ou -1 sinon
int rechercher_tel(Directory *dir, char tel[], int ind); // recherche l'existance d'un contact par no tel et renvoie l'indice de sa position si trouv� ou -1 sinon
bool traiter_recherche(Directory *dir, int pos); // menu qui propose de supprimer le contact trouv�, de poursuivre la recherche ou de s'arreter 

int saisir_chemin(char *message, char *buffer); // demande de saisie au clavier d'un chemin pour un fichier de sauvegarde, fait appel � saisie_chaine()
int sauvegarder(Directory *dir, char file_name[]); // sauvegarde tout le r�pertoire en m�moire vers un fichier en rempla�ant l'ancien fichier



