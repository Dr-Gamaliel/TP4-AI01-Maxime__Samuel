#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <wincon.h>
#define KBUP 328
#define KBDOWN 336

void color(int t,int f);
void gotoxy(int x, int y);
void initialisation(char** LM);
int menu(char** tab, COORD O);


#endif // TP4_H_INCLUDED
