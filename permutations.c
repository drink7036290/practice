
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "lib/helpers.h"
#include "lib/hash.h"

int PermutationsHelper(char *str, char *prefix, int prefix_index)
{
	int len = strlen(str), i = 0, cn = 0;

	if (len == 0)
	{
		printf("%s\n", prefix);
		return 1;
	}

	for (i = 0; i < len; ++i)
	{
		char *substr_left = Substring(str, 0, i - 1);
		char *substr_right = Substring(str, i + 1, len - 1);
		char *substr = StrConcat(substr_left, substr_right);

		prefix[prefix_index + 1] = str[i];
		
//		printf("left %s right %s total %s prefix %s\n", substr_left, substr_right, substr, prefix);

		cn += PermutationsHelper(substr, prefix, prefix_index + 1);
	}

	return cn;
}

int Permutations(char *str)
{
	char *prefix = calloc(strlen(str) + 1, sizeof(char));

	return PermutationsHelper(str, prefix, -1);
}

// ====================================================================

void HashDump(struct Hash *hash)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		int *key = hash_node->key;
		int *freq = hash_node->val;

		printf("%c : %d\n", *key, *freq);
	}
}

void PrepareHash(struct Hash *hash, char *str)
{
	char *c = str;

	while (*c != '\0')
	{
		int tmp = *c;
		int *freq = HashGet(hash, &tmp);

		if (freq == NULL)
		{
			int *key = calloc(1, sizeof(int));
			*key = tmp;

			freq = calloc(1, sizeof(int));
			*freq = 0;

			HashInsert(hash, key, freq);
		}

		++*freq;
		++c;
	}

//	HashDump(hash);
}

int PermutationsDupHelper(struct Hash *hash, int len, char *prefix, int prefix_index)
{
	int cn = 0;
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	if (len == 0)
	{
		printf("%s\n", prefix);
		return 1;
	}

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		int *key = hash_node->key;
		int *freq = hash_node->val;

		if (*freq == 0)
			continue;

		--*freq;
		
		prefix[prefix_index + 1] = *key;
		cn += PermutationsDupHelper(hash, len - 1, prefix, prefix_index + 1);
		
		++*freq;
	}

	return cn;
}

int PermutationsDup(char *str)
{
	int len = strlen(str);
	char *prefix = calloc(len + 1, sizeof(char));
	struct Hash hash_freq;

	HashInit(&hash_freq, FuncIntToInt, FuncIntCompare);

	PrepareHash(&hash_freq, str);

	return PermutationsDupHelper(&hash_freq, len, prefix, -1);
}

int main(int argc, char *argv[])
{
	char str[] = "adac";

	printf("%d\n", Permutations(str));
	printf("(dedup) %d\n", PermutationsDup(str));

	return 0;
}

