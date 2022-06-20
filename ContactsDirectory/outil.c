#include <stdio.h>   /* pour les entr�es-sorties */
#include <string.h>  /* pour les manipulations de cha�nes de caract�res */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include "rep.h"

#define VERSION 21.0

/**************************************************************************/
/* Compl�ter votre nom ici                                                */
/*   Nom : Skorupka                 Pr�nom : Charles                      */
/**************************************************************************/

extern bool modif;



/// <summary>
/// Ajout d'un contact dans le r�pertoire stock� en m�moire
/// </summary>
/// <param name="dir">: Ref : le repertoire</param>
/// <param name="rec">: Value : le contact</param>
/// <returns>ERROR si �chec ou OK si ajout r�alis�</returns>
int ajouter_un_contact_dans_rep(Directory *dir, Record rec)
{
#ifdef IMPL_TAB
	// ajouter code ici pour tableau

	// Debut ajout code

	dir->tab[dir->elts_count] = rec; //on ajoute rec dans notre tableau
	dir->elts_count++; //on ajoute un �l�ment

	// fin ajout de code

	
#else
#ifdef IMPL_LIST

	bool inserted = false;

	if (dir->elts_count == 1) {
		//il n'y a pas de pb , je rentre mon tableau de contact et j'increment de 1
		if (InsertElementAt(dir->list, dir->list->size, rec) != 0) {
			dir->elts_count += 1;
			modif = true;
			dir->sorted = true;
			return(OK);
		}
	}
	else {
		//aucun �l�ment est rentr�, je commence donc � la case head
		SingleLinkedListElem* NeWList = GetElementAt(dir->list, 0);
		int pos = DOWN;
		// on cherche un point d'insertion
		while (!inserted && NeWList != NULL) {
			// ajouter code ici pour Liste
			// Debut ajout code

			if (est_sup(rec, NeWList->pers)){

				InsertElementAt(dir->list, pos, rec);// insert l'�l�ment dans la liste a un emplacement vide
				modif = true; // on a modifier la liste 
				dir->sorted = true; 
				inserted = true; 
				dir->elts_count += 1; // On ajoute un �l�ment a la liste

			}

			pos += 1; // on incr�mente la position
			NeWList = NeWList->next;

			// fin ajout de code
		}

		// c'est le plus grand, on le place � la fin
		if (!inserted) {

			InsertElementAt(dir->list, dir->list->size, rec);
			inserted = true;
			dir->elts_count++;
			dir->sorted = true;
		}
	}

#endif

#endif
	return(OK);

} /* fin ajout */
  /**********************************************************************/
  /* supprime du r�pertoire l'enregistrement dont l'indice est donn� en */
  /*   param�tre       et place modif = true                            */
  /**********************************************************************/
#ifdef IMPL_TAB
void supprimer_un_contact_dans_rep(Directory *dir, int index) {

	
	if (dir->elts_count >= 1)		/* s'il y a au moins un element ds le tableau */
	{
	// ajouter code ici pour tableau						
	// Debut ajout code

		int i = 0; 

		while (i < dir->elts_count) { //parcour tout le tableau 
			dir->tab[index + i] = dir->tab[index + 1 + i];//remplace le contact[index] par le prochain contact (contact[adresse+1])
			i++;
		}
		dir->elts_count--;

	// fin ajout de code
		modif = true;
	}

	return;
} /* fin supprimer */

#else
#ifdef IMPL_LIST
  
  // ajouter code ici pour Liste

	/// <summary>
    /// supprime du r�pertoire l'enregistrement contenu dans le maillon elem
    /// et fixe modif � vrai
	/// 
	/// </summary>
	/// <param name="dir">: Ref vers un r�pertoire</param>
	/// <param name="elem">: Ref vers un maillon de liste chain�e</param>
	/// <returns>0 si pas de suppression effectu�e ou 1 si une suppression r�alis�e</returns>
int supprimer_un_contact_dans_rep_liste(Directory* dir, SingleLinkedListElem* elem) {

	// Ajouter code ici pour impl�mentation liste
	// D�but ajout Code ici

		// ajouter code ici pour tableau						
		// Debut ajout code

	if (DeleteLinkedListElem(dir->list, elem)) //deleteElem  renvoie 1 si la valeur � pu etre supp
	{
		dir->elts_count -= 1; //1 element en moins dans la liste
		modif = true; // on a modifier la liste 
		return 1;
	}

		// fin ajout de code
		// Fin ajout de code

	return DOWN;
}
#endif
#endif


/*********************************************************************/
/// <summary>
/// Affichage d'un enregistrement sur une ligne � l'�cran
/// </summary>
/// <param name="rec"> : Contact � afficher</param>
void affichage_enreg(Record enr)
{
	// ex Dupont, Jean                 0320304050
	// l'espacement est obtenu avec des caract�res de tabulation
	// D�but ajout Code ici

	printf("%10s \t %10s \t %10s", enr.nom, enr.prenom, enr.tel); //affiche les �l�ments dans le bon format 

	// Fin ajout de code
} /* fin affichage_enreg */

 /*********************************************************************/

