
#include "lib/hash.h"
#include "lib/pair.h"
#include "stdlib.h"

#include "lib/helpers.h"

void PairInitWrapper(struct Pair *pair, int x, int y)
{
	if (x < y)
		PairInit(pair, x ,y);
	else
		PairInit(pair, y, x);
}

void SavePair(int *nums, int i, int j, int sum, struct Hash *hash)
{
	struct Pair *pair = NULL;
	int x = nums[i], y = nums[j];

	int key = sum - x - y, *new_key = NULL;
	struct HashNode *hash_node = NULL;
//	printf("%s:%d x %d y %d\n", __FUNCTION__, __LINE__, x, y);

	if ((hash_node = HashGet(hash, &key)) != NULL)
	{
		struct Pair current_pair;
		PairInitWrapper(&current_pair, i, j);

		pair = hash_node->val;
//		PairDump(pair);
//		PairDump(&current_pair);

		if (PairEqual(pair, &current_pair))
		{
//			printf("Equal\n");
			return;
		}
	}

	new_key = malloc(1 * sizeof(int));
	*new_key = sum - x - y;

	pair = malloc(1 * sizeof(struct Pair));
	PairInitWrapper(pair, i, j);

	HashInsert(hash, new_key, pair);
}

struct Pair* FindTwoSum(int *nums, int i, int j, int sum, struct Hash *hash)
{
	struct Pair *pair = NULL;
	int x = nums[i], y = nums[j];

	int key = x + y;
	 struct HashNode *hash_node = NULL;
//	printf("%s:%d x %d y %d\n", __FUNCTION__, __LINE__, x, y);

	if ((hash_node = HashGet(hash, &key)) != NULL)
	{
		struct Pair current_pair;
		PairInitWrapper(&current_pair, i, j);

		pair = hash_node->val;
//		PairDump(pair);
//		PairDump(&current_pair);

		if (PairOverlap(pair, &current_pair))
		{
//			printf("Overlap\n");
			pair = NULL;
		}
	}

	SavePair(nums, i, j, sum, hash);

	return pair;
}

void FourSum(int *nums, int len, int sum)
{
	int i = 0, j = 0;
	struct Hash hash; // int -> Pair

	HashInit(&hash, FuncIntToInt, FuncIntCompare);

	for (i = 0; i < len; ++i)
	{
		for (j = i + 1; j < len; ++j)
		{
			struct Pair *pair = NULL;

			if ((pair = FindTwoSum(nums, i, j, sum, &hash)) != NULL)
				printf("Result %d %d %d %d\n", nums[i], nums[j], nums[pair->x], nums[pair->y]);
		}
	}
}

#define kInputSize 6

int main(int argc, char *argv[])
{
	int nums[kInputSize] = {1, 0, -1, 0, -2, 2};
	int target = 0;

//	int nums[kInputSize] = {2, 2, 2, 2, 2};
//	int target = 8;

	FourSum(nums, kInputSize, target);

	return 0;
}

