
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/graph.h"
#include "../lib/pair.h"



void GraphAddVertices(struct Graph *graph, int num_nodes)
{
	int i = 0;

	for (i = 0; i < num_nodes; ++i)
	{
		int *val = malloc(1 * sizeof(int));
		*val = i + 1;

		GraphAddVertex(graph, val);
	}
}

void GraphAddEdges(struct Graph *graph, struct Pairs *edges)
{
	int i = 0;

	for (i = 0; i < edges->num_pair; ++i)
	{
		struct Pair *pair = &edges->array[i];

		int *x_val = malloc(1 * sizeof(int));
		int *y_val = malloc(1 * sizeof(int));

		int *index1 = malloc(1 * sizeof(int));
		int *index2 = malloc(1 * sizeof(int));

		struct Edge *edge = NULL;

		*x_val = pair->x;
		*y_val = pair->y;

		*index1 = i;
		*index2 = INT_MIN;

		edge = GraphAddEdge(graph, x_val, y_val);
		edge->user_data = index1;

		edge = GraphAddEdge(graph, y_val, x_val);
		edge->user_data = index2;
	}
}

void CompareEdge(struct Vertex *vertex, struct Edge *edge, int *max_index, struct Pair *redundant_edge)
{
	int *index = edge->user_data;

	FuncIntDump(vertex->val);
	FuncIntDump(edge->val);
	printf("\nindex %d max %d\n\n", *index, *max_index);

	if (*index <= *max_index)
		return;

	*max_index = *index;

	redundant_edge->x = *((int *)vertex->val);
	redundant_edge->y = *((int *)edge->val);
}

void CompareVertices(struct Vertex *vertex, struct Vertex *vertex_next, int *max_index, struct Pair *redundant_edge)
{
	struct HashNode *hash_node = NULL;
	struct Edge *edge = NULL;

	hash_node = HashGet(&vertex->hash_edge, vertex_next->val);
	edge = hash_node->val;
	CompareEdge(vertex, edge, max_index, redundant_edge);

	hash_node = HashGet(&vertex_next->hash_edge, vertex->val);
	edge = hash_node->val;
	CompareEdge(vertex_next, edge, max_index, redundant_edge);
}

void FindLargestIndexEdge(struct Graph *graph, struct Vertex *vertex_loop, struct ListNode *stack, struct Pair *redundant_edge)
{
	struct ListNode *head = stack, *p = NULL;
	int max_index = INT_MIN;

	struct Vertex *vertex = NULL, *vertex_next = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		vertex = p->container;

		if (p->next == head)
			break;

		vertex_next = p->next->container;

		CompareVertices(vertex, vertex_next, &max_index, redundant_edge);
	}

	CompareVertices(vertex, vertex_loop, &max_index, redundant_edge);
}

void FindRedundantEdge(int num_nodes, struct Pairs *edges)
{
	struct Graph graph;

	struct ListNode stack;
	struct Pair redundant_edge;

	struct Vertex *vertex_loop = NULL;
	struct ListNode *head = &stack, *p = NULL;
	
	GraphInit(&graph, HashIntToIndex, FuncIntCompare, FuncIntDump);

	GraphAddVertices(&graph, num_nodes);

	GraphAddEdges(&graph, edges);

	GraphDump(&graph);

	ListInit(&stack);
	PairInit(&redundant_edge, INT_MIN, INT_MAX); // init to non-exist edge

	// from NULL, to NULL
	if ((vertex_loop = GraphDFS(&graph, &stack, NULL, NULL)) != NULL) // detect loop
		FindLargestIndexEdge(&graph, vertex_loop, &stack, &redundant_edge);

	printf("stack\n");
	for (p = head->next; p != head; p = p->next)
	{
		struct Vertex *vertex = p->container;

		VertexDump(vertex, FuncIntDump);
	}

	PairDump(&redundant_edge);
}
/*
#define kNumNode 3
#define kNumEdge 3
*/
#define kNumNode 5
#define kNumEdge 5

int main(int argc, char *argv[])
{
	struct Pairs edges;
	PairsInit(&edges, kNumEdge);
/*
	PairInit(&edges.array[0], 1, 2);
	PairInit(&edges.array[1], 1, 3);
	PairInit(&edges.array[2], 2, 3);
*/
	PairInit(&edges.array[0], 1, 2);
	PairInit(&edges.array[1], 2, 3);
	PairInit(&edges.array[2], 3, 4);
	PairInit(&edges.array[3], 1, 4);
	PairInit(&edges.array[4], 1, 5);

	PairsDump(&edges);
	
	FindRedundantEdge(kNumNode, &edges);

	return 0;
}

