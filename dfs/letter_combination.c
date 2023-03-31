
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/list_node.h"

char** PrepareDigitsToChars(int num_digits)
{
	char **digit_to_char = calloc(num_digits, sizeof(char*));

	int i = 0, max_len = 4;
	for (i = 0; i < num_digits; ++i)
		digit_to_char[i] = calloc(max_len + 1, sizeof(char));

	digit_to_char[2] = "abc";
	digit_to_char[3] = "def";
	digit_to_char[4] = "ghi";
	digit_to_char[5] = "jkl";
	digit_to_char[6] = "mno";
	digit_to_char[7] = "pqrs";
	digit_to_char[8] = "tuv";
	digit_to_char[9] = "wxyz";

	return digit_to_char;
}

struct StrInfo
{
	char *str;
	struct ListNode hook;
};

void StrInfoInit(struct StrInfo *str_info, char *str)
{
	str_info->str = calloc(strlen(str) + 1, sizeof(char));
	strcpy(str_info->str, str);

	ListNodeInit(&str_info->hook, str_info);
}

void DumpList(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

//	printf("%s ", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct StrInfo *str_info = p->container;

		printf(" %s", str_info->str);
	}
	printf("\n");
}

void GetCombinationsHelper(char *digits, int index, char *prefix
		, char **digit_to_char, struct ListNode *list_results)
{
	char d = digits[index], *c = NULL;
	char *str = NULL;

	if (d == '\0')
	{
		struct StrInfo *str_info = calloc(1, sizeof(struct StrInfo));
		StrInfoInit(str_info, prefix);
		ListAddTail(list_results, &str_info->hook);

		return;
	}

	str = digit_to_char[d - '0'];
	c = str;

	while (*c != '\0')
	{
		prefix[index] = *c;

		GetCombinationsHelper(digits, index + 1, prefix, digit_to_char, list_results);

		++c;
	}
}

struct ListNode* GetCombinations(char *digits)
{
	int num_digits = 10;
	char **digit_to_char = PrepareDigitsToChars(num_digits);

	char *prefix = calloc(strlen(digits) + 1, sizeof(char));

	struct ListNode *list_results = calloc(1, sizeof(struct ListNode));
	ListInit(list_results);

	GetCombinationsHelper(digits, 0, prefix, digit_to_char, list_results);

	return list_results;
}


int main(int argc, char *argv[])
{
	struct ListNode *list = GetCombinations(argv[1]);
	DumpList(list);

	return 0;
}

