/**********************************************************************/
/*****                    Répertoire téléphonique                   ***/
/**********************************************************************/
/*                                                                    */
/*       rep.c                                                        */
/*       VERSION 21.0                                                 */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/**********************************************************************/
/* REMARQUE: le fichier de données est au format suivant :            */
/*      nom;prenom;no_de_telephone1                                   */
/*      nom2;prenom2;no_de_telephone2                                 */
/*      ...                                                           */
/* soit un enregistrement par ligne de 3 champs separés par ';'        */
/* Si on utilise un autre fichier de donnees que rep.txt, il faut     */
/* passer son nom en paramètre à l'appel du programme                 */
/**********************************************************************/

#include <stdio.h>   /* pour les entrées-sorties */
#include <string.h>  /* pour les manipulations de chaînes de caractères */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <fcntl.h>
#include "rep.h"

/* fichier texte de sauvegarde des contacts */
char nom_fichier[MAX_FILE_NAME];

char mess1[] = "\nn'existe pas dans le répertoire.\n";
char mess2[] = "\nPlus d'autre %s dans le répertoire\n";


bool modif = false;

/********************************************************************************/
/* initialisation d'un répertoire                                               */
/* prend en paramètre un pointeur vers le répertoire à initialiser              */
/* intialise le nombre d'éléments à zéro, charge le fichier et le trie          */
/********************************************************************************/

int init_rep(Directory* rep)
{
	/* fonction complète : ne pas modifier  */
	int errno;
	void* tmpPtr;
	rep->elts_count = 0;
	rep->sorted = true; /* un répertoire vide est trié :-) */
#ifdef IMPL_TAB
						  // code pour tableau

	tmpPtr = (void*)malloc(MAX_RECORDS * sizeof(Record));
	if (tmpPtr != NULL) {
		rep->tab = (Record*)tmpPtr;
	}
	else return(-1);
#else
#ifdef IMPL_LIST
						  // code pour Liste
	tmpPtr = (LinkedList*)NewLinkedList();
	if (tmpPtr != NULL) {
		rep->list = (LinkedList*)tmpPtr;
	}
	else return(-1);
#endif
#endif
	errno = charger(rep, nom_fichier);

	if ((errno == OK) && (rep->elts_count > 0))
	{
		rep->sorted = false;
		trier(rep);
	}
	return(0);
} /* fin init_rep */

  /**************************************************************************/
  /*  Lecture d'une chaîne de caractères depuis le clavier.                 */
  /*  Elle est stockée dans le tableau de char passé en paramètre           */
  /*   La longueur max à lire est donnée en second paramètre                */
  /*   Elle renvoie la longueur de la chaine et une longueur négative s'il  */
  /*   y a eu un problème                                                   */
  /**************************************************************************/
int saisie_chaine(unsigned char c[], int max)
{
	/* fonction complète : ne pas modifier */

	/* longueur de la chaîne c */
	int l;

	/* saisie en rangeant dans tableau c */
	if (fgets(c, max, stdin) == NULL)          /* lecture des caractères sur entrée standard= clavier */
		return -1;
	/* s'il y a une erreur, on renvoie -1                  */

	l = strlen(c);							   /* calcul de la longueur de la chaine */

	if (c[l - 1] == '\n')					   /* suppression du retour chariot            */
	{									       /* en fin de chaîne s'il est présent        */
		c[l - 1] = '\0';                       /* écriture d'un caractère de fin de chaine */
		l--;                                   /* à la place                               */
	}
	return l;								   /* on retourne la longueur de */
} /* fin saisie_chaine */

  /**************************************************************************/
  /* entrée au clavier d'un enregistrement (contact)                        */
  /* l'élément saisi est stocké dans l'enregistrement pointé par le         */
  /* parametre enr                                                          */
  /* la fonction renvoie OK si l'élément est correctement saisie et         */
  /* ERROR s'il y a eu un problème...                                       */
  /**************************************************************************/
int saisie_enreg(Record* enr)
{
	unsigned char tmp[MAX_INPUT_CHAR];
	int l;

	printf("Nom :");
	if ((l = saisie_chaine(tmp, MAX_INPUT_CHAR)) < 0)
		return ERROR;
	/* chaine vide ? */
	if (l == 0)
	{
		printf("nom vide...\n");
		return ERROR;
	}
	/* chaine trop longue ? */
	if (l >= MAX_LASTNAME)
	{
		printf("nom tronqué...\n");

	}
	/* on copie dans le champ nom... */
	strncpy_s(enr->nom, _countof(enr->nom), tmp, _TRUNCATE);

	/* Entrée clavier du prénom */
	printf("Prénom :");
	if ((l = saisie_chaine(tmp, MAX_INPUT_CHAR)) < 0)
		return ERROR;
	/* chaine vide ? */
	if (l == 0)
	{
		printf("prénom vide...\n");
		return ERROR;

	}
	/* chaine trop longue ? */
	if (l >= MAX_LASTNAME)
	{
		printf("prénom trop long...\n");

	}
	/* on copie dans le champ prenom... */
	strncpy_s(enr->prenom, _countof(enr->prenom) - 1, tmp, _TRUNCATE);

	/* Entrée clavier du numéro de téléphone */
	printf("Téléphone :");
	if ((l = saisie_chaine(tmp, MAX_INPUT_CHAR)) < 0)
		return ERROR;
	/* chaine vide ? */
	if (l == 0)
	{
		printf("téléphone vide...\n");
		return ERROR;

	}
	/* chaine trop longue ? */
	if (l >= MAX_TEL)
	{
		printf("téléphone tronqué...\n");

	}
	/* on copie dans le champ tel... */
	strncpy_s(enr->tel, _countof(enr->tel), tmp, _TRUNCATE);

	return OK;
} /* fin saisie_enreg */


  /********************************************************************************/
  /*  Affichage paginé du répertoire par façon commande DOS 'more'                 */
  /********************************************************************************/
