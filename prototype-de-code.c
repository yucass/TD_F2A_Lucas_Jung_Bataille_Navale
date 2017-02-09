#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>
#include <windows.h>

#define TAILLE 10
#define SHIPNUMBER 5
#define PLAYERNUMBER 2

//*******************************STRUCTURES
//Struct obligatoire
typedef struct t_cell
{
    int ligne, colonne;
}t_cell;

//Struct enum inutile
typedef enum e_name
{
    torpedo, submarine, counter_torpedo, cruiser, aircraft_carrier
}e_name;

//structure pour le sens de positionnement du navire
typedef enum e_direction
{
    up, down, left, right
}e_direction;

//Structure navire
typedef struct t_ship
{
    e_name shipname;
    t_cell  coordonnee;
    int life;
    e_direction way;
    int lignemax;
    int colonnemax;
    int position[20];

}t_ship;

//Structure flotte
typedef struct t_fleet
{
    t_ship ship[5];
    int tabposition[100][1];
    int activeplayer;
    int tabhit[100];
    int tabmiss[100];
    int totalshiplife;

}t_fleet;

//********************************* PROTOTYPES
void menu ();
void initfleet (t_fleet fleet[]);
void checkshippositionmax (int a, int b,  t_fleet fleet[1], int erreur);
void checkshipposition (int a, int b,  t_fleet fleet[1], int erreur);
int checkinitShip (int a, int b, int x, int z, t_fleet fleet[], int shipnumber, int *Err, int error);
void targeting (t_fleet fleet[], int shipnumber, int a, int c, int playernumber, int eachship, int eachpos, int changeplayer);
void checkvictory (int shipnumber, t_fleet fleet[], int playernumber, int a, int b);
void display (t_fleet fleet[], int changeplayer);



int main(int argc, char *argv[])
{
     int eachship;
      int eachpos;
     int shipnumber;
     int a, b, c;
    t_fleet fleet[PLAYERNUMBER] ={};
    int fleetalive = 0;
   int playernumber = 2;

    int changeplayer = 0;
    int quit = 0;

    //while (quit == 0)
    //{
       // menu();
        initfleet(fleet);
        while (fleetalive != 1)
        {for (changeplayer = 0; changeplayer < playernumber;)
        {

            targeting ( fleet, shipnumber, a, c, playernumber, eachship, eachpos, changeplayer);
            display ( fleet, changeplayer);
            changeplayer = changeplayer+1; // on cible le joueur suivant
                if (changeplayer  == PLAYERNUMBER) // si le joueur suivant est hors du nombre de joueur, on revient au joueur 0
                    {changeplayer = 0;}
        }
     checkvictory ( shipnumber, fleet, playernumber,a, b);
   // }
    }
    return 0;
}




