#pragma once

// #define VERSION 21.0

#define MAX_LASTNAME 30     /* longueur maximale d'un nom */
#define MAX_FIRSTNAME 25
#define MAX_TEL 20      /* longueur maximale d'un numéro de tél. */

/* Record : structure d'un contact dans le tableau de contacts */
/* le type enregistrement contient 3 champs :                          */
/*  - nom, prenom et tel qui sont tous les trois des chaînes de        */
/*  caractères (tableaux de char)                                      */
typedef struct enreg
{
	unsigned char nom[MAX_LASTNAME];
	unsigned char prenom[MAX_FIRSTNAME];
	unsigned char tel[MAX_TEL];
} Record;

