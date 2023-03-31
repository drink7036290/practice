
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

struct ParensInfo
{
	char left;
	char right;

	int cn_left;
	int cn_right;

	struct ParensInfo *prev;
};

void ParensInfoInit(struct ParensInfo *parens_info, char left, char right, int cn)
{
	parens_info->left = left;
	parens_info->right = right;

	parens_info->cn_left = cn;
	parens_info->cn_right = cn;

	parens_info->prev = NULL;
}

int PrintParensHelper(char *str, int index, struct ParensInfo *parens_infos, int num_types, struct ParensInfo *left_info)
{
	int i = 0, ret = 0, cn_ready = 0;

	for (i = 0; i < num_types; ++i)
	{
		struct ParensInfo *parens_info = &parens_infos[i];
		// 0 <= cn_left <= cn_right

		if (parens_info->cn_left < 0)
			return 0;

		if (parens_info->cn_left > parens_info->cn_right)
			return 0;

		cn_ready += (parens_info->cn_right == 0);
	}

	if (cn_ready == num_types)	
	{
		printf("%s\n", str);
		return 1;
	}

	for (i = 0; i < num_types; ++i)
        {       
                struct ParensInfo *parens_info = &parens_infos[i];
		struct ParensInfo *prev = parens_info->prev; // [TRICKEY !!]

		str[index] = parens_info->left;

		parens_info->prev = left_info;
		--parens_info->cn_left;

		ret += PrintParensHelper(str, index + 1, parens_infos, num_types, parens_info);

		++parens_info->cn_left;
		parens_info->prev = prev; // [RESTORE !!]

		// ======
		if (left_info != NULL)
		{
			if (left_info->right != parens_info->right)
				continue;
		}
		// ======

		str[index] = parens_info->right;

		--parens_info->cn_right;
		ret += PrintParensHelper(str, index + 1, parens_infos, num_types, (left_info == NULL) ? left_info : left_info->prev); // [TRICKEY !!]
		++parens_info->cn_right;
	}

	return ret;
}

void PrintParens(int num)
{
#define kNumTypes 3
	struct ParensInfo *parens_infos = calloc(kNumTypes, sizeof(struct ParensInfo));
	int ret = 0;

	char *str = calloc(num * kNumTypes + 1, sizeof(char));

	ParensInfoInit(&parens_infos[0], '(', ')', num);
	ParensInfoInit(&parens_infos[1], '[', ']', num);
	ParensInfoInit(&parens_infos[2], '{', '}', num);

	ret = PrintParensHelper(str, 0, parens_infos, kNumTypes, NULL);

	printf("total %d\n", ret);

	free(str);
}

int main(int argc, char *argv[])
{
	PrintParens(atoi(argv[1]));

	return 0;
}