void initfleet(t_fleet fleet[])
{
    int tabposition[100][1];
    char tabname[100][50] = {"Porte-avions", "Croiseur","contre torpilleur", "sous-marin", "torpilleur"};
   // t_fleet *fl = &fleet[1];
    int playernumber =2;
    int shipnumber = 1;
    int a,b,x,y, z = 0;
    int error =0;
    int eachpos;
    int *Err = &error;
    for (a=0;a<playernumber;a++)
    {
        for (b=0; b<shipnumber;b++)
            { // ************************** demande les coordonné pour le placement
                fleet[a].ship[b].coordonnee.colonne = 11;
                while (fleet[a].ship[b].coordonnee.colonne > 9)
                {printf("saisissez la colonne pour %s : 0,1,2,3,4,5,6,7,8,9", tabname[b]);
                    scanf("%d", &fleet[a].ship[b].coordonnee.colonne);
                }fleet[a].ship[b].coordonnee.colonne *=10; // *******************************transforme les coordonnées des colonne en dizaine

                checkshipposition ( a, b, fleet, error);

            // ************************** demande les coordonné pour le placement
                fleet[a].ship[b].coordonnee.ligne = 11;
                while (fleet[a].ship[b].coordonnee.ligne > 9)
            {printf("saisissez la ligne pour %s : 0,1,2,3,4,5,6,7,8,9", tabname[b]);
                scanf("%d", &fleet[a].ship[b].coordonnee.ligne);

                checkshipposition ( a, b, fleet, error);

            }
            // ************************** demande le sens pour le placement
            fleet[a].ship[b].way = 5;
            while (fleet[a].ship[b].way > 4)
            {printf("saisissez le sens pour %s   : 0-Haut, 1-Bas, 2-Gauche, 3-Droite", tabname[b]);
                scanf("%d", &fleet[a].ship[b].way);
            }


        // ************************** Switch qui donne les point de vie (donc la taille) du navire
        switch (b)
        {
            case 0 : fleet[a].ship[b].life=4;break;
            case 1 : fleet[a].ship[b].life=3;break;
            case 2 : fleet[a].ship[b].life=2;break;
            case 3 : fleet[a].ship[b].life=2;break;
            case 4 : fleet[a].ship[b].life=1;break;
            default : printf("erreur de calcul PV");break;
        }
        // ************************** switch qui en fonction du sens calcul les cases occupées
        switch (fleet[a].ship[b].way)
        {
            case 0 : fleet[a].ship[b].lignemax = fleet[a].ship[b].coordonnee.ligne + fleet[a].ship[b].life; // calcule des coordonnées occupées  : coordonné + taille du navire
                     fleet[a].ship[b].colonnemax = fleet[a].ship[b].coordonnee.colonne;
checkshippositionmax ( a, b,  fleet, error);
                     for (x=fleet[a].ship[b].coordonnee.ligne; x <= fleet[a].ship[b].lignemax; x++)
                        {   y = fleet[a].ship[b].colonnemax;
                            fleet[a].ship[b].position[z] = x+y; //insert dans position[] les coordonnées occupées par le navire

                            // ************************** demande les coordonné pour le placement
                            checkinitShip ( a,  b,  x, z, fleet,  shipnumber, *Err, error);

                            printf("\n%d\n",fleet[a].ship[b].position[z]); // printf de check
                            z++;
                        }break;


            case 1 : fleet[a].ship[b].lignemax = fleet[a].ship[b].coordonnee.ligne - fleet[a].ship[b].life;
                     fleet[a].ship[b].colonnemax = fleet[a].ship[b].coordonnee.colonne;
checkshippositionmax ( a, b,  fleet, error);
                      for (x=fleet[a].ship[b].lignemax; x <= fleet[a].ship[b].coordonnee.ligne; x++)
                        {   y = fleet[a].ship[b].colonnemax;
                            fleet[a].ship[b].position[z] = x+y;


                            checkinitShip ( a,  b,  x, z, fleet,  shipnumber, *Err, error);

                            printf("\n%d\n",fleet[a].ship[b].position[z]);
                            z++;
                        }break;

            case 2 : fleet[a].ship[b].lignemax = fleet[a].ship[b].coordonnee.ligne;
                     fleet[a].ship[b].colonnemax = fleet[a].ship[b].coordonnee.colonne - fleet[a].ship[b].life;
checkshippositionmax ( a, b,  fleet, error);
                     for (x=fleet[a].ship[b].colonnemax; x <= fleet[a].ship[b].coordonnee.colonne; x++)
                        {   y = fleet[a].ship[b].lignemax;
                            fleet[a].ship[b].position[z] = x+y;


                            checkinitShip ( a,  b,  x, z, fleet,  shipnumber, *Err, error);
                            printf("\n%d\n",fleet[a].ship[b].position[z]);
                                z++;
                        }break;


            case 3 : fleet[a].ship[b].lignemax = fleet[a].ship[b].coordonnee.ligne;
                     fleet[a].ship[b].colonnemax = fleet[a].ship[b].coordonnee.colonne + fleet[a].ship[b].life;
checkinitShip ( a,  b,  x, z, fleet,  shipnumber, *Err, error);
                     for (x=fleet[a].ship[b].coordonnee.colonne ; x <= fleet[a].ship[b].colonnemax; x++)
                        {   y = fleet[a].ship[b].lignemax;
                            fleet[a].ship[b].position[z] = x+y;
                            checkshippositionmax ( a, b,  fleet, error);


                            printf("\n%d\n",fleet[a].ship[b].position[z]);
                            z++;
                        }break;

            default : printf("erreur de calcul PV");break;

        }
//printf("\nerrur fin de code = %d\n", error);
/*if (error == 1)
        {printf("\nerrur debut de if de fin = %d\n", error);
            b--;printf("\nerrur fin de if = %d\n", error);
        }*/
        }
    }


}
// ************************** fonction qui verifie qu'aucun bateau n'occupe deja la case, cette fonction est lié au switch de placement dans initfleet()
// ************************** NE FONCTIONNE PAS, DU MOINS ALEATOIREMENT
int checkinitShip (int a, int b, int x, int z, t_fleet fleet[], int shipnumber, int *Err, int error)
{
    int eachship;
    int eachpos;

    for (eachship=0; eachship<b;eachship++)
    {for (eachpos=0; eachpos<5;eachpos++)
    {
        if (fleet[a].ship[b].position[z] == fleet[a].ship[eachship].position[eachpos])
            {printf("AHHHHHHHHHHHHHHHHHHHHHH");error = 1;
             }else error = 0;

    }


}return (error);
}

