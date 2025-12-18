#include "tp4.h"

int main()
{
    // --- Initialisation des variables ---
    char** LM;
    int choix;
    LM = malloc(sizeof *LM * 9);
    if (LM == NULL) return 1;
    initialisation(LM);

    // Initialisation de l'index desmots du texte
    T_Index index;
    index.racine = NULL;
    index.nbMotsDistincts = 0;
    index.nbMotsTotal = 0;
    index.nbPoints=0;
    index.nomFichier = NULL;
    //Initialisation de la liste des points du texte
    T_Position** Point=malloc(sizeof(T_Position*));

    // Variables temporaires pour les saisies
    char nomFichier[100];
    char motRecherche[100];
    char nomFichierSortie[100];
    int nbMotsLus = 0;
    T_Noeud *resultatRecherche = NULL;

    // --- Gestion du Menu ---

    do
    {
        // Affiche le menu
        CONSOLE_SCREEN_BUFFER_INFO BUF;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BUF);
        choix=menu(LM,BUF.dwCursorPosition);
        system("cls");

        switch (choix)
        {
        case 1: // Charger un fichier
            printf("--- %s ---\n", LM[choix]);
            printf("Entrez le nom du fichier a charger (ex: texte.txt) : ");
            scanf("%s", nomFichier);
            //strcpy(nomFichier,"test_marche.txt");

            index.racine = NULL;
            index.nbMotsDistincts = 0;
            index.nbMotsTotal = 0;
            index.nbPoints=0;

            *Point=NULL;

            nbMotsLus = indexerFichier(&index, nomFichier);

            if (nbMotsLus > 0) {
                printf("Success!! %d mots ont ete lus et indexes.\n", nbMotsLus);
            } else {
                printf("Echec ou fichier vide.\n");
            }
            break;

        case 2: // Caractéristiques de l'index
            printf("--- %s ---\n", LM[choix]);
            if (index.nbMotsTotal == 0) {
                printf("L'index est vide. Veuillez charger un fichier d'abord.\n");
            } else {
                printf("Fichier traite : %s\n", index.nomFichier);
                printf("Nombre total de mots : %d\n", index.nbMotsTotal);
                printf("Nombre de mots distincts : %d\n", index.nbMotsDistincts);
                printf("Le nombre de phrase: %d\n",index.nbPoints);
            }
            break;

        case 3: // Afficher index
            printf("--- %s ---\n", LM[choix]);
            if (index.nbMotsTotal == 0) {
                printf("L'index est vide.\n");
            } else {
                afficherIndex(index);
            }
            break;

        case 4: // Rechercher un mot
            printf("--- %s ---\n", LM[choix]);
            if (index.nbMotsTotal == 0) {
                printf("L'index est vide.\n");
            } else {
                printf("Entrez le mot a rechercher : ");
                scanf("%s", motRecherche);

                resultatRecherche = rechercherMot(&index, motRecherche);
                if (resultatRecherche != NULL) {
                    printf("Mot trouve ! Occurences : %d\n", resultatRecherche->nbOccurences);
                    T_Position *p = resultatRecherche->listePositions;
                    while (p != NULL) {
                        printf("- Ligne :%d | Ordre dans la ligne : %d | Phrase : %d | Ordre dans la phrase :%d\n", p->numeroLigne, p->ordreLigne, p->numeroPhrase, p->ordrePhrase);
                        p = p->suivant;
                    }
                    construireTexteCouleur(index,motRecherche);
                } else {
                    printf("Le mot \"%s\" n'est pas dans l'index.\n", motRecherche);
                }
            }
            break;

        case 5: // Afficher les occurrences (Phrases)
            printf("--- %s ---\n", LM[choix]);
            if (index.nbMotsTotal == 0) {
                printf("L'index est vide.\n");
            } else {
                printf("Entrez le mot dont vous voulez voir les phrases contextuelles : ");
                scanf("%s", motRecherche);
                afficherOccurencesMot(&index,motRecherche);
            }
            break;

        case 6: // Construire le texte
            printf("--- %s ---\n", LM[choix]);

            if (index.nbMotsTotal == 0) {
                printf("L'index est vide. Chargez un fichier d'abord.\n");
            } else {
                printf("Entrez le nom du fichier de sortie (ex: sortie.txt) : ");
                scanf("%s", nomFichierSortie);

                construireTexte(index, nomFichierSortie);
            }
            break;

        case 7: // Quitter
            printf("--- %s ---\n", LM[choix]);
            printf("Liberation de la memoire et fermeture...\n");

            free(LM); //je libere le menu
            free(Point);
            break;

        default:
            printf("Choix invalide.\n");
            break;
        }

        if (choix != 7) {
            printf("\n");
        }
        system("pause");
    } while(choix != 7);

    return 0;
}
