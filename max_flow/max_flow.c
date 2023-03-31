
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

#include "../lib/graph.h"

void PrepareEdge(struct Graph *graph, char *from, char *to, int val)
{
	struct Edge *edge = GraphAddEdge(graph, from, to);

	edge->weight = val;
}

void PrepareGraph(struct Graph *graph, char ***equations, int num_equ_row, int *values)
{
	int i = 0;

	for (i = 0; i < num_equ_row; ++i)
	{
		GraphAddVertex(graph, equations[i][0]);
		GraphAddVertex(graph, equations[i][1]);

		PrepareEdge(graph, equations[i][0], equations[i][1], values[i]);
	}
}

void DumpList(struct ListNode *list_edge, struct Graph *graph)
{
	struct ListNode *head = list_edge, *p = NULL;

	printf("%s\n", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct Edge *edge = p->container;
		struct Vertex *vertex_from = edge->vertex_from;

		FuncStrDump(vertex_from->obj);
		printf(" (%d)", edge->weight);
		FuncStrDump(edge->obj);
		printf("\n");
	}
}

int VertexGetPath(struct Vertex *vertex_from, struct Graph *graph, struct Vertex *vertex_to, struct ListNode *list_edge)
{
	struct ListNode *head = &vertex_from->hash_edge.list_nodes, *p = NULL;

	if (vertex_from->state == VertexStateDone)
                return 0;

	vertex_from->state = VertexStateDone;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Edge *edge = hash_node->val;

		struct Vertex *vertex_next = NULL;

		int got_path = 0;

		vertex_next = HashGet(&graph->hash_vertex, edge->obj);

		if (vertex_next == vertex_to) // complete path
			got_path = 1;
		else
			got_path = VertexGetPath(vertex_next, graph, vertex_to, list_edge);

		if (got_path)
		{
			ListAddFront(list_edge, &edge->hook_flow);

			return 1;
		}
	}

	return 0;
}

int VertexBFS(struct Vertex *vertex_from, struct Graph *graph, struct Vertex *vertex_to, struct ListNode *list_BFS, struct ListNode *list_edge)
{
	struct ListNode *head = &vertex_from->hash_edge.list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Edge *edge = hash_node->val;

		struct Vertex *vertex_next = HashGet(&graph->hash_vertex, edge->obj);

		if (vertex_next == vertex_to) // complete path
		{
			ListAddFront(list_edge, &edge->hook_flow);

			while ((edge = vertex_from->edge_from) != NULL)
			{
				ListAddFront(list_edge, &edge->hook_flow);
				vertex_from = edge->vertex_from;
			}

			return 1;
		}

		if (vertex_next->state == VertexStateDone)
			continue;

		vertex_next->edge_from = edge;
		ListAddTail(list_BFS, &vertex_next->hook_DFS);
		vertex_next->state = VertexStateDone;
	}

	return 0;
}

int GraphGetPath(struct Graph *graph, char *from, char *to, struct ListNode *list_edge)
{
	struct Vertex *vertex_from = HashGet(&graph->hash_vertex, from);
	struct Vertex *vertex_to = HashGet(&graph->hash_vertex, to);

	struct ListNode list_BFS, *p = NULL;

	GraphResetVerticesState(graph);

	// DFS
//	return VertexGetPath(vertex_from, graph, vertex_to, list_edge);

	// BFS
	ListInit(&list_BFS);

	ListAddTail(&list_BFS, &vertex_from->hook_DFS);
	vertex_from->state = VertexStateDone;

	while ((p = ListPopFront(&list_BFS)) != NULL)
	{
		struct Vertex *vertex = p->container;

		if (VertexBFS(vertex, graph, vertex_to, &list_BFS, list_edge))
			return 1;
	}

	return 0;
}

void UpdateFlow(struct Graph *graph, struct ListNode *list_edge)
{
	struct ListNode *head = list_edge, *p = NULL, *n = NULL;
	int min_weight = INT_MAX;

	for (p = head->next; p != head; p = p->next)
	{
		struct Edge *edge = p->container;

		min_weight = Min(min_weight, edge->weight);
	}

	printf("min_weight %d\n", min_weight);

	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct Edge *edge = p->container, *invert_edge = NULL;
		struct Vertex* vertex_from = edge->vertex_from;

		// 1
		ListNodeDelete(p); // hook_flow

		// 2
		invert_edge = GraphAddEdge(graph, edge->obj, vertex_from->obj); // create or exist

		invert_edge->weight += min_weight;

		// 3
		if (edge->weight == min_weight)
			GraphDeleteEdge(graph, edge);
		else
			edge->weight -= min_weight;
	}
}

void max_flow(char ***equations, int *values, int num_equ_row)
{
	struct Graph graph;

	struct ListNode list_edge;

	GraphInit(&graph, FuncStrToInt, FuncStrCompare);

	PrepareGraph(&graph, equations, num_equ_row, values);

	GraphDump(&graph, FuncStrDump);

	ListInit(&list_edge);

	while (GraphGetPath(&graph, "s", "t", &list_edge))
	{
		DumpList(&list_edge, &graph);
		UpdateFlow(&graph, &list_edge);

		GraphDump(&graph, FuncStrDump);
	}
}

int main(int argc, char *argv[])
{
#define kNumEqu 9
	int values[kNumEqu] = {16, 13, 4, 12, 9, 14, 7, 20, 4};

	char ***equations = calloc(kNumEqu, sizeof(char **));

	int i = 0;
	
	for (i = 0; i < kNumEqu; ++i)
	{
		equations[i] = calloc(2, sizeof(char *));

		equations[i][0] = calloc(10, sizeof(char));
		equations[i][1] = calloc(10, sizeof(char));
	}

	equations[0][0] = "s";
	equations[0][1] = "v1";

	equations[1][0] = "s";
        equations[1][1] = "v2";

	equations[2][0] = "v2";
	equations[2][1] = "v1";

	equations[3][0] = "v1";
	equations[3][1] = "v3";

	equations[4][0] = "v3";
	equations[4][1] = "v2";

	equations[5][0] = "v2";
	equations[5][1] = "v4";

	equations[6][0] = "v4";
	equations[6][1] = "v3";

	equations[7][0] = "v3";
	equations[7][1] = "t";

	equations[8][0] = "v4";
	equations[8][1] = "t";

	max_flow(equations, values, kNumEqu);

	return 0;
}

