
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

#include "../lib/graph.h"
#include "../lib/heap.h"

struct VertexData
{
	int distance;
	struct Vertex *previous;

	struct HeapNode *heap_node;
};

void VertexDataDump(struct VertexData *vertex_data)
{
	printf("d %d previous ", vertex_data->distance);
	if (NULL == vertex_data->previous)
		printf("NULL");
	else
		FuncStrDump(vertex_data->previous->val);
	printf("\n");
}

void FuncVertexDataDump(void *obj)
{
	VertexDataDump(obj);
}

void VertexDataInit(struct VertexData *vertex_data)
{
	vertex_data->distance = INT_MAX;
	vertex_data->previous = NULL;

	vertex_data->heap_node = NULL;
}

void FuncVertexDump(void *obj)
{
	struct Vertex *vertex = obj;
	struct VertexData *vertex_data = vertex->user_data;

	FuncStrDump(vertex->val);
	printf(" (d %d)", vertex_data->distance);
}

void PrepareEdge(struct Graph *graph, char *from, char *to, int val)
{
	struct Edge *edge = GraphAddEdge(graph, from, to);
	int *edge_val = NULL;

	if (edge->user_data == NULL)
		edge->user_data = calloc(1, sizeof(int));

	edge_val = edge->user_data;
	*edge_val = val;
}

void PrepareVertex(struct Graph *graph, char *val)
{
	struct Vertex *vertex = GraphAddVertex(graph, val);
	struct VertexData *vertex_data = calloc(1, sizeof(struct VertexData));
	
	VertexDataInit(vertex_data);
	vertex->user_data = vertex_data;
}

void PrepareGraph(struct Graph *graph, char ***equations, int num_equ_row, int *values)
{
	int i = 0;

	for (i = 0; i < num_equ_row; ++i)
	{
		PrepareVertex(graph, equations[i][0]);
		PrepareVertex(graph, equations[i][1]);

		PrepareEdge(graph, equations[i][0], equations[i][1], values[i]);
	}
}

int VertexDataRelax(struct VertexData *from, int val, struct VertexData *to)
{
	int distance = from->distance + val;
	int ret = 0;

	printf(" from %d", to->distance);
	if (to->distance > distance)
	{
		to->distance = distance;
		ret = 1;
	}
	printf(" to %d (%d + %d)\n", to->distance, from->distance, val);

	return ret;
}

void VertexRelax(struct Vertex *vertex, struct Hash *hash_vertex, struct ListNode *list_BFS)
{
	struct ListNode *head = &vertex->hash_edge.list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Edge *edge = hash_node->val;

		struct Vertex *next_vertex = NULL;
		int *edge_val = NULL;

		hash_node = HashGet(hash_vertex, edge->val);
		next_vertex = hash_node->val;

		edge_val = edge->user_data;

		printf("Relax");
		FuncStrDump(next_vertex->val);
		VertexDataRelax(vertex->user_data, *edge_val, next_vertex->user_data);

		if (next_vertex->state == VertexStateDone)
			continue;

		next_vertex->state = VertexStateDone;
		ListAddTail(list_BFS, &next_vertex->hook_DFS);
	}
}

void GraphRelax(struct Graph *graph)
{
/*	struct ListNode *head = &graph->hash_vertex.list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Vertex *vertex = hash_node->val;

		VertexRelax(vertex, &graph->hash_vertex);
	}
*/
	struct HashNode *hash_node = HashGet(&graph->hash_vertex, "s");
	struct Vertex *vertex = hash_node->val;
	struct VertexData *vertex_data = vertex->user_data;

	struct ListNode list_BFS;
	
	vertex_data->distance = 0;

	ListInit(&list_BFS);

	GraphResetVerticesState(graph); // reset

	vertex->state = VertexStateDone;
	ListAddTail(&list_BFS, &vertex->hook_DFS);

	while (! ListIsEmpty(&list_BFS))
	{
		struct ListNode *first = ListPopFront(&list_BFS);
		
		vertex = first->container;

		printf("==================\npick\n");
		FuncStrDump(vertex->val);
		printf(" state %d\n", vertex->state);

		VertexRelax(vertex, &graph->hash_vertex, &list_BFS);
	}
}

void BellmanFord(char ***equations, int *values, int num_equ_row)
{
	struct Graph graph;
	int i = 0, cn = 0;

	GraphInit(&graph, HashStrToIndex, FuncStrCompare);
	PrepareGraph(&graph, equations, num_equ_row, values);

	GraphDump(&graph, FuncStrDump, FuncVertexDataDump, FuncIntDump);

	cn = ListSize(&graph.hash_vertex.list_nodes);

	for (i = 1; i < cn; ++i) // 1 ~ cn-1
		GraphRelax(&graph);

	GraphDump(&graph, FuncStrDump, FuncVertexDataDump, FuncIntDump);
}

void SetStartVertex(struct Graph *graph, char *start)
{
	struct HashNode *hash_node = HashGet(&graph->hash_vertex, start);
	struct Vertex *vertex = hash_node->val;
	struct VertexData *vertex_data = vertex->user_data;

	vertex_data->distance = 0;
}

