
#include "stdio.h"
#include "stdlib.h"

#define ROW_SIZE 8
#define COL_SIZE 8

int grid[ROW_SIZE][COL_SIZE] = 
{
	  {1, 1, 1, 1, 1, 1, 1, 1}
	, {1, 1, 1, 1, 1, 1, 0, 0}
	, {1, 0, 0, 1, 1, 0, 1, 1}
	, {1, 2, 2, 2, 2, 0, 1, 0}
	, {1, 1, 1, 2, 2, 0, 1, 0}
	, {1, 1, 1, 2, 2, 2, 2, 0}
	, {1, 1, 1, 1, 1, 2, 1, 1}
	, {1, 1, 1, 1, 1, 2, 2, 1}
};	


void paintFill_r(int x, int y, int org_color, int new_color)
{
	if ((x < 0) || (x >= ROW_SIZE))
		return;

	if ((y < 0) || (y >= COL_SIZE))
		return;

	if (grid[x][y] != org_color)
		return;

	grid[x][y] = new_color; // self first to avoid repeating

	paintFill_r(x - 1, y, org_color, new_color);
	paintFill_r(x + 1, y, org_color, new_color);
	paintFill_r(x, y - 1, org_color, new_color);
	paintFill_r(x, y + 1, org_color, new_color);
}

void paintFill(int x, int y, int new_color)
{
	int org_color = grid[x][y];

	if (org_color == new_color)
		return;

	paintFill_r(x, y, org_color, new_color);
}


int main(int argc, char *argv[])
{
	int i = 0, j = 0;
	
	paintFill(4, 4, 3);

	for (i = 0; i < ROW_SIZE; ++i)
	{
		for (j = 0; j < COL_SIZE; ++j)
			printf(" %d", grid[i][j]);

		printf("\n");
	}

	return 0;
}




