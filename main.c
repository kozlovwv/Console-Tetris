#include "tetris.h"

int main()
{
	char move;
	char nextgame = 'Y';	

	Init();

	while(nextgame == 'Y' || nextgame == 'y')
	{
		SayHello();

		nodelay(stdscr, TRUE);

		FillBaseValues();

		MakeNewFigure();
		PrintField();

		while (EnoughSpace)
		{
			gettimeofday(&end, NULL);

			if ((move = getch()) != ERR)
			{
				Control(move);
			}

			else
			{
				if ((end.tv_sec * 1e6 + end.tv_usec) - (start.tv_sec * 1e6 + start.tv_usec) > interval)
				{
					Control('S');
					start = end;
				}
			}
		}

		clear();
		nodelay(stdscr, FALSE);

		mvprintw(LINES/2, COLS/2 - 8, "Total score: %d", score);
		getch();

		SayLose();

		nextgame = getch();
	}

	endwin();
	return 0;
}