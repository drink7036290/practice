
#include <stdio.h>
#include <stdlib.h>

#include "../lib/graph.h"
#include "../lib/pair.h"

#define kNumCourse 4
#define kPrerequisitesPair 4

void GraphAddEdges(struct Graph *graph, struct Pairs *pairs)
{
	int i = 0;

	for (i = 0; i < pairs->num_pair; ++i)
	{
		struct Pair *pair = &pairs->array[i];

		GraphAddEdge(graph, &pair->y, &pair->x);
	}
}

void GraphAddVertices(struct Graph *graph, int num_course)
{
	int i = 0;

	for (i = 0; i < num_course; ++i)
	{
		int *val = malloc(1 * sizeof(int));		
		*val = i;
	
		GraphAddVertex(graph, val);
	}
}

void PrintSchedule(int num_course, struct Pairs *pairs)
{
	struct Graph graph;
	struct ListNode stack;
	int ret = 0;
	struct ListNode *head = &stack, *p = NULL, *n = NULL;

	GraphInit(&graph, HashIntToIndex, FuncIntCompare, FuncIntDump);

	GraphAddVertices(&graph, num_course);

	GraphAddEdges(&graph, pairs);

	GraphDump(&graph);

	ListInit(&stack);

	ret = (GraphDFS(&graph, &stack, NULL, NULL) != NULL); // from NULL, to NULL

	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct Vertex *vertex = p->container;

		if (ret)
			ListNodeDelete(p);
		else
			FuncIntDump(vertex->val);
	}

	if (ret == 0)
		printf("\n");

}

int main(int argc, char *argv[])
{
	struct Pairs pairs;
	PairsInit(&pairs, kPrerequisitesPair);

	PairInit(&pairs.array[0], 1, 0);
	PairInit(&pairs.array[1], 2, 0);
	PairInit(&pairs.array[2], 3, 1);
	PairInit(&pairs.array[3], 3, 2);

	PairsDump(&pairs);
	
	PrintSchedule(kNumCourse, &pairs);

	return 0;
}

