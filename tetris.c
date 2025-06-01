#include "tetris.h"
#include <time.h>
#include <stdlib.h>

Figure Figures[7] = {
    {(char *[]){(char []){1,1},(char []){1,1}}, 2},
    {(char *[]){(char []){0,1,0},(char []){0,1,0},(char []){1,1,0}}, 3},
    {(char *[]){(char []){0,1,0},(char []){0,1,0},(char []){0,1,1}}, 3},
    {(char *[]){(char []){0,0,0},(char []){1,1,0},(char []){0,1,1}}, 3},
    {(char *[]){(char []){0,0,0},(char []){0,1,1},(char []){1,1,0}}, 3},
    {(char *[]){(char []){0,1,0},(char []){1,1,1},(char []){0,0,0}}, 3},
    {(char *[]){(char []){0,0,0,0},(char []){1,1,1,1},(char []){0,0,0,0},(char []){0,0,0,0}}, 4}
};

char gameField[20][10] = {0};
int score = 0, fullLines = 0, level = 0, interval = 1e6;
bool EnoughSpace = TRUE, RightSide = TRUE;

struct timeval start, end;

Figure CurrentFigure;
Figure NextFigure;

void RotateFigure(Figure F)
{
	char temp;

    for (int i = 0; i < F.size; i++)
	{
        for (int j = i; j < F.size; j++)
		{
            temp = F.arr[i][j];
            F.arr[i][j] = F.arr[j][i];
            F.arr[j][i] = temp;
        }
    }

    for (int i = 0; i < F.size; i++)
	{
        for (int j = 0; j < F.size / 2; j++)
		{
            temp = F.arr[i][j];
            F.arr[i][j] = F.arr[i][F.size - j - 1];
            F.arr[i][F.size - j - 1] = temp;
        }
    }
}

void ClearLines(int line)
{
	for (int i = line; i > 0; i--)
	{
		for(int j = 0; j < 10; j++)
		{
			gameField[i][j] = gameField[i - 1][j];
		}
	}

	for (int i = 0; i < 10; i++)
	{
		gameField[0][i] = 0;
	}
}

void CheckFullLines()
{
	int f;

	for (int i = 0; i < 20; i++)
	{
		f = 1;

		for (int j = 0; j < 10; j++)
		{
			if (!(gameField[i][j])) 
			{
				f = 0;
				break;
			}
		}

		if (f)
		{
			ClearLines(i);
			score += 100;
			fullLines++;
			if (fullLines % 10 == 0)
			{
				level++;
				interval -= 100000;
			}
		}
	}
}

