#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>
#include <windows.h>

#define TAILLE 10
#define SHIPNUMBER 2
#define PLAYERNUMBER 2
#define TAILLENAVIRE 2

enum BOOL
{
    False=0, True
};

typedef struct t_coordonnees
{
    int ligne;
    int colonne;
}t_coordonnees;

typedef struct t_ship
{
    t_coordonnees position;
    t_coordonnees maxposition;
}t_ship;

typedef struct t_fleet
{
        t_ship ship[SHIPNUMBER];
            int tabposition[TAILLE*TAILLE];
            int life;
}t_fleet;



//********************************************** PROTOTYPES
void initfleet(t_fleet fleet[], int eachship, int eachpos);
void targeting (t_fleet fleet[], int a, int c, int eachship, int eachpos, int currentplayer);
    /*==>*/ void veriftargeting (int fireposition, t_fleet fleet[], int a, int c, int eachship, int eachpos, int currentplayer);
    /*==>*/ int changeplayer (int currentplayer, t_fleet fleet[]);
int checkvictory (t_fleet fleet[], int a, int b, int victory);
void display (t_fleet fleet[], int currentplayer);
    /*==>*/  void tabdisplay (int positionintab, int line, char displaytab[]);

//----------------------------------------------------------------------------------------------------------------------------

//******************************************************************************************************************
//**************************************************** MAIN ********************************************************
//******************************************************************************************************************
int main(int argc, char *argv[])
{
    //VARIABLES
    t_fleet fleet[PLAYERNUMBER];
    int eachship;
    int eachpos;
    int a, b, c;
    int victory =999;
    int currentplayer = 0;
    char tabpos[50];


    printf("*****************************************************************\n");
    printf("*****************************************************************\n");
    for(a=0;a<50;a++)
    {tabpos[a] = ' ';
        }
        printf("___/\\_/_/___                             ___/\\_/_/___\n");
       printf("\\_____|_____/                            \\_____|_____/\n");
       printf("          ");
    for(a=0;a<38;a++)
    {
        tabpos[a-1] = '.';
        tabpos[a] = '*';

        printf("%c", tabpos[a]);

        Sleep(200);
    }
system("cls");
printf("                             ____\n");
printf("                     __,-~~/~    `---.\n");
printf("                   _/_,---(      ,    )\n");
printf("               __ /        <    /   )  \\___\n");
printf("- ------===;;;'====------------------===;;;===----- -  -\n");
printf("                  \\/  ~\"~\"~\"~\"~\"~\~\"~)~\"/\n");
printf("                  (_ (   \\  (     >    \\)\n");
printf("                   \_( _ <         >_>'\n");
printf("                      ~ `-i' ::>|--\"\n");
printf("                          I;|.|.|\n");
printf("                         <|i::|i|`.\n");
printf("                        (` ^'\"`\-' \"\)\n");

        Sleep(500);
        printf("\n\n");
        for(a=0;a<38;a++)
    {

        tabpos[a] = '*';

        printf("%c", tabpos[a]);

        Sleep(200);
    }
    printf("\n                        SUPER BATAILLE NAVALE DE LA MORT !!\n\n");

    for (a=0; a <PLAYERNUMBER; a++)
    {
        for (b=0;b<=TAILLE*TAILLE;b++)
        {
            fleet[a].tabposition[b] = 999; // on initialise le tableau tabposition a 999
        }fleet[a].life = SHIPNUMBER * (TAILLENAVIRE+1); // on calcul les PV des joueurs
    }


    initfleet(fleet, eachship, eachpos); // Fonction de creation de la flotte
    while (victory == 999) // valeur de rupture de boucle
        {
            display (fleet, currentplayer);
            targeting ( fleet, a, c, eachship, eachpos, currentplayer);
            //checkvictory (fleet, a, b, victory);
            victory=checkvictory (fleet, a, b, victory);

            currentplayer = changeplayer ( currentplayer,fleet);
        }
        return 0;
}


