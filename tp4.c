#include "tp4.h"
/// mise en place du menu
/*void color(int t,int f)
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
}*/

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

int menu( char** tab)//, COORD O)
{
    //system("cls");
    //O.Y=+2;
    int choix=1, k=0;
    do
    {
        /*int y=O.Y;
        color(15,0);
        gotoxy(O.X,O.Y-2);*/
        printf("\n %s\n",tab[0]);
        for (int i=1; i<8; i++)
        {
            if (i==choix)
            {
                     /*gotoxy(O.X,y++);
                     color(0,15);*/
                     printf("%d. %s",i,tab[i]);
                     for(int j=strlen(tab[i]); j<strlen(tab[6]); j++) printf(" ");
                     printf("\n");
            }
            else
            {
                /*gotoxy(O.X,y++);
                color(15,0);*/
                printf("%d. %s",i,tab[i]);
                for(int j=strlen(tab[i]); j<strlen(tab[6]); j++) printf(" ");
                printf("\n");
            }
        }
        /*k=*/scanf("%d",&k);//getch();
        if (k<=7&&k>=1)//(k<=55&&k>=49)
        {
            choix=k;//%49+1;
            k=13;
        }
        else {choix=1; k=13;}
       /* else if(k!=13)
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
        }*/
    }while(k!=13);
    return(choix);
}
/// mise en place des fonctions de base
T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase,int* maj)
{
    T_Position* x, *inserable;
    x=listeP;
    inserable=malloc(sizeof(T_Position));
    inserable->numeroLigne=ligne;
    inserable->numeroPhrase=phrase;
    inserable->ordre=ordre;
    inserable->suivant=NULL;
    if(!x)
    {
        x=inserable;
    }
    else
    {
        if(x->numeroLigne>=ligne && x->ordre>ordre)
        {
            inserable->suivant=x;
            x=inserable;
            *maj=1;
        }
        else
        {
            while(x->suivant!=NULL)
            {
                if(x->suivant->numeroLigne>=ligne && x->suivant->ordre>ordre)
                {
                    break;
                }
                x=x->suivant;
            }
            if(x->numeroLigne!=ligne && x->ordre!=ordre)
            {
                    inserable->suivant=x->suivant;
                    x->suivant=inserable;
                    *maj=1;
            }
            x=listeP;
        }
    }
    return(x);
}

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase) ///ne pas oublier de considerer le point
{
    int *maj=malloc(sizeof(int));
    *maj=0;
    if (index == NULL || mot == NULL) return 0;

    // Création de motTraite un copie du mot en cours tt en minuscule pour pouvoire faciliter le traitement
    char motTraite[100];
    int i = 0;
    while(mot[i] != '\0')
    {
        if(mot[i] >= 'A' && mot[i] <= 'Z')
        {
            motTraite[i] = mot[i] + 32; // passage maj vers min
        }
        else
        {
            motTraite[i] = mot[i];
        }
        i++;
    }
    motTraite[i] = '\0'; // fin de chaine


    // Si arbre vide je met mon noeud à la racine
    if (index->racine == NULL)
    {
        // implementation de la struct du noeud
        T_Noeud *nouveau = malloc(sizeof(T_Noeud));
        if (nouveau == NULL) return 0;

        nouveau->mot = malloc(strlen(motTraite) + 1);
        strcpy(nouveau->mot, motTraite);

        nouveau->nbOccurences = 1;
        nouveau-> filsGauche = NULL;
        nouveau-> filsDroit = NULL ;
        nouveau->listePositions = NULL ;
        // Appele de la fonction de Samuel pour ajt la position
        nouveau->listePositions = ajouterPosition(nouveau->listePositions, ligne, ordre, phrase,maj);

        // J e met le noeud à la racine de l'index
        index->racine = nouveau;
        index->nbMotsDistincts = 1;
        index->nbMotsTotal = 1;
        return 1;
    }


    // Cas où l'arbre existe déja, recherche de où on met le noeud

    T_Noeud *courant = index->racine;
    while (1)
    {
        int comparaison ;
        comparaison = strcmp(motTraite, courant->mot);
        // Si mot identique
        if (comparaison == 0)
        {
            courant->listePositions = ajouterPosition(courant->listePositions, ligne, ordre, phrase,maj);
            if (*maj==1)
                courant->nbOccurences++;
            index->nbMotsTotal++; // On a un mot de plus au total
            return 1;
        }
        // Cas 1 si mot plus petit alors je vais dans la partie gauche de l'arbre
        else if (comparaison < 0)
        {
        // si sous arbre gauche vide on met le mot ici
            if (courant->filsGauche == NULL)
            {
                T_Noeud *nouveau = malloc(sizeof(T_Noeud));
                nouveau->mot = malloc(strlen(motTraite) + 1);
                strcpy(nouveau->mot, motTraite);
                nouveau->nbOccurences = 1;
                nouveau->filsGauche = NULL;
                nouveau->filsDroit = NULL;
                nouveau->listePositions = NULL;
                nouveau->listePositions = ajouterPosition(nouveau->listePositions, ligne, ordre, phrase,maj);
                courant->filsGauche = nouveau; // On attache le nouveau noeud
                index->nbMotsDistincts++;
                index->nbMotsTotal++;
                return 1;
            }
        // Sinon on continue de descendre à gauche jusqua nuull
        courant = courant->filsGauche;
        }

        // comparaison > 0 le mot est plus grand lexicographiquement donc je vais à droite
        // Similaire à precedent
        else
        {
            if (courant->filsDroit == NULL)
            {
                T_Noeud *nouveau = malloc(sizeof(T_Noeud));
                nouveau->mot = malloc(strlen(motTraite) + 1);
                strcpy(nouveau->mot, motTraite);
                nouveau->nbOccurences = 1;
                nouveau->filsGauche = NULL;
                nouveau->filsDroit = NULL;
                nouveau->listePositions = NULL;
                nouveau->listePositions = ajouterPosition(nouveau->listePositions, ligne, ordre, phrase,maj);
                courant->filsDroit = nouveau; // On attache le nouveau noeud
                index->nbMotsDistincts++;
                index->nbMotsTotal++;
                return 1;
            }
            courant = courant->filsDroit;
        }
    }
    free (maj);
}

