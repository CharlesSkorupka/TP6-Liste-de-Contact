#pragma once
#include <stdbool.h>
#include "liste.h"


// #define VERSION 21.0



//#define IMPL_TAB  // si défini, on compile pour une implémentation tableau du répertoire
#define IMPL_LIST // si défini, on compile pour un implémentation Liste Chaînée du répertoire

#ifdef IMPL_LIST
#undef IMPL_TAB
#endif

#define MAX_RECORDS 1000  /* nbre max d'enregistrements */
#define MAX_INPUT_CHAR 1024 /* longueur max pour la saisie d'une chaîne
ATTENTION, il faut que MAX_SAISIE > MAX_NOM et
MAX_SAISIE > MAX_TEL */

#define OK    1
#define ERROR 0

#define SEPARATOR ';'
#define LINES_PER_PAGE 5
#define MAX_FILE_NAME 80


/**********************************************************************/
/*** Déclarations des types                                         ***/
/***                                                                ***/
/**********************************************************************/



/* Directory : structure qui contient :                          */
/* le nombre de contacts                                         */
/* le tableau de contacts,                                       */
/* sorted : qui vaut VRAI si le tableau est trié et FAUX sinon   */
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

// initialise le répertoire passé en paramètre et fait appel à charger() pour charger le fichier de contacts
int init_rep(Directory *rep);

// lit les lignes dans le fichier de contacts et les analyse avec la fonction lire_champ_suivant()
int charger(Directory *rep, char nom_fichier[]);

// Affiche le menu utilisateur principal
void afficher_menu_principal();

void option_ajouter(Directory *dir); // Traitement de l'option ajout d'un contact depuis le menu principal

// Ajout d'un contact dans le répertoire stocké en mémoire
int ajouter_un_contact_dans_rep(Directory *rep, Record enr); 

// Suppression d'un contact, utilisé notamment par traiter_recherche() afin de supprimer un contact recherché et trouvé
void supprimer_un_contact_dans_rep(Directory *dir, int index); 


// Suppression d'un contact pour implémentation avec une liste chaînée
int supprimer_un_contact_dans_rep_liste(Directory *dir, SingleLinkedListElem *elem);


int saisie_chaine(unsigned char c[], int max);  // entree d'une chaine de caractères quelconque au clavier, un nom par exemple
int saisie_enreg(Record *rec); // fait appel à saisie_chaine pour l'entree d'un nouveau contact
void affichage_enreg(Record rec); // affiche un seul enregistrement sur une ligne
void affichage_enreg_frmt(Record rec); // affiche un seul enregistrement avec des largeurs de colonnes fixes
void affichage_repertoire(Directory *dir); // affichage de tout le répertoire en paginant les lignes à la façon de la commande 'MORE'
bool est_sup(Record rec1, Record rec2); // test si un contact se trouve après dans l'ordre alphabétique par rapport à un autre contact
void trier(Directory *dir);  // tri des contacts du répertoire en mémoire

void compact(char *s); // suppression de tous les caratères non numériques dans une chaine (0..9) afin de faciliter la recherche par no de tel par ex.
int lire_champ_suivant(char *ligne, int *idx, char *champ, int field_size, char separator); // analyse d'une ligne lue dans le fichier et stockée dans le buffer 'ligne' 

void option_rechercher(Directory *rep); //traitement de l'option de recherche du menu principal
void afficher_menu_recherche();  //affiche un menu afin de proposer une recherche par nom ou par no tel ou, d'abandonner la recherche
int rechercher_nom(Directory *dir, char lastname[], int ind); // recherche l'existance d'un contact et renvoie l'indice de sa position si trouvé ou -1 sinon
int rechercher_tel(Directory *dir, char tel[], int ind); // recherche l'existance d'un contact par no tel et renvoie l'indice de sa position si trouvé ou -1 sinon
bool traiter_recherche(Directory *dir, int pos); // menu qui propose de supprimer le contact trouvé, de poursuivre la recherche ou de s'arreter 

int saisir_chemin(char *message, char *buffer); // demande de saisie au clavier d'un chemin pour un fichier de sauvegarde, fait appel à saisie_chaine()
int sauvegarder(Directory *dir, char file_name[]); // sauvegarde tout le répertoire en mémoire vers un fichier en remplaçant l'ancien fichier