void initfleet(t_fleet fleet[],int eachship, int eachpos)
{
    int a,b, x, y, z = 0;
    int error = 0;
    char tabname[100][100] = {"Porte-avions", "Croiseur","contre torpilleur", "sous-marin", "torpilleur"};

    for (a=0;a<PLAYERNUMBER;a++)
    {printf("Joueur : %d Veuillez selectionner les coordonnees pour vos navires\n\n", a);
        for (b=0; b<SHIPNUMBER;b++)
            {
                // ************************** demande les coordonné pour le placement
                fleet[a].ship[b].position.ligne = TAILLE;
                while (fleet[a].ship[b].position.ligne > TAILLE-1)
                        {printf("saisissez la ligne pour %s : 0,1,2,3,4,5,6,7,8,9", tabname[b]);
                            scanf("%d", &fleet[a].ship[b].position.ligne);

                        }fleet[a].ship[b].position.ligne *= 10;


                // ************************** demande les coordonné pour le placement
                fleet[a].ship[b].position.colonne = TAILLE;
                while (fleet[a].ship[b].position.colonne > TAILLE-1)
                        {printf("saisissez la colonne pour %s : 0,1,2,3,4,5,6,7,8,9", tabname[b]);
                            scanf("%d", &fleet[a].ship[b].position.colonne);
                        }


                //******************************Verification du hors champs
                fleet[a].ship[b].maxposition.colonne = fleet[a].ship[b].position.colonne + TAILLENAVIRE;
                if (fleet[a].ship[b].maxposition.colonne >= TAILLE || fleet[a].ship[b].maxposition.colonne < 0)
                {printf("ATTENTION ! On sort de la zone d'entrainement\n"); error =1;}


                //*******************************Verification de la presence d'un autre navire
                for (eachpos=0; eachpos<TAILLE*TAILLE; eachpos++)
                    {
                        if ((fleet[a].ship[b].maxposition.colonne + fleet[a].ship[b].position.ligne) == fleet[a].tabposition[eachpos]
                            || (fleet[a].ship[b].position.colonne + fleet[a].ship[b].position.ligne) == fleet[a].tabposition[eachpos])
                        {printf("ATTENTION IMPACT !! navire deja present\n"); error = 1;}
                    }
                if (error == 0) // si il n'y a pas d'erreur on enregistre les coordonnées dans le tableau
                    {
                    for (x=fleet[a].ship[b].position.colonne; x <= fleet[a].ship[b].maxposition.colonne; x++)
                        {   y = fleet[a].ship[b].position.ligne ;
                            fleet[a].tabposition[x+y] = x+y;
                            printf("\n%d\n",fleet[a].tabposition[x+y]);
                        }
                    }else b--;error =0; // si il y a une erreur, on retourne au bateau en cour, on set erreur a 0
                }

    }

}

//********************** Fonction de Tir
void targeting (t_fleet fleet[], int a, int c, int eachship, int eachpos, int currentplayer)
{
    int targetcolumn;
    int targetline;
    int fireposition;

    printf("Joueur : %d a vous de jouer\n\n", currentplayer);

    // ************************** demande les coordonné pour le tir
    targetline = TAILLE;
    while (targetline > TAILLE-1)
            {printf("saisissez la ligne de tir : 0,1,2,3,4,5,6,7,8,9");
                scanf("%d", &targetline);
            }targetline *= 10; // transforme la ligne en dizaine

    // ************************** demande les coordonné pour le tir
    targetcolumn = TAILLE;
    while (targetcolumn > TAILLE-1)
            {printf("saisissez la colonne de tir : 0,1,2,3,4,5,6,7,8,9");
                scanf("%d", &targetcolumn);
            }


    // ************************** additione la ligne et la colonne (unité et dizaine) pour obtenir qu'une seule valeur numerique
    fireposition = targetcolumn + targetline;
    veriftargeting (fireposition, fleet, a,c, eachship, eachpos, currentplayer);

}


//******************************* Fonction de verification du tir
void veriftargeting (int fireposition, t_fleet fleet[], int a, int c, int eachship, int eachpos, int currentplayer)
{
    currentplayer=changeplayer ( currentplayer,  fleet);

    if (fireposition == fleet[currentplayer].tabposition[fireposition])
        {printf("TOUCHE !!!!\n\n"); fleet[currentplayer].life = fleet[currentplayer].life-1; fleet[currentplayer].tabposition[fireposition] = TAILLE*TAILLE +1;
        }else {printf("VOUS AVEZ TOUCHE UN POISSON !!\n\n");fleet[currentplayer].tabposition[fireposition] = TAILLE*TAILLE+2;}

}

