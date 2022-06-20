/**********************************************************************/
/*****                    R�pertoire t�l�phonique                   ***/
/**********************************************************************/
/*                                                                    */
/*       rep.c                                                        */
/*       VERSION 21.0                                                 */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/**********************************************************************/
/* REMARQUE: le fichier de donn�es est au format suivant :            */
/*      nom;prenom;no_de_telephone1                                   */
/*      nom2;prenom2;no_de_telephone2                                 */
/*      ...                                                           */
/* soit un enregistrement par ligne de 3 champs separ�s par ';'        */
/* Si on utilise un autre fichier de donnees que rep.txt, il faut     */
/* passer son nom en param�tre � l'appel du programme                 */
/**********************************************************************/

#include <stdio.h>   /* pour les entr�es-sorties */
#include <string.h>  /* pour les manipulations de cha�nes de caract�res */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <fcntl.h>
#include "rep.h"

/* fichier texte de sauvegarde des contacts */
char nom_fichier[MAX_FILE_NAME];

char mess1[] = "\nn'existe pas dans le r�pertoire.\n";
char mess2[] = "\nPlus d'autre %s dans le r�pertoire\n";


bool modif = false;

/********************************************************************************/
/* initialisation d'un r�pertoire                                               */
/* prend en param�tre un pointeur vers le r�pertoire � initialiser              */
/* intialise le nombre d'�l�ments � z�ro, charge le fichier et le trie          */
/********************************************************************************/

int init_rep(Directory* rep)
{
	/* fonction compl�te : ne pas modifier  */
	int errno;
	void* tmpPtr;
	rep->elts_count = 0;
	rep->sorted = true; /* un r�pertoire vide est tri� :-) */
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
  /*  Lecture d'une cha�ne de caract�res depuis le clavier.                 */
  /*  Elle est stock�e dans le tableau de char pass� en param�tre           */
  /*   La longueur max � lire est donn�e en second param�tre                */
  /*   Elle renvoie la longueur de la chaine et une longueur n�gative s'il  */
  /*   y a eu un probl�me                                                   */
  /**************************************************************************/
int saisie_chaine(unsigned char c[], int max)
{
	/* fonction compl�te : ne pas modifier */

	/* longueur de la cha�ne c */
	int l;

	/* saisie en rangeant dans tableau c */
	if (fgets(c, max, stdin) == NULL)          /* lecture des caract�res sur entr�e standard= clavier */
		return -1;
	/* s'il y a une erreur, on renvoie -1                  */

	l = strlen(c);							   /* calcul de la longueur de la chaine */

	if (c[l - 1] == '\n')					   /* suppression du retour chariot            */
	{									       /* en fin de cha�ne s'il est pr�sent        */
		c[l - 1] = '\0';                       /* �criture d'un caract�re de fin de chaine */
		l--;                                   /* � la place                               */
	}
	return l;								   /* on retourne la longueur de */
} /* fin saisie_chaine */

  /**************************************************************************/
  /* entr�e au clavier d'un enregistrement (contact)                        */
  /* l'�l�ment saisi est stock� dans l'enregistrement point� par le         */
  /* parametre enr                                                          */
  /* la fonction renvoie OK si l'�l�ment est correctement saisie et         */
  /* ERROR s'il y a eu un probl�me...                                       */
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
		printf("nom tronqu�...\n");

	}
	/* on copie dans le champ nom... */
	strncpy_s(enr->nom, _countof(enr->nom), tmp, _TRUNCATE);

	/* Entr�e clavier du pr�nom */
	printf("Pr�nom :");
	if ((l = saisie_chaine(tmp, MAX_INPUT_CHAR)) < 0)
		return ERROR;
	/* chaine vide ? */
	if (l == 0)
	{
		printf("pr�nom vide...\n");
		return ERROR;

	}
	/* chaine trop longue ? */
	if (l >= MAX_LASTNAME)
	{
		printf("pr�nom trop long...\n");

	}
	/* on copie dans le champ prenom... */
	strncpy_s(enr->prenom, _countof(enr->prenom) - 1, tmp, _TRUNCATE);

	/* Entr�e clavier du num�ro de t�l�phone */
	printf("T�l�phone :");
	if ((l = saisie_chaine(tmp, MAX_INPUT_CHAR)) < 0)
		return ERROR;
	/* chaine vide ? */
	if (l == 0)
	{
		printf("t�l�phone vide...\n");
		return ERROR;

	}
	/* chaine trop longue ? */
	if (l >= MAX_TEL)
	{
		printf("t�l�phone tronqu�...\n");

	}
	/* on copie dans le champ tel... */
	strncpy_s(enr->tel, _countof(enr->tel), tmp, _TRUNCATE);

	return OK;
} /* fin saisie_enreg */


  /********************************************************************************/
  /*  Affichage pagin� du r�pertoire par fa�on commande DOS 'more'                 */
  /********************************************************************************/
