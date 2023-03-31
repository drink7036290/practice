
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/helpers.h"
#include "../lib/list_node.h"

struct NumberNode
{
	int number;

	struct ListNode hook;
};

void NumberNodeInit(struct NumberNode *node, int number)
{
	node->number = number;
	ListNodeInit(&node->hook, node);
}

void PushNumber(struct ListNode* list_number, int number)
{
	struct NumberNode *node = malloc(1 * sizeof(struct NumberNode));

//	printf("number %d\n", number);

	NumberNodeInit(node, number);

	ListAddTail(list_number, &node->hook);
}

struct StringNode
{
	char *str;

	struct ListNode hook;
};

void StringNodeInit(struct StringNode *node, char *str)
{
	node->str = str;

	ListNodeInit(&node->hook, node);
}

void PushSubstring(struct ListNode* list_string, char *str, int start, int end)
{
	struct StringNode *node = NULL;

//	printf("start %d end %d\n", start, end);

	if ((start < 0) || (end < 0))
		return;

	node = malloc(1 * sizeof(struct StringNode));

	StringNodeInit(node, Substring(str, start, end));

	ListAddTail(list_string, &node->hook);
}

void AppendOrExport(struct ListNode* list_string, char *new_str, char **final_str)
{
	if (ListIsEmpty(list_string))
		*final_str = StrConcat(*final_str, new_str);
	else
	{
		struct ListNode *node = list_string->prev; // last
		struct StringNode *string_node = node->container;

		string_node->str = StrConcat(string_node->str, new_str);
	}
}

void BuildNewString(struct ListNode* list_number, struct ListNode* list_string, char **final_str)
{
	struct ListNode* node = NULL;

	struct NumberNode *number_node = NULL;
	struct StringNode *string_node = NULL;

	int number = 0;
	char *str = NULL, *new_str = NULL;

	if (ListIsEmpty(list_number) || ListIsEmpty(list_string))
		return;

	node = ListPopTail(list_number);
	number_node = node->container;
	number = number_node->number;
	free(number_node);

	node = ListPopTail(list_string);
	string_node = node->container;
	str = string_node->str;
	free(string_node);

	while (number > 0)
	{
		new_str = StrConcat(new_str, str);

		--number;
	}

	free(str);

	AppendOrExport(list_string, new_str, final_str);
}

void Dump2Lists(struct ListNode* list_number, struct ListNode* list_string)
{
	struct ListNode *head = list_number, *p = NULL;
	
	printf("=====\nnumbers:\n");

	for (p = head->next; p != head; p = p->next)
	{
		struct NumberNode *node = p->container;

		printf(" %d", node->number);
	}

	printf("\nstrings:\n");

	head = list_string;

	for (p = head->next; p != head; p = p->next)
	{
		struct StringNode *node = p->container;

		printf(" %s", node->str);
	}

	printf("\n=====\n");
}

void DecodeString(char *str)
{
	struct ListNode list_number, list_string;

	char *tmp_str = str, c = '\0', *final_str = NULL;
	int accu_number = 0;
	int str_left_index = -1, str_right_index = -1, curr_index = -1;

	ListInit(&list_number);
	ListInit(&list_string);

	while ((c = *tmp_str) != '\0')
	{
		++curr_index;

		Dump2Lists(&list_number, &list_string);

		printf("'%c' curr_index %d\n", c, curr_index);

		if ((c - 'a' < 26) && (c >= 'a'))
		{
			if (ListIsEmpty(&list_number))
			{
				char *new_str = calloc(1 + 1, sizeof(char));
				*new_str = c;

				AppendOrExport(&list_string, new_str, &final_str);
			}
			else
			{
				if (str_left_index == -1)
					str_left_index = curr_index;

				str_right_index = curr_index;
			}
		}		
		else if ((c - '0' < 10) && (c >= '0'))
		{
//			printf("digit\n");

			if (str_left_index != -1)
			{
				PushSubstring(&list_string, str, str_left_index, str_right_index);
				str_left_index = str_right_index = -1;
			}

			accu_number = accu_number * 10 + (c - '0');
		}
		else if (c == '[')
		{
//			printf("[\n");

			PushNumber(&list_number, accu_number);
			accu_number = 0;
		}
		else // (input == ']')
		{
//			printf("]\n");

			if (ListSize(&list_number) == ListSize(&list_string))
				AppendOrExport(&list_string, Substring(str, str_left_index, str_right_index), &final_str);
			else
				PushSubstring(&list_string, str, str_left_index, str_right_index);
			str_left_index = str_right_index = -1;

			BuildNewString(&list_number, &list_string, &final_str);
		}

		++tmp_str;
	}

	printf("%s\n", final_str);
}

int main(int argc, char *argv[])
{
//	char str[] = "3[a]2[bc]";
	char str[] = "3[a2[c]e]";
//	char str[] = "ef2[abc]3[cd]ef";

	DecodeString(str);

	return 0;
}