bool CheckCollisions(Figure F)
{
	for (int i = 0; i < F.size; i++)
	{
		for (int j = 0; j < F.size; j++)
		{
			if (F.arr[i][j] && ((F.y + i >= 20) || (F.x + j < 0) || (F.x + j >= 10) || (gameField[F.y + i][F.x + j])))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void Control(char move)
{
	Figure TempFigure = CurrentFigure;

	if (move == 's' || move == 'S')
	{
		TempFigure.y++;
		if(!(CheckCollisions(TempFigure)))
		{
			CurrentFigure.y++;
		}

		else
		{
			for (int i = 0; i < CurrentFigure.size; i++)
			{
				for(int j = 0; j < CurrentFigure.size; j++)
				{
					gameField[CurrentFigure.y + i][CurrentFigure.x + j] = CurrentFigure.arr[i][j] | gameField[CurrentFigure.y + i][CurrentFigure.x + j];
				}
			}

			CheckFullLines();
			MakeNewFigure();
		}
	}

	else if (move == 'a' || move == 'A')
	{
		TempFigure.x--;
		if(!(CheckCollisions(TempFigure)))
		{
			CurrentFigure.x--;
		}
	}

	else if (move == 'd' || move == 'D')
	{
		TempFigure.x++;
		if(!(CheckCollisions(TempFigure)))
		{
			CurrentFigure.x++;
		}
	}

	else if (move == 'w' || move == 'W')
	{
		RotateFigure(CurrentFigure);
		if(CheckCollisions(CurrentFigure))
		{
			RotateFigure(CurrentFigure);
			RotateFigure(CurrentFigure);
			RotateFigure(CurrentFigure);
		}
	}

	else if (move == ' ')
	{
		while(!(CheckCollisions(CurrentFigure)))
		{
			CurrentFigure.y++;
		}

		CurrentFigure.y--;

		for (int i = 0; i < CurrentFigure.size; i++)
		{
			for(int j = 0; j < CurrentFigure.size; j++)
			{
				gameField[CurrentFigure.y + i][CurrentFigure.x + j] = CurrentFigure.arr[i][j] | gameField[CurrentFigure.y + i][CurrentFigure.x + j];
			}
		}

		CheckFullLines();
		MakeNewFigure();
	}

	else if (move == '1')
	{
		RightSide = FALSE;
	}

	PrintField();
}

void MakeNewFigure()
{
	CurrentFigure = NextFigure;
	CurrentFigure.y = 0;
	CurrentFigure.x = rand() % (11 - CurrentFigure.size);

	NextFigure = Figures[rand() % 7];

	if(CheckCollisions(CurrentFigure))
	{
		EnoughSpace = FALSE;
	}
}

void PrintField()
{
	clear();

	mvprintw(2, (COLS / 2) - 20, " _____   ___   _____   ___   ___   ___");
	mvprintw(3, (COLS / 2) - 20, "|_   _| | __| |_   _| | _ \\ |_ _| / __|");
	mvprintw(4, (COLS / 2) - 20, "  | |   | _|    | |   |   /  | |  \\__ \\");
	mvprintw(5, (COLS / 2) - 20, "  |_|   |___|   |_|   |_|_\\ |___| |___/");

	mvvline(9, (COLS / 2) - 12, '<', 21);
	mvvline(9, (COLS / 2) - 11, '!', 21);
	mvvline(9, (COLS / 2) + 10, '!', 21);
	mvvline(9, (COLS / 2) + 11, '>', 21);
	mvvline(9, (COLS / 2) - 11, '!', 21);
	mvhline(29, (COLS / 2) - 10, '=', 20);
	mvprintw(30, (COLS / 2) - 10, "\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/");

	mvprintw(9, (COLS / 2) - 29, "FULL LINES: %3d", fullLines);
	mvprintw(10, (COLS / 2) - 29, "LEVEL: %8d", level);
	mvprintw(11, (COLS / 2) - 29, "SCORE: %8d", score);
	mvprintw(17, (COLS / 2) - 29, "NEXT FIGURE:");

	for (int i = 0; i < NextFigure.size; i++)
	{
		for (int j = 0; j < NextFigure.size; j++)
		{
			if(NextFigure.arr[i][j]) mvprintw(19 + i, (COLS / 2) - 29 + j * 2, "[]");
		}
	}

	if(RightSide)
	{
		mvprintw(9, (COLS / 2) + 14, "W - ROTATE");
		mvprintw(10, (COLS / 2) + 14, "A - LEFT");
		mvprintw(11, (COLS / 2) + 14, "D - RIGHT");
		mvprintw(12, (COLS / 2) + 14, "S - DOWN");
		mvprintw(13, (COLS / 2) + 14, "SPACE - DROP");
		mvprintw(14, (COLS / 2) + 14, "1 - REMOVE INSTRUCTIONS");
	}

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (gameField[i][j]) mvprintw(9 + i, (COLS / 2) - 10 + j*2, "[]");
			else mvprintw(9 + i, (COLS / 2) - 10 + j*2, " .");
		}
	}

	for (int i = 0; i < CurrentFigure.size; i++)
	{
		for (int j = 0; j < CurrentFigure.size; j++)
		{
			if (CurrentFigure.arr[i][j]) mvprintw(9 + i + CurrentFigure.y, (COLS / 2) - 10 + (j + CurrentFigure.x) * 2, "[]");
		}
	}

	refresh();
}

void FillBaseValues()
{
	NextFigure = Figures[rand() % 7];
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			gameField[i][j] = 0;
		}
	}
	score = 0;
	fullLines = 0;
	level = 0;
	interval = 1000000;
	EnoughSpace = TRUE;
	RightSide = TRUE;
	gettimeofday(&start, NULL);
}

