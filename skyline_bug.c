
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

#include "lib/hash.h"

struct Point
{
	int x;
	int h;

	int is_end;

	struct ListNode hook;
};

int FuncPointCompare(void *obj1, void *obj2)
{
	struct Point *point1 = obj1;
	struct Point *point2 = obj2;

	if (point1->x != point2->x)
		return FuncIntCompare(&point1->x, &point2->x);

	return FuncIntCompare(&point1->is_end, &point2->is_end);
}

int FuncPointToInt(void *obj)
{
	struct Point *point = obj;

	return ((point->x + point->h) * 5381);
}

void PointDump(struct Point *point)
{
	printf(" x %d h %d is_end %d\n", point->x, point->h, point->is_end);
}

void FuncPointDump(void *obj)
{
	PointDump(obj);
}

void PointInit(struct Point *point, int x, int h, int is_end)
{
	point->x = x;
	point->h = h;

	point->is_end = is_end;
	ListNodeInit(&point->hook, point);
}

void PreparePoint(struct Hash *hash, int x, int h, int is_end)
{
	struct Point *point = NULL, point_tmp;
	PointInit(&point_tmp, x, h, is_end);

	if ((point = HashGet(hash, &point_tmp)) == NULL)
	{
		point = calloc(1, sizeof(struct Point));
		PointInit(point, x, h, is_end);

		HashInsert(hash, point, point);
	}
	else if (point->h < h)
		point->h = h;
}

void HashDump(struct Hash *hash)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Point *point = hash_node->val;

		PointDump(point);
	}
}

void PrepareHash(struct Hash *hash, int **buildings, int num_building, int num_building_col)
{
	int i = 0;

	for (i = 0; i < num_building; ++i)
	{
		int s = buildings[i][0];
		int e = buildings[i][1];
		int h = buildings[i][2];

		PreparePoint(hash, s, h, 0);
		PreparePoint(hash, e, h, 1);
	}

	HashDump(hash);
	printf("\n");
}

struct Point* GenerateByCompare(struct Point *point_last, struct Point *point, int *last_h, struct ListNode *list_results)
{
	printf("last");
	PointDump(point_last);

	printf("curr");
	PointDump(point);	

	if (! point_last->is_end) // s1
	{
		if (! point->is_end) // s2
		{
//			struct Point *point_mid = NULL;

			printf("%d\n", __LINE__);

			if (point_last->h >= point->h) // h1 >= h2
			{
				*last_h = point->h;
				return point_last;
			}
/*
			printf("%d\n", __LINE__);

			point_mid = calloc(1, sizeof(struct Point));
			PointInit(point_mid, point->x, point_last->h, 0); // s2, h1
			ListAddTail(list_results, &point_mid->hook);
*/		}
		// e1
		else if (point_last->h > point->h)
		{
			printf("%d\n", __LINE__);
			*last_h = point->h;
			return point_last;
		}
	}
	else // e1
	{
		if (! point->is_end) // s2
		{
			printf("%d\n", __LINE__);
			point_last->h = 0;
		}
		// e2
		else if (point_last->h > point->h)
		{
//			struct Point *point_mid = NULL;

			printf("%d\n", __LINE__);
/*
			point_mid = calloc(1, sizeof(struct Point));
			PointInit(point_mid, point_last->x, point->h, 0); // e1, h2
			ListAddTail(list_results, &point_mid->hook);
*/
			if (*last_h > point->h)
			{
				point_last->h = *last_h;
				return point_last;
			}

			point_last->h = point->h;
			*last_h = point->h;
		}
		else if (point_last->h == point->h)
		{
			printf("%d\n", __LINE__);

			point_last->x = point->x;
			point_last->h = point->h;

			return point_last;
		}
	}

	printf("%d\n", __LINE__);
	ListAddTail(list_results, &point->hook);
	return point;
}

void GeneratePoints(struct Hash *hash, struct ListNode *list_results)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	struct HashNode *hash_node = NULL;
	struct Point *point = NULL, *point_last = NULL;

	int last_h = 0;

	// take first
	p = head->next;
	hash_node = p->container;

	point = hash_node->val;
	ListAddTail(list_results, &point->hook);

	point_last = point;
	last_h = point->h;

	// others
	p = p->next;

	for (; p != head; p = p->next)
	{
		hash_node = p->container;
		point = hash_node->val;

		GenerateByCompare(point_last, point, &last_h, list_results);

		point_last = point;
	}

	// last h 0
	p = list_results->prev;
	point = p->container;
	point->h = 0;
}

void DumpList(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

	printf("\n");
	for (p = head->next; p != head; p = p->next)
	{
		struct Point *point = p->container;

		PointDump(point);
	}
	printf("\n");
}

void GetSkyline(int **buildings, int num_building, int num_building_col)
{
	struct Hash hash;
	struct ListNode list_results;

	HashInit(&hash, FuncPointToInt, FuncPointCompare);

	PrepareHash(&hash, buildings, num_building, num_building_col);

	HashSort(&hash, SortTypeNonDecreasing, FuncPointDump);

	ListInit(&list_results);
	GeneratePoints(&hash, &list_results);

	DumpList(&list_results);
}

void Test(void)
{
#define kNumRow 5
#define kNumCol 3

	int matrix_ref[kNumRow][kNumCol] = {
		  {2,9,10}
		, {3,7,15}
		, {5,8,12} // 5,12,12
		,
		  {15,20,10}
		, {19,24,8}

//		  {0, 2, 3}
//		, {2, 5, 3}
	};
	
	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	for (i = 0; i < kNumRow; ++i)
	{
		matrix[i] = calloc(kNumCol, sizeof(int));
	
		for (j = 0; j < kNumCol; ++j)
			matrix[i][j] = matrix_ref[i][j];
	}

	GetSkyline(matrix, kNumRow, kNumCol);
}
int main(int argc, char *argv[])
{
	Test();

	return 0;
}