void affichage_repertoire(Directory* rep)
{
	int idx = 0;							/* Index sur enregistrement courant */
	int cpt = LINES_PER_PAGE;				/* Compteur de ligne affich�es */
	char key = 0;							/* Controle de l'affichage */

	trier(rep);								/* trie du r�pertoire avant affichage */
#ifdef IMPL_LIST
	// on place un pointeur sur la t�te de liste,
	// elle est peut-�tre vide
	SingleLinkedListElem* currentElement = rep->list->head;
#endif
	// tant qu'il y a un �l�ment
	while ((idx < rep->elts_count) && (key != 'X'))
	{
		if (cpt > 0)						/* S'il reste des lignes � afficher */
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
		{	/* si page compl�tement affich�e, on attend la suite */
			printf("\n\n_____________________________ entr�e/espace/x ___ :\n");

			do
			{
				key = (char)toupper(_getch());
				cpt = (key == 13) ? 1 : LINES_PER_PAGE;
			} while ((key != ' ') && (key != 13) && (key != 'X'));
		}

	}
} /* fin affichage_repertoire */


  /**********************************************************************/
  /* Recherche par no de t�l�phone avec compactage du no                */
  /**********************************************************************/
int rechercher_tel(Directory* rep, char tel[], int ind)
{
	int i = ind; // indice de d�but de recherche
	int ind_fin;

	char tmp_tel[MAX_TEL];
	char tmp_tel2[MAX_TEL];
	bool trouve = false;

	ind_fin = rep->elts_count - 1; // indice de fin � ne pas d�passer
	strncpy_s(tmp_tel, _countof(tmp_tel), tel, _TRUNCATE);
	compact(tmp_tel); // nettoyage du num�ro

#ifdef IMPL_LIST
					  // on se place sur l'�l�ment en i�me position s'il existeg
	SingleLinkedListElem* currentElement = GetElementAt(rep->list, i);
	while ((currentElement != NULL) && (!trouve))
	{
		strncpy_s(tmp_tel2, _countof(tmp_tel2), currentElement->pers.tel, _TRUNCATE);
		compact(tmp_tel2);
		if (strcmp(tmp_tel, tmp_tel2) == 0)
			trouve = true;
		else {
			// si pas trouv�, on passe au suivant
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
  /*  - ligne: pointeur sur un buffer qui contient la ligne lue dans le fichier et termin�e par un z�ro     */
  /*  - idx: pointeur sur un entier qui indique la position courante de la recherche dans la ligne           */
  /*  - champ: pointeur sur la variable � renseigner avec les informations trouv�es entre deux s�parateurs   */
  /*  - taille_champ : la taille du champ � ne pas d�passer                                                  */
  /*  - separateur : le caract�re utilis� comme s�parateur                                                   */
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
	else return(ERROR);		/* fin de ligne ou s�parateur non atteints */

} /* fin


  () */


  /**********************************************************************/
  /*    Affichage du sous menu rechercher                               */
  /**********************************************************************/

void afficher_menu_recherche()
{
	printf("\n\n Recherche :\n\n");
	printf("\n\t par (N)om");
	printf("\n\t par (T)�l�phone");
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
	// on r�cup�re l'�l�ment en i�me position
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
		printf("\nR�pertoire vide ...\n");
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
/* ajoute  un contact au r�pertoire                                   */
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
	printf("\n\n R�pertoire :");
	printf("\n\n\t(A)fficher tout");
	printf("\n\ta(J)outer une personne");
	printf("\n\trecherche(R) une personne");
	printf("\n\n\t\te(X)it\n");
}

/**********************************************************************/