void HeapVertices(struct Heap *heap, struct Graph *graph)
{
	struct ListNode *head = &graph->hash_vertex.list_nodes, *p = NULL;

	SetStartVertex(graph, "s");

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Vertex *vertex = hash_node->val;

		struct HeapNode *heap_node = HeapInsert(heap, vertex);
		struct VertexData *vertex_data = vertex->user_data;
		vertex_data->heap_node = heap_node;
	}

	HeapDump(heap, FuncVertexDump);
}

void DijkstraVertexRelax(struct Vertex *vertex, struct Hash *hash_vertex, struct Heap *heap)
{
	struct ListNode *head = &vertex->hash_edge.list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Edge *edge = hash_node->val;

		struct Vertex *next_vertex = NULL;
		int *edge_val = NULL;

		hash_node = HashGet(hash_vertex, edge->val);
		next_vertex = hash_node->val;

		edge_val = edge->user_data;

		printf("Relax");
		FuncStrDump(next_vertex->val);

		if (VertexDataRelax(vertex->user_data, *edge_val, next_vertex->user_data))
		{
			struct VertexData *vertex_data = next_vertex->user_data;

			HeapNodeUp(vertex_data->heap_node, heap->func_obj_compare, heap->func_obj_update_heap_node);
		}
	}
}

void DumpList(struct ListNode *list_vertex)
{
	struct ListNode *head = list_vertex, *p = NULL;

	printf("%s", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct Vertex *vertex = p->container;
		FuncStrDump(vertex->val);
	}
	printf("\n");
}

void Dijkstra(char ***equations, int *values, int num_equ_row)
{
	struct Graph graph;
	struct Vertex *vertex = NULL;

	struct Heap heap;

	struct ListNode list_path;

	GraphInit(&graph, HashStrToIndex, FuncStrCompare);
	PrepareGraph(&graph, equations, num_equ_row, values);

//	GraphDump(&graph, FuncStrDump, FuncVertexDataDump, FuncIntDump);

	HeapInit(&heap, kMinHeap, FuncVertexCompare);
	heap.func_obj_update_heap_node = FuncVertexUpdateHeapNode;

	ListInit(&list_path);
	
	HeapVertices(&heap, &graph);

	GraphDump(&graph, FuncStrDump, FuncVertexDataDump, FuncIntDump);

	while ((vertex = HeapPop(&heap)) != NULL) // extract min
	{
		ListAddTail(&list_path, &vertex->hook_DFS);

		printf("\nmin");
		FuncStrDump(vertex->val);
		printf("\n");

		HeapDump(&heap, FuncVertexDump);

		DijkstraVertexRelax(vertex, &graph.hash_vertex, &heap);
	}

	DumpList(&list_path);
}

int main(int argc, char *argv[])
{
/* Bellman_Ford
#define kNumEqu 10
	int values[kNumEqu] = {6, 7, 8, 9, 5, -2, 2, -3, -4, 7};
*/
#define kNumEqu 10
	int values[kNumEqu] = {10, 5, 2, 2, 1, 3, 7, 9, 4, 6};

	char ***equations = calloc(kNumEqu, sizeof(char **));

	int i = 0;
	
	for (i = 0; i < kNumEqu; ++i)
	{
		equations[i] = calloc(2, sizeof(char *));

		equations[i][0] = calloc(10, sizeof(char));
		equations[i][1] = calloc(10, sizeof(char));
	}
// y->t
	equations[0][0] = "s";
	equations[0][1] = "t";

	equations[1][0] = "s";
        equations[1][1] = "y";

	equations[2][0] = "t";
	equations[2][1] = "y";

	equations[3][0] = "y";
	equations[3][1] = "z";

	equations[4][0] = "t";
	equations[4][1] = "x";

	equations[5][0] = "y";
	equations[5][1] = "t";

	equations[6][0] = "z";
	equations[6][1] = "s";

	equations[7][0] = "y";
	equations[7][1] = "x";

	equations[8][0] = "x";
	equations[8][1] = "z";

	equations[9][0] = "z";
	equations[9][1] = "x";
	
	Dijkstra(equations, values, kNumEqu);
/*
	equations[0][0] = "s";
	equations[0][1] = "t";

	equations[1][0] = "s";
        equations[1][1] = "y";

	equations[2][0] = "t";
	equations[2][1] = "y";

	equations[3][0] = "y";
	equations[3][1] = "z";

	equations[4][0] = "t";
	equations[4][1] = "x";

	equations[5][0] = "x";
	equations[5][1] = "t";

	equations[6][0] = "z";
	equations[6][1] = "s";

	equations[7][0] = "y";
	equations[7][1] = "x";

	equations[8][0] = "t";
	equations[8][1] = "z";

	equations[9][0] = "z";
	equations[9][1] = "x";
	
	BellmanFord(equations, values, kNumEqu);
*/
	return 0;
}

