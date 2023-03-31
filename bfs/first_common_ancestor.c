
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/BST.h"

struct BSTNode* FirstCommonAncestor(struct BSTNode *p, struct BSTNode *q)
{
	struct BSTNode *r = NULL;

	if (p == NULL)
		printf(" p NULL");
	else
		printf(" p %d", p->val);

	if (q == NULL)
		printf(" q NULL");
	else
		printf(" q %d", q->val);

	printf("\n");

	while ((p != NULL) && (q != NULL))
	{
		if (p->visited)
			return p;
		p->visited = 1;

		if (q->visited)
			return q;
		q->visited = 1;

		p = p->parent;
		q = q->parent;
	}

	r = (p != NULL) ? p : q ;
	while (r != NULL)
	{
		if (r->visited)
			return r;

		r = r->parent;
	}

	return NULL;
}

void TestFirstCommonAncestor(int num)
{
	struct BST bst;
//	struct BST bst2;
	struct BSTNode *bst_nodeA = NULL, *bst_nodeB = NULL, *ancestor = NULL;

	BSTInit(&bst);
//	BSTInit(&bst2);

	srand(time(NULL));

	while (num > 0)
	{
		int val = rand() % 100;

		struct BSTNode *bst_node = BSTInsert(&bst, val, NULL);
//		struct BSTNode *bst_node2 = BSTInsert(&bst2, val, NULL);

		if (bst_nodeA == NULL)
		{
			if ((rand() % 5) == 0)
				bst_nodeA = bst_node;
		}

		if (bst_nodeB == NULL)
		{
			if ((rand() % 5) == 0)
				bst_nodeB = bst_node;
		}

//		bst_nodeB = bst_node2;

		--num;
	}

	BSTDump(&bst);

	if ((ancestor = FirstCommonAncestor(bst_nodeA, bst_nodeB)) == NULL)
		printf("ancestor NULL\n");
	else
		printf("ancestor %d\n", ancestor->val);
}

int main(int argc, char *argv[])
{
	TestFirstCommonAncestor(atoi(argv[1]));

	return 0;
}

