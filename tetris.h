#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <sys/time.h>

typedef struct Figure
{
    char **arr;
    char size;
    char y, x;
} Figure;

void PrintField();
void MakeNewFigure();
void Control(char move);
bool CheckCollisions(Figure F);
void CheckFullLines();
void ClearLines(int line);
void RotateFigure(Figure F);
void FillBaseValues();
void Init();
void SayHello();
void SayLose();

extern char gameField[20][10];
extern int score, fullLines, level, interval;
extern bool EnoughSpace, RightSide;

extern struct timeval start, end;

extern Figure Figures[7];

extern Figure CurrentFigure;
extern Figure NextFigure;

#endif