
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int FuncStrCompare(void *obj1, void *obj2)
{
	char *str1 = obj1, *str2 = obj2;

	return strcmp(str1, str2);
}

int FuncIntCompare(void *obj1, void *obj2)
{
//	return (*((int*)obj1)) - (*((int*)obj2)); // overflow

	int *val1 = obj1;
	int *val2 = obj2;

	if (*val1 > *val2)
		return 1;

	if (*val1 == *val2)
		return 0;

	return -1;	
}

void FuncStrDump(void *obj)
{
	char *str = obj;

	printf(" %s", str);
}

void FuncIntDump(void *obj)
{
	int *val = obj;
 
	printf(" %d", *val);
}

int FuncStrToInt(void *obj)
{
	char *str = obj, c = '\0';
	unsigned int magic = 5381;

	while ((c = *str) != '\0')
	{
		magic = ((magic << 5) + magic) + c; // magic * 33 + c ; overflow is ok
		++str;
	}

	return magic; // unsigned to signed
}

int FuncIntToInt(void *obj)
{
	int *val = obj;
	return *val;
}

int Max(int a, int b)
{
	return (a > b) ? a : b ;
}

int Min(int a, int b)
{
	return (a < b) ? a : b ;
}

long long MaxLL(long long a, long long b)
{
	return (a > b) ? a : b ;
}

long long MinLL(long long a, long long b)
{
	return (a < b) ? a : b ;
}

void Swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void SwapObj(void **obj1, void **obj2)
{
	void *tmp = *obj1;
	*obj1 = *obj2;
	*obj2 = tmp;
}

int StrIsEmpty(char *str)
{
	if (str == NULL)
		return 1;

	if (*str == '\0')
		return 1;

	return 0;
}

char* StrConcat(char *str1, char *str2)
{
	char *merged_str = NULL, *pos = NULL;

	int len1 = (str1 == NULL) ? 0 : strlen(str1) ;
	int len2 = (str2 == NULL) ? 0 : strlen(str2) ;

	merged_str = calloc(len1 + len2 + 1, sizeof(char));

	pos = merged_str;

	if (str1 != NULL)
		strncpy(pos, str1, len1);

	pos += len1;

	if (str2 != NULL)
		strncpy(pos, str2, len2);

	return merged_str;
}

char *StrConcatChar(char *str, char c)
{
	char *tmp_str = calloc(1 + 1, sizeof(char));
	*tmp_str = c;

	return StrConcat(str, tmp_str);
}

char* Substring(char *str, int start, int end)
{
	int len = 0, valid = 1;
	char *substring = NULL;

	if ((0 > start) || (start > end) || (end > strlen(str)))
		valid = 0;
	else
		len = end - start + 1;

	substring = calloc(len + 1, sizeof(char));

	if (valid)
		strncpy(substring, str + start, len);

	return substring;
}

