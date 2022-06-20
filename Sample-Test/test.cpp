#include "gtest/gtest.h"

extern "C" {
#include "../ContactsDirectory/personne.h"
}
extern "C" {
#include "../ContactsDirectory/liste.h"
}
extern "C" {
#include "../ContactsDirectory/rep.h"
}

TEST(TestCaseName, TestName) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(ajouter_un_contact_dans_rep, TestAjouterUnContactDansRep) {
	Directory dir;
	dir.elts_count = 0;
	dir.list;
}

TEST(est_sup, TestEstSup) {

	//On test BCD et ABC la fonction retourne true

	Record enr1; //on crée une strucutre record pour appeler notre fonction 
	unsigned char nom[MAX_LASTNAME] = "BCD"; //on crée un string identique au .nom de la stucture record

	Record enr2;
	unsigned char nom2[MAX_LASTNAME] = "ABC";

	for (int i = 0; i < MAX_LASTNAME; i++) {//assigne les valeurs des unsigned char avec celle des structure record
		enr1.nom[i] = nom[i];
		enr2.nom[i] = nom2[i];
	}
	EXPECT_TRUE(est_sup(enr1, enr2));

	//On test bcd et abc la fonction retourne true

	Record enr3;
	unsigned char nom3[MAX_LASTNAME] = "bcd";

	Record enr4;
	unsigned char nom4[MAX_LASTNAME] = "abc";

	for (int i = 0; i < MAX_LASTNAME; i++) {
		enr3.nom[i] = nom3[i];
		enr4.nom[i] = nom4[i];
	}
	EXPECT_TRUE(est_sup(enr3, enr4));

	//On test bcd et ABC la fonction retourne true

	Record enr5;
	unsigned char nom5[MAX_LASTNAME] = "bcd";

	Record enr6;
	unsigned char nom6[MAX_LASTNAME] = "abc";

	for (int i = 0; i < MAX_LASTNAME; i++) {
		enr5.nom[i] = nom5[i];
		enr6.nom[i] = nom6[i];
	}
	EXPECT_TRUE(est_sup(enr5, enr6));

}

TEST(compact, compact1) {

	char chaine1[] = "A0123456789B";
	compact(ch         VH+¨°
	aine1);
	ASSERT_STREQ(chaine1, "0123456789");

	char chaine2[] = "1&2~3#4'5{6}7(8)[9]";
	compact(chaine2);
	ASSERT_STREQ(chaine2, "123456789");

	char chaine3[] = "1-2|3`4_5@67+8=9";
	compact(chaine3);
	ASSERT_STREQ(chaine3, "123456789");

	char chaine4[] = "1<2>3^4$5%6*7?8,9";
	compact(chaine4);
	ASSERT_STREQ(chaine4, "123456789");

	char chaine5[] = "1;2/3:4!5<6>789";
	compact(chaine5);
	ASSERT_STREQ(chaine5, "123456789");
}

