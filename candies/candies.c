
#include <stdio.h>
#include <stdlib.h>

#include "../lib/helpers.h"
#include "../lib/hash.h"

void CollectCandies(int *candies, int num_candy, struct Hash *hash_candy, int *total_types)
{
	int i = 0;

	for (i = 0; i < num_candy; ++i)
	{
		struct HashNode *hash_node = HashGet(hash_candy, &candies[i]);
		int *key = NULL;

		if (hash_node != NULL)
			continue;

		key = malloc(1 * sizeof(int));
		*key = candies[i];

		HashInsert(hash_candy, key, NULL);

		++*total_types;
	}
}

int GetTypes(int *candies, int num_candy)
{
	struct Hash hash_candy; // key: type
	int total_types = 0;

	HashInit(&hash_candy, HashIntToIndex, FuncIntCompare);

	CollectCandies(candies, num_candy, &hash_candy, &total_types);

	return Min(num_candy / 2, total_types);
}


int main(int argc, char *argv[])
{
#define kNumCandy 4
	int candies[kNumCandy] = {6,6,6,6};

	printf("%d\n", GetTypes(candies, kNumCandy));

	return 0;
}

