
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

#include "../lib/hash.h"

int CheckSubstring(char *str, int start, int end, struct Hash *hash_str)
{
	int i = 0;

	char *substring = NULL;
	struct HashNode *hash_node = NULL;

	for (i = start; i < end; ++i) // < end
	{
		substring = Substring(str, start, i);
		hash_node = HashGet(hash_str, substring);

		if (hash_node == NULL)
			continue;

		printf("%s found substring %s\n", str, substring);

		if (CheckSubstring(str, i + 1, end, hash_str))
			return 1;
	}

	if (start != 0)
	{
		substring = Substring(str, start, end); // == end
		hash_node = HashGet(hash_str, substring);

		if (hash_node != NULL)
			printf("%s found substring %s\n", str, substring);

		return (hash_node != NULL);
	}

	return 0;
}

char* CheckStrings(char** strs, int num_str, struct Hash *hash_str)
{
	int i = 0, max_len = 0;
	char *returned_str = NULL;

	for (i = 0; i < num_str; ++i)
	{
		int len = strlen(strs[i]);

		if (! CheckSubstring(strs[i], 0, len - 1, hash_str))
				continue;

		printf("%s:%d %s\n", __FUNCTION__, __LINE__, strs[i]);

		if (max_len < len)
		{
			max_len = len;
			returned_str = strs[i];
		}
	}

	return returned_str;
}

void BuildHashStr(char** strs, int num_str, struct Hash *hash_str)
{
	int i = 0;

	for (i = 0; i < num_str; ++i)
		HashInsert(hash_str, strs[i], NULL);
}

char* FindLongestStr(char** strs, int num_str)
{
	struct Hash hash_str;

	HashInit(&hash_str, HashStrToIndex, FuncStrCompare);

	BuildHashStr(strs, num_str, &hash_str);

	return CheckStrings(strs, num_str, &hash_str);
}

int main(int argc, char *argv[])
{
#define kNumDict 7
	char* D[kNumDict] = {"cat", "banana", "dog", "nana", "walk", "walker", "dogwalker"};

	printf("%s\n", FindLongestStr(D, kNumDict));

	return 0;
}