// ************************** Procédure de tir
void targeting (t_fleet fleet[], int shipnumber, int a, int c, int playernumber, int eachship, int eachpos, int changeplayer)
{
    int targetcolumn;
    int targetline;
    int fireposition;

// ************************** demande les coordonné pour le tir
    targetcolumn = TAILLE + 1;
        while (targetcolumn > TAILLE-1)
                {printf("saisissez la colonne de tir : 0,1,2,3,4,5,6,7,8,9");
                    scanf("%d", &targetcolumn);
                }targetcolumn *=10; // transforme les coordonnées de la colonne en dizaine

                // ************************** demande les coordonné pour le tir
    targetline = TAILLE + 1;
        while (targetline > TAILLE-1)
                {printf("saisissez la ligne de tir : 0,1,2,3,4,5,6,7,8,9");
                    scanf("%d", &targetline);
                }
// ************************** additione la ligne et la colonne (unité et dizaine) pour obtenir qu'une seule valeur numerique
    fireposition = targetcolumn + targetline;
veriftargeting (shipnumber, fireposition,fleet, a,c, playernumber, eachship, eachpos, changeplayer);


}
// ************************** fonction de verification
void veriftargeting (int shipnumber, int fireposition, t_fleet fleet[], int a, int c, int playernumber, int eachship, int eachpos, int changeplayer)
{
printf("\n%d\n%d\n", fireposition, changeplayer);

    for (c=0; c<100; c++)
    {
        if (fleet[changeplayer].tabhit[c] == fireposition) // verifie que le tir n'a pas deja touché un bateau en utilisant tabhit (tableau de touché)
        {
            printf("vous avez deja touché un bateau à cette endroit"); //targeting (fleet, shipnumber, a, c);
        }
        if (fleet[changeplayer].tabmiss[c] == fireposition) // verifie que le tir n'as pas deja touché l'eau en utilisant tabmiss (tableau de raté)
        {
            printf("vous avez deja touché un poisson à cette endroit"); //targeting (fleet, shipnumber, a, c);
        }

}
// ************************** verifie que le tir touche bien un bateau ==> mais un bateau du joueur suivant !!
// Ne marche pas non plus lol
changeplayer = changeplayer+1; // on cible le joueur suivant
                if (changeplayer  == PLAYERNUMBER) // si le joueur suivant est hors du nombre de joueur, on revient au joueur 0
                    {changeplayer = 0;}

    for (eachship=0; eachship<SHIPNUMBER;eachship++) // pour chaque navire
        {for (eachpos=0; eachpos<5;eachpos++) // pour chaque position dans le tableau des coordonnéé de placement du navire
            {
           // printf("\ntest position : %d\n", fleet[changeplayer].ship[eachship].position[eachpos]);
            //printf("\nflotte cible : %d\n", fleet[changeplayer]);
            //printf("\njoueur cible : %d\n", changeplayer);

                if (fireposition == fleet[changeplayer].ship[eachship].position[eachpos])
                {printf("TOUCHE !!!!"); fleet[changeplayer].ship[eachship].life-1; fleet[changeplayer].tabhit[fireposition] = fireposition; // on deduit 1PV au navire, on stock la coordonné de tir en memoire
                }else fleet[changeplayer].tabmiss[fireposition] = fireposition;
    }
}changeplayer = changeplayer+1; // on cible le joueur suivant
                if (changeplayer  == PLAYERNUMBER) // si le joueur suivant est hors du nombre de joueur, on revient au joueur 0
                    {changeplayer = 0;}
}

