
#ifndef STRING_HELPER_H_
#define STRING_HELPER_H_

#include "string.h"

enum SortType
{
	  SortTypeDecreasing = 0
	, SortTypeNonDecreasing
};

typedef int (*FuncObjCompare)(void *obj1, void *obj2);

int FuncStrCompare(void *obj1, void *obj2);
int FuncIntCompare(void *obj1, void *obj2);

typedef void (*FuncObjDump)(void *obj);

void FuncStrDump(void *obj);
void FuncIntDump(void *obj);

typedef int (*FuncObjToInt)(void *obj);

int FuncStrToInt(void *obj);
int FuncIntToInt(void *obj);

int Max(int a, int b);
int Min(int a, int b);

long long MaxLL(long long a, long long b);
long long MinLL(long long a, long long b);

void Swap(int *a, int *b);
void SwapObj(void **obj1, void **obj2);

int StrIsEmpty(char *str);

char* StrConcat(char *str1, char *str2);
char *StrConcatChar(char *str, char c);

char* Substring(char *str, int start, int end);

#endif
