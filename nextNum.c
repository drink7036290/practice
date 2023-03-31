
#include "stdio.h"
#include "stdlib.h"

int get(int num, int isNext)
{
	int cn = 0, val = 0, org_num = num, i = 0;

	while (num > 0)
	{
		val = (num & 1);
		
//		printf("%s:%d num %d val %d isNext %d\n", __FUNCTION__, __LINE__, num, val, isNext);

		if (isNext)
			val = ! val;

//		printf("%s:%d num %d val %d cn %d\n", __FUNCTION__, __LINE__, num, val, cn);

		if (val)
		{
			num >>= 1; ++cn;
			continue;
		}

//		printf("%s:%d num %d cn %d\n", __FUNCTION__, __LINE__, num, cn);

		num >>= 1; ++cn;
		if (num == 0)
			return -1;

		val = (num & 1);
		if (isNext)
			val = ! val;

//		printf("%s:%d num %d val %d cn %d\n", __FUNCTION__, __LINE__, num, val, cn);

		if (val)
		{
			if (isNext)
				num |= 1; // change to 1
			else
				num &= ~1; // change to 0

			num <<= 1; --cn; // change to 0

			if (! isNext)
				num |= 1; // change to 1

			break;
		}

//		printf("%s:%d num %d cn %d\n", __FUNCTION__, __LINE__, num, cn);
	}

	if (num == 0)
		return -1;

//	printf("%s:%d num %d cn %d\n", __FUNCTION__, __LINE__, num, cn);

	if (cn == 0)
		return num;

	val = ~1;
	for (i = 0; i < cn; ++i)
		val <<= 1;
	val = ~val;

	while (cn > 0)
	{
		num <<= 1; --cn;
	}

//	printf("num %d left %d val %d\n", num, org_num & val, val);

	return num | (org_num & val);
}

int main(int argc, char *argv[])
{
	int num = 9;

	printf("before %d next %d\n", get(num, 0), get(num, 1));

	return 0;
}