void affichage_repertoire(Directory* rep)
{
	int idx = 0;							/* Index sur enregistrement courant */
	int cpt = LINES_PER_PAGE;				/* Compteur de ligne affichées */
	char key = 0;							/* Controle de l'affichage */

	trier(rep);								/* trie du répertoire avant affichage */
#ifdef IMPL_LIST
	// on place un pointeur sur la tête de liste,
	// elle est peut-être vide
	SingleLinkedListElem* currentElement = rep->list->head;
#endif
	// tant qu'il y a un élément
	while ((idx < rep->elts_count) && (key != 'X'))
	{
		if (cpt > 0)						/* S'il reste des lignes à afficher */
		{								/* dans la page */
#ifdef IMPL_TAB
			affichage_enreg_frmt(rep->tab[idx]);	/* Affichage enrg courant */

#else
#ifdef IMPL_LIST
			affichage_enreg_frmt(currentElement->pers);	/* Affichage enrg courant */
			currentElement = currentElement->next; /* on passe au suivant */
#endif
#endif

			idx++;							/* On passe au suivant */
			cpt--;							/* Une ligne de moins a afficher */
		}
		else
		{	/* si page complétement affichée, on attend la suite */
			printf("\n\n_____________________________ entrée/espace/x ___ :\n");

			do
			{
				key = (char)toupper(_getch());
				cpt = (key == 13) ? 1 : LINES_PER_PAGE;
			} while ((key != ' ') && (key != 13) && (key != 'X'));
		}

	}
} /* fin affichage_repertoire */


  /**********************************************************************/
  /* Recherche par no de téléphone avec compactage du no                */
  /**********************************************************************/
int rechercher_tel(Directory* rep, char tel[], int ind)
{
	int i = ind; // indice de début de recherche
	int ind_fin;

	char tmp_tel[MAX_TEL];
	char tmp_tel2[MAX_TEL];
	bool trouve = false;

	ind_fin = rep->elts_count - 1; // indice de fin à ne pas dépasser
	strncpy_s(tmp_tel, _countof(tmp_tel), tel, _TRUNCATE);
	compact(tmp_tel); // nettoyage du numéro

#ifdef IMPL_LIST
					  // on se place sur l'élément en ième position s'il existeg
	SingleLinkedListElem* currentElement = GetElementAt(rep->list, i);
	while ((currentElement != NULL) && (!trouve))
	{
		strncpy_s(tmp_tel2, _countof(tmp_tel2), currentElement->pers.tel, _TRUNCATE);
		compact(tmp_tel2);
		if (strcmp(tmp_tel, tmp_tel2) == 0)
			trouve = true;
		else {
			// si pas trouvé, on passe au suivant
			currentElement = currentElement->next;
			i++;
		}
	}
#else
#ifdef IMPL_TAB
	while ((i <= ind_fin) && (!trouve))
	{

		strncpy_s(tmp_tel2, _countof(tmp_tel2), rep->tab[i].tel, _TRUNCATE);



		compact(tmp_tel2);
		if (strcmp(tmp_tel, tmp_tel2) == 0)
			trouve = true;
		else
			i++;
	}
#endif // IMPL_TAB
#endif

	return((trouve) ? i : -1);
} /* fin rechercher_tel */


  /***********************************************************************************************************/
  /*                                                                                                         */
  /*  Lecture de champs contenus dans les lignes du fichier                                                  */
  /*  - ligne: pointeur sur un buffer qui contient la ligne lue dans le fichier et terminée par un zéro     */
  /*  - idx: pointeur sur un entier qui indique la position courante de la recherche dans la ligne           */
  /*  - champ: pointeur sur la variable à renseigner avec les informations trouvées entre deux séparateurs   */
  /*  - taille_champ : la taille du champ à ne pas dépasser                                                  */
  /*  - separateur : le caractère utilisé comme séparateur                                                   */
  /***********************************************************************************************************/

