#pragma once

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4
#define KEY_SPACE 5
#define KEY_ENTER 6
#define KEY_ESCAPE 7
#define EMPTY -1

void MenuEscape(int op);
void MostraLOS(int(*los)[10]);
int Getch();
void MenuInicial(int op);
void clrscr();
void gotoxy(int x, int y);