/// <summary>
/// Affichage d'un enregistrement avec alignement des mots
/// lors d'un affichage sous la forme d'une liste
/// </summary>
/// <param name="rec"> : Contact � afficher</param>
void affichage_enreg_frmt(Record enr)
{
	// Le nom est align� sur 30 caract�res
	// le pr�nom est align� sur 25 caract�res
	// le no de t�l�phone est align� sur 15 caract�res
	// ex | Dupont                |Jean                  |0320304050      
	// 
	// D�but ajout Code ici

	printf("|%-30s \t |%-25s \t |%-5s\n", enr.nom, enr.prenom, enr.tel);//affiche les �l�ments dans le bon format 

	// Fin ajout de code
} /* fin affichage_enreg_frmt */


 /*********************************************************************/
/// <summary>
/// Teste si dans l'ordre alphabetique, un enregistrement est apres un autre
/// </summary>
/// <param name="enr1">: Contact 1</param>
/// <param name="enr2">: Contact 2</param>
/// <returns>true si enr1 est apr�s enr2</returns>
bool est_sup(Record enr1, Record enr2)
{

	// Utiliser la fonction _stricmp pour comparer des chaines de caract�res
	// 
	// D�but ajout Code ici


	if (_stricmp(enr1.nom, enr2.nom) > 0) {
		return(true); 
	}


	// Fin ajout code
	return(false);
	


}
 
/*********************************************************************/
/*   Tri Alphabetique du tableau d'enregistrements                   */
/*********************************************************************/

void trier(Directory *dir)
{

#ifdef IMPL_TAB
	// ajouter code ici pour tableau
	// Debut ajout code

	
	Record tmp;
	Record* test = dir->tab->nom;

	for (int i = 0; i < dir->elts_count; i++)
	{
		if (est_sup(dir->tab[i], dir->tab[i + 1]) == true)
		{
			tmp = test[i];
			test[i] = test[i + 1];
			test[i + 1] = tmp;
		}
	}


	// fin ajout de code
	
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	// rien � faire !
	// la liste est toujours tri�e
#endif
#endif


	dir->sorted = true;

} /* fin trier */

  /**********************************************************************/
  /* recherche dans le r�pertoire d'un enregistrement correspondant au  */
  /*   nom � partir de l'indice ind                                     */
  /*   retourne l'indice de l'enregistrement correspondant au crit�re ou*/
  /*   un entier n�gatif si la recherche est n�gative				    */
  /**********************************************************************/

int rechercher_nom(Directory *dir, char nom[], int ind)
{
	int i = ind;		    /* position (indice) de d�but de recherche dans tableau/liste dir */
	

	char tmp_nom[MAX_LASTNAME];	/* 2 variables temporaires dans lesquelles */
	char tmp_nom2[MAX_LASTNAME];	/* on place la chaine recherchee et la chaine lue dans le */
							/* tableau, afin de les convertir en majuscules et les comparer */
	bool trouve = false;		


#ifdef IMPL_TAB
							// ajouter code ici pour tableau
	int ind_fin;			/* position (indice) de fin de tableau/liste dir */
	ind_fin = dir->elts_count - 1;
	strcpy_s(tmp_nom,_countof(tmp_nom), nom);
	_strupr_s(tmp_nom, _countof(tmp_nom));

	while ((i <= ind_fin) && (!trouve))
	{
		strcpy_s(tmp_nom2, _countof(tmp_nom2), dir->tab[i].nom);
		_strupr_s(tmp_nom2, _countof(tmp_nom2));
		if (strcmp(tmp_nom, tmp_nom2) == 0)
			trouve = true;						/* bingo! */
		else
			i++;					/* pas trouve, on continue */
	}	
	//printf("Trouve ? : %d", trouve); 
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	// Debut ajout code


	int ind_fin = dir->list->size-1;	//taille de la liste

	strcpy_s(tmp_nom, _countof(tmp_nom), nom);
	_strupr_s(tmp_nom, _countof(tmp_nom));


	while ((i <= ind_fin) && (!trouve))
	{
		LinkedList* tmp = dir;
		SingleLinkedListElem* ListeTmp = GetElementAt(tmp , i);  // on cr�e une liste temporaire qui va parcourir tout nos �l�ments
		//SingleLinkedListElem* ListeTmp = GetElementAt(dir->list, i);  // on cr�e une liste temporaire qui va parcourir tout nos �l�ments
		strcpy_s(tmp_nom2, _countof(tmp_nom2), ListeTmp->pers.nom); 
		_strupr_s(tmp_nom2, _countof(tmp_nom2));

		if (_stricmp(tmp_nom, tmp_nom2) == 0) {
			trouve = true;  //on a trouv� l'�l�ment
		}
		else {
			i++; // on ne trouve pas l'�l�ment donc on passe au prochain 
		}
	}
	// fin ajout de code
#endif
#endif

	return((trouve) ? i : -1);
} /* fin rechercher_nom */

  /*********************************************************************/
  /* Supprimer tous les caracteres non num�riques de la chaines        */
  /*********************************************************************/
