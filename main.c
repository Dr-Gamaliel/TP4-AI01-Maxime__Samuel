#include "tp4.h"
int main()
{
    char** LM;
    int choix;
    LM=malloc(sizeof *LM *9);
    initialisation(LM);
    do
    {
        /*CONSOLE_SCREEN_BUFFER_INFO BUF;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BUF);*/
        choix=menu(LM);//,BUF.dwCursorPosition);
        system("cls");
        switch (choix)
        {
        case 1:
            printf("---vous etes dans le sous menu \"%s\"---\n",LM[choix]);
            printf("%s\n",LM[8]);
            test();
            break;
        case 2:
            printf("---vous etes dans le sous menu \"%s\"---\n",LM[choix]);
            printf("%s\n",LM[8]);
            break;
        case 3:
            printf("---vous etes dans le sous menu \"%s\"---\n",LM[choix]);
            printf("%s\n",LM[8]);
            break;
        case 4:
            printf("---vous etes dans le sous menu \"%s\"---\n",LM[choix]);
            printf("%s\n",LM[8]);
            break;
        case 5:
            printf("---vous etes dans le sous menu \"%s\"---\n",LM[choix]);
            printf("%s\n",LM[8]);
            break;
        case 6:
            printf("---vous etes dans le sous menu \"%s\"---\n",LM[choix]);
            printf("%s\n",LM[8]);
            break;
        case 7:
            printf("---vous etes dans le sous menu \"%s\"---\n",LM[choix]);
            printf("sortie en cours...\n");
            break;
        default:
            printf("choix etrange\n");
            break;
        }
        system("pause");
    }while(choix!=7);
    return 0;
}