// ************************** Fonction qui determine si un joueur gagne ou non
int checkvictory (t_fleet fleet[], int a, int b, int victory)
{
    a=0;
    b=0;
    int fleetalive =0;

    for (a=0; a<PLAYERNUMBER;a++)
        {
            if (fleet[a].life <=0)
                {printf("Le joueur %d a perdu !!\n", a); victory = 999;}
        }

    // ************************** on verifie si il y a plusieurs joueurs avec plus que 0 PV
    for (a =0; a<PLAYERNUMBER; a++)
        {
            if (fleet[a].life > 0)
            {fleetalive ++;}
        }
    // ************************** Si exactement 1 joueurs a plus que 0 PV (au cumule) il est gagnant
    if (fleetalive == 1)
        {
            for (a =0; a<PLAYERNUMBER; a++)
                {
                    if (fleet[a].life != 0)
                        {
                            printf("la Flotte %d a gagnee\n", a); victory = 1;
                        }
                }
        }
    return victory;
}

//******************* Fonction d'affichage
void display (t_fleet fleet[], int currentplayer)
{
    char displaytab[TAILLE*TAILLE] = {NULL};
    int a, z, positionintab =0;
    int line=0, c =1;

    printf("Votre flotte Mon Capitaine !");
    for (a=0; a<TAILLE*TAILLE; a++)
        {
            if (fleet[currentplayer].tabposition[a] > 990)
            {displaytab[a] = '.';}
            if (fleet[currentplayer].tabposition[a] == TAILLE*TAILLE+1)
            {displaytab[a] = 'H';}
            if (fleet[currentplayer].tabposition[a] == TAILLE*TAILLE+2)
            {displaytab[a] = 'W';}
            if (fleet[currentplayer].tabposition[a] <= TAILLE*TAILLE)
                {displaytab[a] = 'B';}
        }
    tabdisplay (positionintab, line, displaytab);
    currentplayer=changeplayer (currentplayer, fleet);
    displaytab[TAILLE*TAILLE] = NULL;
    printf("Voici nos echos radar Mon Capitaine :");
    for (a=0; a<TAILLE*TAILLE; a++)
        {
            if (fleet[currentplayer].tabposition[a] > 990)
            {displaytab[a] = '.';}
            if (fleet[currentplayer].tabposition[a] == TAILLE*TAILLE+1)
            {displaytab[a] = 'H';}
            if (fleet[currentplayer].tabposition[a] == TAILLE*TAILLE+2)
            {displaytab[a] = 'W';}
             if (fleet[currentplayer].tabposition[a] <= TAILLE*TAILLE)
                {displaytab[a] = '.';}
        }
        tabdisplay (positionintab, line, displaytab);

}

void tabdisplay (int positionintab, int line, char displaytab[])
{
        //******************************Systeme de retour a la ligne pour l'affichage d'un tableau a 1 dimension (tous les X elements on retourne a la ligne)
    for (positionintab=0; positionintab<TAILLE*TAILLE; positionintab++) // pour chaque element du tableau displaytab
        {
            if (positionintab == line*TAILLE) // si l'element positiontab == a la ligne x la longueur du tableau => ex : line = 2 : position de 0 a 20 avant le retour \n
                {printf("\n");line++;} // on affiche le retour chariot

            printf("%c ",displaytab[positionintab]); // dans tous les cas on affiche l'element du tableau
        }printf("\n\n"); line=1; // on fini par un double retour a la ligne et on initialise a nouveau la ligne a 1 (juste au cas ou)
}



int changeplayer (int currentplayer, t_fleet fleet[])
{
        currentplayer = currentplayer+1; // on cible le joueur suivant
    if (currentplayer  >= PLAYERNUMBER) // si le joueur suivant est hors du nombre de joueur, on revient au joueur 0
        {currentplayer = 0;}
    while (fleet[currentplayer].life <=0) // verifie que le joueur suivant soit encore en vie
        {currentplayer = currentplayer+1;}

    if (currentplayer  >= PLAYERNUMBER) // si le joueur suivant est hors du nombre de joueur, on revient au joueur 0
        {currentplayer = 0;}

        return currentplayer;
}



/* exemple d'affichage du tableau

* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *

sauf que, une fois sur 2 (lorsque les coordonnées de tir sont superieur a 9), on obtient un truc bizarre comme ca

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *



void creatcell ()
{
    t_coordonnees cell;

    cell.colonne[]
}

*/