void Init()
{
	srand(time(NULL));
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(1) | A_BOLD);
}

void SayHello()
{
	clear();

	mvprintw(9, COLS/2 - 54, " ___       ___      _________      ___           ________      ________      _____ ______      _________");
	mvprintw(10, COLS/2 - 54, "/\\  \\     /\\  \\    /\\  ______\\    /\\  \\         /\\   ____\\    /\\   __  \\    /\\   _ \\ _   \\    /\\  ______\\");
	mvprintw(11, COLS/2 - 54, "\\ \\  \\    \\ \\  \\   \\ \\ \\_____/_   \\ \\  \\        \\ \\  \\___/    \\ \\  \\/\\  \\   \\ \\  \\\\__\\ \\  \\   \\ \\ \\_____/_");
	mvprintw(12, COLS/2 - 54, " \\ \\  \\  __\\ \\  \\   \\ \\  ______\\   \\ \\  \\        \\ \\  \\        \\ \\  \\ \\  \\   \\ \\  \\|__| \\  \\   \\ \\  ______\\");
	mvprintw(13, COLS/2 - 54, "  \\ \\  \\|\\__\\_\\  \\   \\ \\ \\_____/_   \\ \\  \\_____   \\ \\  \\_____   \\ \\  \\_\\  \\   \\ \\  \\   \\ \\  \\   \\ \\ \\_____/_");
	mvprintw(14, COLS/2 - 54, "   \\ \\____________\\   \\ \\________\\   \\ \\_______\\   \\ \\_______\\   \\ \\_______\\   \\ \\__\\   \\ \\__\\   \\ \\________\\");
	mvprintw(15, COLS/2 - 54, "    \\/____________/    \\/________/    \\/_______/    \\/_______/    \\/_______/    \\/__/    \\/__/    \\/________/");

	mvaddstr(20, COLS/2 - 12, "PRESS ANY BUTTON TO ");

	attron(A_BLINK);

	addstr("PLAY");

	attroff(A_BLINK);

	getch();
}

void SayLose()
{
	clear();

	mvprintw(9, COLS/2 - 37, " ___           ________      ________      _________      ________");
	mvprintw(10, COLS/2 - 37, "/\\  \\         /\\   __  \\    /\\   ____\\    /\\  ______\\    /\\   __  \\");
	mvprintw(11, COLS/2 - 37, "\\ \\  \\        \\ \\  \\/\\  \\   \\ \\  \\___/_   \\ \\ \\_____/_   \\ \\  \\/\\  \\");
	mvprintw(12, COLS/2 - 37, " \\ \\  \\        \\ \\  \\ \\  \\   \\ \\_____  \\   \\ \\  ______\\   \\ \\   _  /_");
	mvprintw(13, COLS/2 - 37, "  \\ \\  \\_____   \\ \\  \\_\\  \\   \\/____/\\  \\   \\ \\ \\_____/_   \\ \\  \\\\\\_ \\_");
	mvprintw(14, COLS/2 - 37, "   \\ \\_______\\   \\ \\_______\\   /\\________\\   \\ \\________\\   \\ \\__\\\\ \\__\\");
	mvprintw(15, COLS/2 - 37, "    \\/_______/    \\/_______/   \\/________/    \\/________/    \\/__/ \\/__/");

	mvaddstr(20, COLS/2 - 8, "TRY AGAIN? ");

	attron(A_BLINK);

	addstr("(Y/N)");

	attroff(A_BLINK);
}