
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

struct Manacher
{
	char *str;
	int len;

	int *results;
};

int ManacherGetMaxResult(struct Manacher *manacher)
{
	int i = manacher->len - 1, max = INT_MIN;

	while (i >= 0)
	{
		if (manacher->results[i] > max)
		{
			max = manacher->results[i];
			printf("max %d (%s center %d)\n", max, manacher->str, i);
		}

		--i;
	}

	return max;
}

int ManacherExpand(struct Manacher *manacher, int center, int radius)
{
	while ((center - radius >= 0) && (center + radius < manacher->len))
	{
		if (manacher->str[center - radius] != manacher->str[center + radius])
			break;

		++radius;
	}

	manacher->results[center] = --radius;

	return radius;
}

int ManacherFind(struct Manacher *manacher)
{
	int center = 0, radius = 0;

	while (center < manacher->len)
	{
		int org_center = center;
		int org_radius = ManacherExpand(manacher, center, radius);

		++center;
		radius = 0;

		while (center <= org_center + org_radius) // <=
		{
			int mirror_center = org_center - (center - org_center);
			int max_mirror_radius = org_center + org_radius - center;

			if (manacher->results[mirror_center] < max_mirror_radius)
				manacher->results[center++] = manacher->results[mirror_center];
			else if (manacher->results[mirror_center] > max_mirror_radius)
				manacher->results[center++] = max_mirror_radius;
			else
			{
				radius = max_mirror_radius;
				break;
			}
		}
	}

	return ManacherGetMaxResult(manacher);
}

void ManacherInit(struct Manacher *manacher, char *str, int len)
{
	int i = 0, index = 0;

	manacher->len = (len << 1) - 1; // 2n - 1
	manacher->str = calloc(manacher->len + 2, sizeof(char)); // last '|' + '\0'

	for (i = 0; i < len; ++i)
	{
		manacher->str[index++] = str[i];
		manacher->str[index++] = '|';
	}

	manacher->results = calloc(manacher->len, sizeof(int));
}

int Manacher(char *str)
{
	int len = strlen(str);

	if (len > 0)
	{
		struct Manacher manacher;

		ManacherInit(&manacher, str, len);
		return ManacherFind(&manacher);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	char str[] =
//		"babadabba";
//		"babad";
		"cbbd";

	printf("%d\n", Manacher(str));

	return 0;
}

