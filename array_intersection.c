
#include "stdio.h"
#include "stdlib.h"

#define kStrLen1 6
#define kStrLen2 4

void FindIntersection(int *nums1, int len1, int *nums2, int len2)
{
	int i = 0, j = 0;

	if ((len1 <= 0) || (len2 <= 0))
		return;

	while ((i < len1) && (j < len2))
	{
		int val1 = nums1[i];
		int val2 = nums2[j];

		if (val1 == val2)
		{
			printf("%d\n", val1);
			++i;
			++j;
		}
		else if (val1 < val2)
			++i;
		else // val1 > val2
			++j;
	}
}

int main(int argc, char *argv[])
{
	int nums1[kStrLen1] = {1, 3, 9, 10, 17, 18};
	int nums2[kStrLen2] = {1, 4, 9, 10};

	FindIntersection(nums1, kStrLen1, nums2, kStrLen2);

	return 0;
}