T_Noeud* rechercherMot(T_Index* index, char *mot)
{
    T_Noeud* X;
    X=NULL;
    if(index)
    {
        X=index->racine;
        int trouve=0;
        while(!trouve&&X)
        {
            int pos;
            pos=strcmp(X->mot,mot);
            if(pos==0)
                trouve=1;
            if(pos<0)
                X=X->filsDroit;
            if(pos>0)
                X=X->filsGauche;
        }
    }
    return(X);
}

File* creer_file(int l)
{
    File* F=malloc(sizeof(File));
    F->Longueur=l+1;
    F->head=0;
    F->tail=0;
    F->tab=malloc(sizeof(T_Noeud)*l+1);
    return(F);
}
int est_vide(File* F)
{
    if(F)
        return(F->head==F->tail);
    return(1);
}
int est_pleine(File* F)
{
    if(F)
        return(F->head==(F->tail+1)%F->Longueur);
    return (1);
}
void enfiler(File* F, const T_Noeud* n)
{
    if(!est_pleine(F))
    {
        F->tab[F->tail]=*n;
        if (F->tail==F->Longueur-1)
            F->tail=0;
        else
            F->tail++;
    }
}
const T_Noeud* defiler(File* F)
{
    if(!est_vide(F))
    {
        const T_Noeud* a=&(F->tab[F->head]);
        if (F->head==F->Longueur-1)
            F->head=0;
        else
            F->head++;
        return(a);
    }
    return(NULL);
}

void afficher_noeud(const T_Noeud* X)
{
    if(X)
    {
        char mot[100];
        strcpy(mot,X->mot);
        mot[0]=mot[0]+'A'-'a';
        printf("|-- %s(%d)\n", mot, X->nbOccurences);
        T_Position* y;
        y=X->listePositions;
        while(y!=NULL)
        {
            printf("|---- (l:%d,o:%d,p:%d)\n",y->numeroLigne,y->ordre,y->numeroPhrase);
            y=y->suivant;
        }
    }
    else
        printf("adresse nulle");
}
void parcours_infixe(T_Noeud* X, File* F)
{
    if(X!=NULL)
    {
        parcours_infixe(X->filsGauche,F);
        enfiler(F,X);
        parcours_infixe(X->filsDroit,F);
    }
}
void afficherIndex(T_Index* index)
{
    if(index)
    {
        File* F=creer_file(index->nbMotsDistincts+1);
        parcours_infixe(index->racine,F);
        char title='a';
        const T_Noeud* X=defiler(F);
        while(X)
        {
            if(X && X->mot[0]==title)
                printf("\n\n%c\n",title+'A'-'a');
            while(X && X->mot[0]==title)
            {
                afficher_noeud(X);
                X=defiler(F);
                if(X && X->mot[0]==title)
                    printf("|\n");
            }
            title++;
        }
    }
}

//chien[(1,7,4)] chat[(2,1,4)] droit[(1,1,4);(2,2,3);(3,3,1)] bateau[(1,5,7)] cheval(3,3,3) avion(2,2,3)
//besoin(1,1,2) chef(8,2,3) chevre(3,1,3) chatte(1,1,3) dortoir(1,9,3) sorcier(1,6,3)

void test()
{
    T_Index* index;
    index=malloc(sizeof(T_Index));
    index->racine=NULL;
    int i=1;
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"chien",1,4,7));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"chat",2,4,1));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"droit",1,4,1));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"droit",2,1,2));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"Droit",3,1,3));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"bateau",1,7,5));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"cheval",3,3,3));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"avion",2,3,2));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"besoin",1,2,1));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"chef",8,3,2));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"Chevre",3,3,1));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"chatte",1,3,1));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"dortoir",1,3,9));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"sorcier",1,3,6));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"Quis",2,16,8));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"quisque",1,0,3));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"Risus",1,2,3));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"sit",0,3,0));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"sit",2,8,7));
    printf("resultat de l'ajour %d: %d\n",i++,ajouterOccurence(index,"SiT",3,6,10));
    afficherIndex(index);
}
