
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

int FlipToWin(int n, int cn_1)
{
	int start_index = 0, curr_index = -1, last_index = -1;
	int max_len = 0;

	while (n > 0)
	{
		++curr_index;

		if ((n & 1) == 0)
		{
			if (cn_1 == 0) // take 1 away
			{
				start_index = last_index + 1;
				last_index = -1;

				++cn_1;
			}

			// fill 1
			if (last_index == -1)
				last_index = curr_index;

			--cn_1;
		}

		max_len = Max(max_len, curr_index - start_index + 1);
		n >>= 1;
	}

	return max_len;
}


int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	int cn_1 = atoi(argv[2]);

	printf("input %d 0x%x cn_1 %d max %d\n", n, n, cn_1, FlipToWin(n, cn_1));

	return 0;
}

