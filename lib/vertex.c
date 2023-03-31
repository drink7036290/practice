
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "vertex.h"

struct Vertex* VertexFindSet(struct Vertex *vertex)
{
	if (vertex != vertex->set)
		vertex->set = VertexFindSet(vertex->set);

	return vertex->set;
}

struct Vertex* VertexDFS(struct Vertex *vertex, struct Hash *hash_vertex, struct ListNode *list_vertex, void *to)
{
	struct ListNode *head = &vertex->hash_edge.list_nodes, *p = NULL;
	struct Vertex *vertex_to = NULL;

	if (vertex->state == VertexStateDone)
		return NULL;

	if (vertex->state == VertexStateCurr) // loop
	{
//		printf("loop\n");
		return NULL;
	}

	vertex->state = VertexStateCurr;

	if (to != NULL)
	{
		if (! hash_vertex->func_obj_compare(vertex->obj, to)) // equal
		{
			vertex->state = VertexStateDone;
			ListAddFront(list_vertex, &vertex->hook_DFS);

			return vertex;
		}
	}

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct Edge *edge = hash_node->val;

		struct Vertex *next_vertex = HashGet(hash_vertex, edge->obj);

		if ((vertex_to = VertexDFS(next_vertex, hash_vertex, list_vertex, to)) != NULL)
			break;
	}

	vertex->state = VertexStateDone;
	ListAddFront(list_vertex, &vertex->hook_DFS);

	return vertex_to;
}

void VertexDeleteEdge(struct Vertex *vertex, struct Edge *edge)
{
	HashDelete(&vertex->hash_edge, edge->obj);

	free(edge);
}

struct Edge* VertexAddEdge(struct Vertex *vertex, void *obj)
{
	struct Edge *edge = HashGet(&vertex->hash_edge, obj);

	if (edge != NULL)
		return edge;

	edge = calloc(1, sizeof(struct Edge));
	EdgeInit(edge, obj, vertex);

	HashInsert(&vertex->hash_edge, obj, edge);

	return edge;
}

void FuncVertexUpdateHeapNode(void *obj, struct HeapNode *heap_node)
{
	struct Vertex *vertex = obj;

	vertex->heap_node = heap_node;
}

int FuncVertexCompare(void *obj1, void *obj2)
{
	struct Vertex *vertex1 = obj1;
	struct Vertex *vertex2 = obj2;

	return FuncIntCompare(&vertex1->distance, &vertex2->distance);
}

void VertexDump(struct Vertex *vertex, FuncObjDump func_obj_dump)
{
	struct ListNode *head = &vertex->hash_edge.list_nodes, *p = NULL;
	struct Vertex *set = NULL;	

	func_obj_dump(vertex->obj);
//	printf(" state %d", vertex->state);
//	printf(" distance %d", vertex->distance);

	printf(" set");
	set = VertexFindSet(vertex);
	if (set == NULL)
		printf(" NULL");
	else
		func_obj_dump(set->obj);

	printf(" rank %d ratio_to_set %.1lf", vertex->rank, vertex->ratio_to_set);

	printf("\n   edge");

	for (p = head->next; p != head; p = p->next)
        {
		struct HashNode *hash_node = p->container;
                struct Edge *edge = hash_node->val;
                
		EdgeDump(edge, func_obj_dump);
        }

	printf("\n");

}

void VertexInit(struct Vertex *vertex, void *obj, FuncObjToInt func_obj_to_int, FuncObjCompare func_obj_compare)
{
	vertex->obj = obj;
	HashInit(&vertex->hash_edge, func_obj_to_int, func_obj_compare);

	vertex->distance = INT_MAX;

	vertex->set = vertex;
	vertex->rank = 0;
	vertex->ratio_to_set = 1;

	vertex->state = VertexStateNew;
	ListNodeInit(&vertex->hook_DFS, vertex);

	vertex->edge_from = NULL;
	vertex->vertex_from = NULL;
}
