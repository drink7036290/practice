
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

void PrepareGraph(struct Graph *graph, char ***equations, int num_equ_row, int *values, int bi_dir)
{
	int i = 0;

	for (i = 0; i < num_equ_row; ++i)
	{
		GraphAddVertex(graph, equations[i][0]);
		GraphAddVertex(graph, equations[i][1]);

		PrepareEdge(graph, equations[i][0], equations[i][1], values[i]);

		if (bi_dir)
			PrepareEdge(graph, equations[i][1], equations[i][0], values[i]);
	}
}

void DumpList(struct ListNode *list_edge, struct Graph *graph)
{
	struct ListNode *head = list_edge, *p = NULL;

	printf("%s\n", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct Edge *edge = p->container;
		
		struct Vertex *vertex_to = HashGet(&graph->hash_vertex, edge->obj);
		struct Vertex *vertex_from = edge->vertex_from;

		FuncStrDump(vertex_from->obj);
		printf(" (%d)", edge->weight);
		FuncStrDump(vertex_to->obj);
		printf("\n");
	}
}

void FuncEdgeDump(void *obj)
{
	struct Edge *edge = obj;

	EdgeDump(edge, FuncStrDump);
}

struct Vertex* FindSet(struct Vertex *vertex)
{
	if (vertex->set != vertex)
		vertex->set = FindSet(vertex->set);

	return vertex->set;
}

void LinkSets(struct Vertex *vertex1, struct Vertex *vertex2)
{
	if (vertex1->rank > vertex2->rank)
	{
		vertex2->set = vertex1;
		return;
	}

	vertex1->set = vertex2;
	
	if (vertex1->rank == vertex2->rank)
		++vertex2->rank;
}

void UnionSets(struct Vertex *vertex1, struct Vertex *vertex2)
{
	LinkSets(FindSet(vertex1), FindSet(vertex2));
}

void UnionFind(struct Graph *graph, struct ListNode *list_edge)
{
	struct ListNode *head = &graph->list_edge, *p = NULL, *n = NULL;

	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct Edge *edge = p->container;
		struct Vertex *vertex_to = HashGet(&graph->hash_vertex, edge->obj);
		struct Vertex *vertex_from = edge->vertex_from;
		
		if (FindSet(vertex_from) == FindSet(vertex_to))
			continue;
		
		ListNodeDelete(p);	
		ListAddTail(list_edge, p);
		
		UnionSets(vertex_from, vertex_to);
	}
}

void Kruskal(char ***equations, int *values, int num_equ_row)
{
	struct Graph graph;

	struct ListNode list_edge;

	GraphInit(&graph, FuncStrToInt, FuncStrCompare);

	PrepareGraph(&graph, equations, num_equ_row, values, 0);

	ListMergeSort(&graph.list_edge, SortTypeNonDecreasing, FuncEdgeCompare, FuncEdgeDump);

//	GraphDump(&graph, FuncStrDump);

	ListInit(&list_edge);

	UnionFind(&graph, &list_edge);
	
	DumpList(&list_edge, &graph);
}

void FuncVertexDump(void *obj)
{
	struct Vertex *vertex = obj;

	FuncStrDump(vertex->obj);
	printf(" (d %d)", vertex->distance);
}

void PrimDijkstra(struct Heap *heap, struct Graph *graph, struct Hash *hash_vertex)
{
	struct Vertex *vertex = NULL;

	while ((vertex = HeapPop(heap)) != NULL)
	{
		struct ListNode *head = &vertex->hash_edge.list_nodes, *p = NULL;
/*
		printf("vertex");
		FuncVertexDump(vertex);
		printf("\n");
*/
		HashInsert(hash_vertex, vertex->obj, vertex);

		for (p = head->next; p != head; p = p->next)
		{
			struct HashNode *hash_node = p->container;
			struct Edge *edge = hash_node->val;

			struct Vertex *vertex_to = HashGet(&graph->hash_vertex, edge->obj);

			if (HashGet(hash_vertex, vertex_to->obj) != NULL)
				continue;
/*
			printf("vertex_to");
			FuncVertexDump(vertex_to);
			printf(" edge weight %d\n", edge->weight);
*/
			if (vertex_to->distance <= edge->weight)
				continue;

			vertex_to->distance = edge->weight;
			HeapNodeUp(vertex_to->heap_node, heap->func_obj_compare, heap->func_obj_update_heap_node);
		}
	}
}

void PrepareHeap(struct Heap *heap, struct Graph *graph)
{
	struct ListNode *head = &graph->hash_vertex.list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Vertex *vertex = hash_node->val;

		vertex->heap_node = HeapInsert(heap, vertex);
	}

//	HeapDump(heap, FuncVertexDump);
}

void PrimSetStart(struct Graph *graph)
{
	struct ListNode *head = &graph->hash_vertex.list_nodes;
	struct ListNode *first = head->next;

	struct HashNode *hash_node = first->container;
	struct Vertex *vertex = hash_node->val;

	vertex->distance = 0;
}

void DumpHashVertex(struct Hash *hash_vertex)
{
	struct ListNode *head = &hash_vertex->list_nodes, *p = NULL;

	printf("%s\n", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Vertex *vertex = hash_node->val;
		
		FuncVertexDump(vertex);
	}
	printf("\n");
}

void Prim(char ***equations, int *values, int num_equ_row)
{
	struct Graph graph;

	struct Hash hash_vertex;
	struct Heap heap;

	GraphInit(&graph, FuncStrToInt, FuncStrCompare);
	PrepareGraph(&graph, equations, num_equ_row, values, 1);

	PrimSetStart(&graph);

	HeapInit(&heap, kMinHeap, FuncVertexCompare);
	heap.func_obj_update_heap_node = FuncVertexUpdateHeapNode;

	PrepareHeap(&heap, &graph);

//	GraphDump(&graph, FuncStrDump);

	HashInit(&hash_vertex, FuncStrToInt, FuncStrCompare);

	PrimDijkstra(&heap, &graph, &hash_vertex);
	
	DumpHashVertex(&hash_vertex);
}

int main(int argc, char *argv[])
{
#define kNumEqu 14
	int values[kNumEqu] = {4, 8, 11, 7, 1, 6, 2, 8, 7, 4, 2, 14, 9, 10};

	char ***equations = calloc(kNumEqu, sizeof(char **));

	int i = 0;
	
	for (i = 0; i < kNumEqu; ++i)
	{
		equations[i] = calloc(2, sizeof(char *));

		equations[i][0] = calloc(10, sizeof(char));
		equations[i][1] = calloc(10, sizeof(char));
	}

	equations[0][0] = "a";
	equations[0][1] = "b";

	equations[1][0] = "a";
        equations[1][1] = "h";

	equations[2][0] = "b";
	equations[2][1] = "h";

	equations[3][0] = "h";
	equations[3][1] = "i";

	equations[4][0] = "h";
	equations[4][1] = "g";

	equations[5][0] = "i";
	equations[5][1] = "g";

	equations[6][0] = "i";
	equations[6][1] = "c";

	equations[7][0] = "b";
	equations[7][1] = "c";

	equations[8][0] = "c";
	equations[8][1] = "d";

	equations[9][0] = "c";
	equations[9][1] = "f";

	equations[10][0] = "g";
	equations[10][1] = "f";

	equations[11][0] = "d";
	equations[11][1] = "f";

	equations[12][0] = "d";
	equations[12][1] = "e";

	equations[13][0] = "e";
	equations[13][1] = "f";
	
	Kruskal(equations, values, kNumEqu);
	Prim(equations, values, kNumEqu);

	return 0;
}