void compact(char *s)
{
// Debut ajout code
	int i = 0; 
	int j = 0; 
	int taille = 0; 

	taille = strlen(s); 

	for (i; i < taille; i++) { //parcour la chaine de caract�re 
		if (!isdigit(s[i])) { //rep�re si le caract�re n'est pas un chiffre 
			for (j = i; j <= taille - 1; j++) { //d�cale les cases du tableau pour effacer le caract�re non reconu 
				s[j] = s[j + 1]; 
			}
			taille = taille - 1; //on d�cale le tableau donc changement de taille 
			i = i - 1; //on analyse la m�me case car le tableau a �tait d�cal� 
		}
	}

// fin ajout de code

	return;
}

/**********************************************************************/
/* sauvegarde le r�pertoire dans le fichier dont le nom est pass� en  */
/* argument                                                           */
/* retourne OK si la sauvegarde a fonctionn� ou ERROR sinon           */
/**********************************************************************/
int sauvegarder(Directory *dir, char file_name[])
{
	FILE *fic_rep;					/* le fichier */
	int err; 
#ifdef IMPL_TAB
	// ajouter code ici pour tableau
	// Debut ajout codedo


	err = fopen_s(&fic_rep, file_name, "w"); // ouverture d'un fichier en �criture-remplacement
	 //je r�-�cris tout dans le fichier // 

	//test si mon fichier est bien ouvert
	if (fic_rep != NULL)
	{
		//je re-�cris tout les �l�ments de mon fichier
		for (int i = 0; i < dir->elts_count; i++)
		{
			fprintf(fic_rep, "%s;%s;%s\n", dir->tab[i].nom, dir->tab[i].prenom, dir->tab[i].tel);
			//fputs("%c |%c\t |%c", *(dir->tab + i)->nom , *(dir->tab+ i)->prenom, *(dir->tab + i)->tel, &fic_rep);
		}
		//je ferme mon fichier une fois que tout est finis
		fclose(fic_rep);
	}
	// fin ajout de code
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste

	// Debut ajout code

	err = fopen_s(&fic_rep, file_name, "w");//ouvrre le fichier en �criture
	if (fic_rep != NULL)//si on a un ffichier
	{
		for (int i = 0; i < dir->elts_count; i++)
		{
			fprintf(fic_rep, "%s;%s;%s\n", GetElementAt(dir->list, i)->pers.nom, GetElementAt(dir->list, i)->pers.prenom, GetElementAt(dir->list, i)->pers.tel);//�crit dans le fichier
		}
	}
	fclose(fic_rep);//termin� donc fermeture du fichier


	// fin ajout de code
#endif
#endif

	return(OK);
} /* fin sauvegarder */


  /**********************************************************************/
  /*   charge dans le r�pertoire le contenu du fichier dont le nom est  */
  /*   pass� en argument                                                */
  /*   retourne OK si le chargement a fonctionn� et ERROR sinon         */
  /**********************************************************************/
int charger(Directory* dir, char nom_fichier[])
{
	FILE* fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Record);
	char buffer[sizeof(Record) + 1];
	int idx = 0;
	Record ListeTmp; //structure temporaire qui stock les �l�ments a ajouter

	char* char_nw_line;

	_set_errno(0);
	if (((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) || (fic_rep == NULL))
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep) && (dir->elts_count < MAX_RECORDS))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
#ifdef IMPL_TAB
				if (lire_champ_suivant(buffer, &idx, dir->tab[num_rec].nom, MAX_LASTNAME, SEPARATOR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, dir->tab[num_rec].prenom, MAX_LASTNAME, SEPARATOR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, dir->tab[num_rec].tel, MAX_TEL, SEPARATOR) == OK)
							num_rec++;		/* element � priori correct, on le comptabilise */
					}
				}
#else
#ifdef IMPL_LIST
				// ajouter code implemention liste
				// **** d�but ajout ****
				//M�me d�marche que pour les tableaux en l'adaptant au listes
				if (lire_champ_suivant(buffer, &idx, ListeTmp.nom, MAX_LASTNAME, SEPARATOR) == OK)
				{
					idx++;							/* on saute le separateur */

					if (lire_champ_suivant(buffer, &idx, ListeTmp.prenom, MAX_LASTNAME, SEPARATOR) == OK)
					{
						idx++;

						if (lire_champ_suivant(buffer, &idx, ListeTmp.tel, MAX_TEL, SEPARATOR) == OK)
							InsertElementAt(dir->list, num_rec, ListeTmp);
						num_rec++;		/* element � priori correct, on le comptabilise */
					}

				}
				// **** fin ajout ****
#endif
#endif

			}

		}
		dir->elts_count = num_rec;
		fclose(fic_rep);
		return(OK);
	}


} /* fin charger */