#include "tp4.h"

/// mise en place du menu
void color(int t,int f)
{
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(H,f*16+t);
}

void gotoxy(int x, int y)
{
   HANDLE H;
   COORD O;
   O.X = x;
   O.Y = y;
   H= GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleCursorPosition(H,O);
}

void initialisation(char** LM)
{
    for(int i=0; i<9; i++)
        LM[i]=malloc(sizeof **LM *50);
    strcpy(LM[0],"             ----menu----                ");
    strcpy(LM[1],"Charger un fichier");
    strcpy(LM[2],"Caracteristiques index");
    strcpy(LM[3],"Afficher index");
    strcpy(LM[4],"Rechercher un mot");
    strcpy(LM[5],"Afficher les occurences d'un mot");
    strcpy(LM[6],"Construire le texte a partir de l'index");
    strcpy(LM[7],"Quitter");
    strcpy(LM[8],"Cette partie sera disponible bientot");
}

int menu( char** tab, COORD O)
{
    system("cls");
    O.Y=+2;
    int choix=1, k=0;
    do
    {
        int y=O.Y;
        color(15,0);
        gotoxy(O.X,O.Y-2);
        printf("\n %s\n",tab[0]);
        for (int i=1; i<8; i++)
        {
            if (i==choix)
            {
                     gotoxy(O.X,y++);
                     color(0,15);
                     printf("%d. %s",i,tab[i]);
                     for(int j=strlen(tab[i]); j<strlen(tab[6]); j++) printf(" ");
                     printf("\n");
            }
            else
            {
                gotoxy(O.X,y++);
                color(15,0);
                printf("%d. %s",i,tab[i]);
                for(int j=strlen(tab[i]); j<strlen(tab[6]); j++) printf(" ");
                printf("\n");
            }
        }
        k=getch();
        if (k<=55&&k>=49)
        {
            choix=k%49+1;
            k=13;
        }
        else if(k!=13)
        {
            k=getch();
            if (k+256==KBDOWN)
            {
                choix++;
                if(choix==8) choix=1;
            }
            if (k+256==KBUP)
            {
                choix--;
                if(choix==0) choix=7;
            }
        }
    }while(k!=13);
    return(choix);
}
