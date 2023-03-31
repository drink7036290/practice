
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "lib/helpers.h"
#include "lib/hash.h"

int PermutationsHelper(char *str, char *prefix)
{
	int index = 0, len = strlen(str), cn = 0;

	if (len == 0)
	{
		printf("%s\n", prefix);
		return 1;
	}

	while (str[index] != '\0')
	{
		char *substr = StrConcat(Substring(str, 0, index - 1), Substring(str, index + 1, len - 1));
		char *sub_prefix = StrConcatChar(prefix, str[index]);

//		printf("substr %s prefix %s\n", substr, sub_prefix);

		cn += PermutationsHelper(substr, sub_prefix);

		++index;
	}

	return cn;
}

int Permutations(char *str)
{
	return PermutationsHelper(str, NULL);
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

int PermutationsDupHelper(struct Hash *hash, int len, char *prefix)
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
		
		cn += PermutationsDupHelper(hash, len - 1, StrConcatChar(prefix, *key));
		
		++*freq;
	}

	return cn;
}

int PermutationsDup(char *str)
{
	int len = strlen(str);
	struct Hash hash_freq;

	HashInit(&hash_freq, FuncIntToInt, FuncIntCompare);

	PrepareHash(&hash_freq, str);

	return PermutationsDupHelper(&hash_freq, len, NULL);
}

int main(int argc, char *argv[])
{
	char str[] = "adac";

	printf("%d\n", Permutations(str));
	printf("(dedup) %d\n", PermutationsDup(str));

	return 0;
}