int lire_champ_suivant(char* ligne, int* idx, char* champ, int taille_champ,
	char separateur)
{

	int idx2 = 0;

	while ((idx2 < (taille_champ - 2)) && (ligne[*idx] != separateur)
		&& (ligne[*idx] != '\0'))
	{
		champ[idx2] = ligne[*idx];
		idx2 += 1;
		*idx += 1;
	}
	if ((ligne[*idx] == separateur) || (ligne[*idx] == '\0'))
	{
		champ[idx2] = 0;	/* fin de chaine sur caractere suivant */
		return(OK);
	}
	else return(ERROR);		/* fin de ligne ou séparateur non atteints */

} /* fin


  () */


  /**********************************************************************/
  /*    Affichage du sous menu rechercher                               */
  /**********************************************************************/

void afficher_menu_recherche()
{
	printf("\n\n Recherche :\n\n");
	printf("\n\t par (N)om");
	printf("\n\t par (T)éléphone");
	printf("\n\t\te(X)it\n");
	return;
}
/**********************************************************************/
/*                                                                    */
/* traiter_rechercher proposer de supprimer l element trouve          */
/* et demande s'il faut poursuivre ou  non la recherche               */
/**********************************************************************/

bool traiter_recherche(Directory* rep, int pos)
{
	char key = 0;
#ifdef IMPL_LIST
	// on récupère l'élément en ième position
	SingleLinkedListElem* currentElement = GetElementAt(rep->list, pos);
	if (currentElement != NULL)
		affichage_enreg(currentElement->pers);
#else
#ifdef IMPL_TAB
	affichage_enreg(rep->tab[pos]);
#endif
#endif
	printf("\n\n_____________________________ /espace/(D)elete/e(X)it ___ :\n");
	do
	{
		key = (char)toupper(_getch());
	} while ((key != ' ') && (key != 'D') && (key != 'X'));

	switch (key)
	{
	case ' ':
		return(true);
		break;
	case 'X':
		return(false);
		break;
	case 'D':
#ifdef IMPL_TAB
		supprimer_un_contact_dans_rep(rep, pos);
#else
#ifdef IMPL_LIST
		supprimer_un_contact_dans_rep_liste(rep, currentElement);
#endif
#endif
		break;
	}
	return(false);

}
/**********************************************************************/
/*  Traite l'option rechercher du menu principal                      */
/**********************************************************************/


void option_rechercher(Directory* rep)
{
	char choix;

	int ind;			/* Indice de debut de la recherche ds tableau */
	int pos;			/* Position ds le tableau ou l'element a ete trouve */
	char nom[MAX_LASTNAME];	/* Les chaines recherchees */
	char tel[MAX_TEL];
	bool suite = false;			/* true si recherche du suivant, false si on arrete*/

	if (rep->elts_count <= 0)
	{
		printf("\nRépertoire vide ...\n");
		return;
	}
	afficher_menu_recherche();
	do
	{

		ind = 0;						/* la recherche reprend depuis le debut du tableau */
		suite = false;				/**/
		do
		{						/* lecture touche frappee au clavier */
			choix = (char)toupper(_getch());
		} while ((choix != 'N') && (choix != 'T') && (choix != 'X'));

		switch (choix)
		{
		case 'N':
			printf("\nNom ? :");
			saisie_chaine(nom, MAX_LASTNAME);


			do
			{
				pos = rechercher_nom(rep, nom, ind);

				if (pos >= 0)
				{
					ind = pos + 1;
					suite = traiter_recherche(rep, pos);
				}
				else
				{
					if (suite == true) printf(mess2, nom);	/* Plus de nouvelle occurence */
					else printf(mess1);						/* Pas d'occurence trouvee la premiere fois*/
					suite = false;								/* On arrete la recherche */
				}

			} while (suite);
			break;
		case 'T':
			printf("\nTel ? :");
			saisie_chaine(tel, MAX_TEL);

			do
			{
				pos = rechercher_tel(rep, tel, ind);

				if (pos >= 0)
				{
					ind = pos + 1;
					suite = traiter_recherche(rep, pos);
				}
				else
				{
					if (suite == true) printf(mess2, tel);	/* Plus de nouvelle occurence */
					else printf(mess1);						/* Pas d'occurence trouvee la premiere fois*/
					suite = false;								/* On arrete la recherche */
				}
			} while (suite);
			break;
		}
		afficher_menu_recherche();
	} while (choix != 'X');

	return;
}

/**********************************************************************/
/* ajoute  un contact au répertoire                                   */
/**********************************************************************/

void option_ajouter(Directory* rep)
{
	Record tmpenr;
	if (saisie_enreg(&tmpenr))
		if (!ajouter_un_contact_dans_rep(rep, tmpenr))
			printf("\nAjout impossible !! \n");

	return;
}
/**********************************************************************/
/*  Demande de saisie d'un chemin de fichier						  */
/**********************************************************************/

int saisir_chemin(char* message, char* buffer)
{
	printf("\n%s\n", message);
	saisie_chaine(buffer, MAX_FILE_NAME);
	printf("\n");
	return(OK);
}



/**********************************************************************/
/* Affichage du menu principal                                        */
/**********************************************************************/

void afficher_menu_principal()
{
	printf("\n\n Répertoire :");
	printf("\n\n\t(A)fficher tout");
	printf("\n\ta(J)outer une personne");
	printf("\n\trecherche(R) une personne");
	printf("\n\n\t\te(X)it\n");
}

/**********************************************************************/


