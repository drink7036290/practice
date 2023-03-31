
#include "stdio.h"
#include "stdlib.h"

#define ROW_SIZE 4
#define COL_SIZE 5

int grid[ROW_SIZE][COL_SIZE] = 
	{
	  {0, 0, 0, 0, 0}
	 ,{0, 1, 0, 1, 0}
	 ,{0, 1, 0, 1, 0}
	 ,{0, 0, 0, 0, 0}
	};

int hasPath(int x1, int y1, int x2, int y2)
{
	if ((x2 < 0) || (x2 >= ROW_SIZE))
		return 0;

	if ((y2 < 0) || (y2 >= COL_SIZE))
		return 0;

	if ((x1 == x2) && (y1 == y2))
	{
//		printf("%d, %d\n", x2, y2);
		grid[x2][y2] = 2;
		return 1;
	}

	if (grid[x2][y2] == 1)
		return 0;

	if (hasPath(x1, y1, x2 - 1, y2) || (hasPath(x1, y1, x2, y2 - 1)))
	{
		grid[x2][y2] = 2;
		return 1;
	}

	return 0;
}

int dfs(int x1, int y1, int x2, int y2)
{
	int ret = 0, org = 0;

	if ((x1 < 0) || (x1 >= ROW_SIZE))
                return 0;

        if ((y1 < 0) || (y1 >= COL_SIZE))
                return 0;

        if ((x1 == x2) && (y1 == y2))
	{
		grid[x1][y1] = 3;
		return 1;
	}

	if (grid[x1][y1] == 1)
		return 0;

	if (grid[x1][y1] == 3)
		return 0;

	org = grid[x1][y1];
	grid[x1][y1] = 3;

	ret = dfs(x1, y1 - 1, x2, y2);

	if (! ret)
		ret = dfs(x1 + 1, y1, x2, y2);

	if (! ret)
		ret = dfs(x1, y1 + 1, x2, y2);

	if (! ret)
		ret = dfs(x1 - 1, y1, x2, y2);

	if (! ret)
                grid[x1][y1] = org;

        return ret;
}

int main(int argc, char *argv[])
{
	int i = 0, j = 0;

	printf("only right down %d\n", hasPath(0, 0, ROW_SIZE - 1, COL_SIZE - 1));
	printf("dfs %d\n", dfs(0, 0, ROW_SIZE - 1, COL_SIZE - 1));

	for (i = 0; i < ROW_SIZE; ++i)
	{
		for (j = 0; j < COL_SIZE; ++j)
			printf(" %d", grid[i][j]);

		printf("\n");
	}

	return 0;
}




