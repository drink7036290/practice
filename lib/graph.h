
#ifndef GRAPH_H_
#define GRAPH_H_

#include "vertex.h"

struct Graph
{
	struct Hash hash_vertex;

	struct ListNode list_edge;
};

void GraphResetVerticesState(struct Graph *graph);

struct Vertex* GraphDFS(struct Graph *graph, struct ListNode *list_vertex, void *from, void *to);

void GraphDeleteEdge(struct Graph *graph, struct Edge *edge);
struct Edge* GraphAddEdge(struct Graph *graph, void *from, void *to);

struct Vertex* GraphAddVertex(struct Graph *graph, void *val);

void GraphDump(struct Graph *graph, FuncObjDump func_obj_dump);

void GraphInit(struct Graph *graph, FuncObjToInt func_obj_to_int, FuncObjCompare func_obj_compare);

#endif
