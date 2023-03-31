
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"
#include "../lib/hash.h"

int CountWaysHelper(char *equ, int start, int end, bool target, struct Hash *hash_result)                               
{       
	int i = 0, ret = 0;
	char *str = NULL;
	int *str_cn = NULL;

	if (start == end)
		return (equ[start] == (target ? '1' : '0'));

	str = Substring(equ, start, end);
	str = StrConcatChar(str, target ? '1': '0');
	if ((str_cn = HashGet(hash_result, str)) != NULL)
		return *str_cn;

	// iterate op
	for (i = start + 1; i <= end; i += 2)                                                 
	{                                                                                     
		int left1 = CountWaysHelper(equ, start, i - 1, 1, hash_result);                            
		int left0 = CountWaysHelper(equ, start, i - 1, 0, hash_result);                            

		int right1 = CountWaysHelper(equ, i + 1, end, 1, hash_result);                             
		int right0 = CountWaysHelper(equ, i + 1, end, 0, hash_result);

		int total1 = 0;                                                               

		if (equ[i] == '&')                                                            
			total1 = left1 * right1;
		else if (equ[i] == '|')                                                       
			total1 = left1 * (right0 + right1) + left0 * right1;                  
		else // ^ 
			total1 = left1 * right0 + left0 * right1;                             

		if (target)
			ret += total1;                                                        
		else
			ret += (left1 + left0) * (right1 + right0) - total1;                  
	}                                                                                     

	str_cn = calloc(1, sizeof(int));
	*str_cn = ret;
	HashInsert(hash_result, str, str_cn);

	return ret;
}       

int CountWays(char *equ, bool target)
{
	struct Hash hash_result;

	HashInit(&hash_result, FuncStrToInt, FuncStrCompare);

	return CountWaysHelper(equ, 0, strlen(equ) - 1, target, &hash_result);
} 

int main(int argc, char *argv[])
{
//	char str[] = "1^0|0|1";
//	bool target = false;

	char str[] = "0&0&0&1^1|0";
	bool target = true;

	printf("%d\n", CountWays(str, target));

	return 0;
}

