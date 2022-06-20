/**********************************************************************/
/*****                    R�pertoire t�l�phonique                   ***/
/**********************************************************************/
/*                                                                    */
/*       contactsDirectory.c                                                        */
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

/* flag de modification du r�pertoire : indique si le tableau de contacts �t� modifi�         */
/* Permet notamment de d�cider s'il faut sauvegarder le r�pertoire dans un fichier             */
extern bool modif;

extern char nom_fichier[MAX_FILE_NAME];
extern char mess1[];
extern char mess2[];


/*****************************************************************************/
/*                        programme principal : main                         */
/*****************************************************************************/

void main(int argc, char* argv[])
{
	

	char choix;						/* caract�re de l'option choisie dans menu utilisateur */

	Directory repertoire;			/* variable qui d�clare et cr�e le r�pertoire en m�moire */
	system("CHCP 1252");
	printf("\nsetlocale : %s\n", setlocale(LC_ALL, "fr-FR")); /* option de localisation pour les caract�res accentu�s  */


	if (argc == 1)
	{
		strcpy_s(nom_fichier, _countof(nom_fichier), "rep.txt");	/* nom du fichier par defaut */
	}
	else
	{
		strcpy_s(nom_fichier, _countof(nom_fichier), (char*)argv[1]);	/* nom du fichier utilisateur */
	}
	/* intialise le nombre d'�l�ments � z�ro, alloue la m�moire du r�pertoire,
	charge le fichier et le trie */
	if (init_rep(&repertoire) < 0) return;

	afficher_menu_principal();

	do								/* lecture menu principal */
	{							/* tant que pas demande pour Quitter */
		do
		{						/* lecture touche frappee au clavier */
			choix = (char)toupper(_getch());
		} while ((choix != 'A') && (choix != 'X') && (choix != 'J') && (choix != 'R'));

		switch (choix)
		{
		case 'A':
			affichage_repertoire(&repertoire);
			break;
		case 'J':
			option_ajouter(&repertoire);
			break;
		case 'R':
			option_rechercher(&repertoire);
			break;
		}
		afficher_menu_principal();
	} while (choix != 'X');

	if (modif)					/* Si modification apportee au repertoire */
	{						/* on propose une sauvegarde */
		printf("\n Enregistrer modification ? O/N");
		do
		{
			choix = (char)toupper(_getch());
		} while ((choix != 'O') && (choix != 'N'));

		if (choix == 'O')
		{
			printf("\n Enregistrer sous %s ? O/N", nom_fichier);
			do
			{
				choix = (char)toupper(_getch());
			} while ((choix != 'O') && (choix != 'N'));

			if (choix == 'N') saisir_chemin("Nom du fichier: ?", nom_fichier);
			sauvegarder(&repertoire, nom_fichier);
		}
	}
	printf("\nBye !\n\n");


	return;
}
/*****************************FIN du programme principal*********************/
