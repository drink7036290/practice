
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

#include "lib/hash.h"

struct StrInfo
{
	char *str;
	int len;
};

void StrInfoInit(struct StrInfo *str_info)
{
	str_info->str = NULL;
	str_info->len = 0;
}

int SubstrIsEqual(char *v, char *p, int len)
{
	while ((*v != '\0') && (*p != '\0'))
	{
		if (*v != *p)
			return 0;

		if (--len == 0)
			break;

		++v;
		++p;
	}

	return 1;
}

int PatternMatchHelper(char *value, char *pattern, struct Hash *hash_pattern)
{
	int len = 0, i = 0, tmp = 0;

	char *v = value, *p = pattern;
	struct StrInfo *str_info = NULL;

	printf("v %c p %c\n", *v, *p);

	if (*p == '\0')
		return (*v == '\0');

	tmp = *p;
	if ((str_info = HashGet(hash_pattern, &tmp)) == NULL)
	{
		int *key = calloc(1, sizeof(int));
		*key = *p;

		str_info = calloc(1, sizeof(struct StrInfo));
		StrInfoInit(str_info);

		printf("new p %c\n", *p);

		HashInsert(hash_pattern, key, str_info);	
	}

	if (str_info->len > 0)
	{
		printf("substr %s %s len %d\n", v, str_info->str, str_info->len);
		if (! SubstrIsEqual(v, str_info->str, str_info->len))
		{
			printf("not equal\n");
			return 0;
		}

		printf("equal\n");
		return PatternMatchHelper(v + str_info->len, p + 1, hash_pattern);
	}

	len = strlen(value);
	for (i = 1; i <= len; ++i)
	{
		str_info->str = Substring(value, 0, i - 1); // [start, end]
		str_info->len = i;

		printf("\nlen %d str %s\n", i, str_info->str);

		if (PatternMatchHelper(v + str_info->len, p + 1, hash_pattern))
			return 1;
	}

	return 0;
}

void HashDump(struct Hash *hash_pattern)
{
	struct ListNode *head = &hash_pattern->list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		struct StrInfo *str_info = hash_node->val;

		printf("%c -> %s(%d)\n", *((char*)hash_node->key), str_info->str, str_info->len);
	}
}

int PatternMatch(char *value, char *pattern)
{
	struct Hash hash_pattern;
	HashInit(&hash_pattern, FuncIntToInt, FuncIntCompare);

	if (PatternMatchHelper(value, pattern, &hash_pattern))
	{
		HashDump(&hash_pattern);
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	char value[] = "catcatgocatgo";
	char pattern[] = "aabab";

	printf("%d\n", PatternMatch(value, pattern));

	return 0;
}