// ************************** On verifi qu'on tir bien dans le cadre de la MAP (bien que cela soit deja lock par des mini-maxi
void checkfirerange(int targetcolumn, int targetline, int shipnumber, int fireposition, t_fleet fleet[], int a, int c, int playernumber, int eachship, int eachpos, int changeplayer)
{
    if (targetcolumn >0 && targetcolumn < TAILLE && targetline >0 && targetline < TAILLE)
    {
        printf("PAS D'ERREUR DE TIR\n");
    }else printf("ERREUR DE TRAJECTOIRE, VEUILLEZ RE CALCULER VOTRE TIR"); veriftargeting (shipnumber, fireposition, fleet, a, c, playernumber, eachship, eachpos, changeplayer);
}



// ************************** Verouille l'utilisateur pour ne pas placer les navire hors du jeu
void checkshippositionmax (int a, int b,  t_fleet fleet[], int erreur)
{
    if (fleet[a].ship[b].lignemax < 0)
    {
        printf ("vous etes hors champs");
    }
    if (fleet[a].ship[b].lignemax >TAILLE)
    {
        printf ("vous etes hors champs");
    }
    if (fleet[a].ship[b].colonnemax/10 < 0)
    {
        printf ("vous etes hors champs");
    }
    if (fleet[a].ship[b].colonnemax/10 > TAILLE)
    {
        printf ("vous etes hors champs");
    }
    erreur = 0;
}


// ************************** Verouille l'utilisateur pour ne pas placer les navire hors du jeu
void checkshipposition (int a, int b,  t_fleet fleet[1], int erreur)
{
    if (fleet[a].ship[b].coordonnee.ligne < 0)
    {
        printf ("vous etes hors champs");
    }
    if (fleet[a].ship[b].coordonnee.ligne >TAILLE)
    {
        printf ("vous etes hors champs");
    }
    if (fleet[a].ship[b].coordonnee.colonne/10 < 0)
    {
        printf ("vous etes hors champs");
    }
    if (fleet[a].ship[b].coordonnee.colonne/10 > TAILLE)
    {
        printf ("vous etes hors champs");
    }
    erreur = 1;
}


// ************************** Fonction qui determine si un jooueur gagne ou non
void checkvictory (int shipnumber, t_fleet fleet[], int playernumber, int a, int b)
{
    a=0;
    b=0;
    int fleetalive =0;
// ************************** On cumule les PV de chaque navire
    for (a =0; a<PLAYERNUMBER; a++)
    {for (b=0; b<SHIPNUMBER; b++)
    {    fleet[a].totalshiplife = 0;
        fleet[a].totalshiplife = fleet[a].totalshiplife + fleet[a].ship[b].life;
    }
    }// ************************** on verifie si il y a plusieurs joueurs avec plus que 0 PV
    for (a =0; a<PLAYERNUMBER; a++)
    {
        if (fleet[a].totalshiplife > 0)
        {
            fleetalive ++;
        }
    // ************************** Si exactement 1 joueurs a plus que 0 PV (au cumule) il est gagnant
    if (fleetalive == 1)
    {
        for (a =0; a<PLAYERNUMBER; a++)
    {
        if (fleet[a].totalshiplife != 0)
        {
            printf("la Flotte %d a gagnee\n", fleet[a]);
        }
    }
    }
}
}

void display (t_fleet fleet[], int changeplayer)
{
    char displaytab[TAILLE*TAILLE];

    int a, h =0;
    int f, c =1;
    printf("\n\n");

    for (c=0;c<TAILLE*TAILLE; c++)
    {
        displaytab[c] = '0';
    }

    for (a=0; a<TAILLE*TAILLE-1; a++)
    {if (fleet[changeplayer].tabhit[a] != 0)
        {displaytab[a] = 'X';}
     if (fleet[changeplayer].tabmiss[a] != 0)
        {displaytab[a] = 'W';}

    }

    for (h=0; h<TAILLE*TAILLE; h++)
    {
        if (h == f*TAILLE)
        {
           printf("\n");f++;
        } printf("%c ",displaytab[h]);
    }printf("\n\n"); f=1;
}











//**************************************************************** SYSTEME DE COULEUR DE TEXTE
void Color(int couleurDuTexte,int couleurDeFond) // fonction d'affichage de couleurs
{
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

