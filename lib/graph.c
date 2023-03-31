
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

void GraphResetVerticesState(struct Graph *graph)
{
	struct ListNode *head = &graph->hash_vertex.list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
	        struct Vertex *vertex = hash_node->val;

		vertex->state = VertexStateNew;

		ListNodeDelete(&vertex->hook_DFS);
		vertex->edge_from = NULL;
	}
}

// return to's vertex if to != NULL ; else return NULL
struct Vertex* GraphDFS(struct Graph *graph, struct ListNode *list_vertex, void *from, void *to)
{
	struct ListNode *head = &graph->hash_vertex.list_nodes, *p = NULL;

	struct Vertex *vertex = NULL, *vertex_to = NULL;

	GraphResetVerticesState(graph);

	if (from != NULL)
	{
		if ((vertex = HashGet(&graph->hash_vertex, from)) == NULL)
			return NULL;

		return VertexDFS(vertex, &graph->hash_vertex, list_vertex, to);
	}
	
	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		vertex = hash_node->val;

		if ((vertex_to = VertexDFS(vertex, &graph->hash_vertex, list_vertex, to)) != NULL)
			return vertex_to;
	}

	return NULL;
}

void GraphDeleteEdge(struct Graph *graph, struct Edge *edge)
{
	ListNodeDelete(&edge->hook);

	VertexDeleteEdge(edge->vertex_from, edge);
}

struct Edge* GraphAddEdge(struct Graph *graph, void *from, void *to)
{
	struct Vertex *vertex_from = GraphAddVertex(graph, from);
	struct Edge *edge = NULL;

	GraphAddVertex(graph, to);
	edge = VertexAddEdge(vertex_from, to);

	ListAddTail(&graph->list_edge, &edge->hook);

	return edge;
}

struct Vertex* GraphAddVertex(struct Graph *graph, void *obj)
{
	struct Vertex *vertex = HashGet(&graph->hash_vertex, obj);

	if (vertex != NULL) // vertex already exists!
		return vertex;

	vertex = calloc(1, sizeof(struct Vertex));
	VertexInit(vertex, obj, graph->hash_vertex.func_obj_to_int, graph->hash_vertex.func_obj_compare);

	HashInsert(&graph->hash_vertex, obj, vertex);

	return vertex;
}

void GraphDump(struct Graph *graph, FuncObjDump func_obj_dump)
{
	struct ListNode *head = &graph->hash_vertex.list_nodes, *p = NULL;
        
        for (p = head->next; p != head; p = p->next)
        {
		struct HashNode *hash_node = p->container;
                struct Vertex *vertex = hash_node->val; 
                
		VertexDump(vertex, func_obj_dump);
        }
/*
	printf("Graph list_edge\n");
	head = &graph->list_edge;

	for (p = head->next; p != head; p = p->next)
        {
		struct Edge *edge = p->container;
                
		EdgeDump(edge, func_obj_dump);
        }

	printf("\n");
*/
}

void GraphInit(struct Graph *graph, FuncObjToInt func_obj_to_int, FuncObjCompare func_obj_compare)
{
	HashInit(&graph->hash_vertex, func_obj_to_int, func_obj_compare);

	ListInit(&graph->list_edge);
}
