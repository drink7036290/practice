
#ifndef VERTEX_H_
#define VERTEX_H_

#include "edge.h"
#include "hash.h"
#include "heap.h"

enum VertexState
{
	  VertexStateNew = 0
	, VertexStateCurr
	, VertexStateDone
};

struct Vertex
{
	void *obj;
	struct Hash hash_edge;

	// Dijkstra
	int distance;
	struct HeapNode *heap_node;

	// UnionFind
	struct Vertex *set;
	int rank;
	double ratio_to_set; // evaluate division

	// DFS/BFS
	enum VertexState state;
	struct ListNode hook_DFS;

	// BFS
	struct Edge *edge_from;
	struct Vertex *vertex_from;
};

struct Vertex* VertexFindSet(struct Vertex *vertex);

struct Vertex* VertexDFS(struct Vertex *vertex, struct Hash *hash_vertex, struct ListNode *stack, void *to);

void VertexDeleteEdge(struct Vertex *vertex, struct Edge *edge);
struct Edge* VertexAddEdge(struct Vertex *vertex, void *obj);

void FuncVertexUpdateHeapNode(void *obj, struct HeapNode *heap_node);
int FuncVertexCompare(void *obj1, void *obj2);

void VertexDump(struct Vertex *vertex, FuncObjDump func_obj_dump);

void VertexInit(struct Vertex *vertex, void *obj, FuncObjToInt func_obj_to_int, FuncObjCompare func_obj_compare);

#endif
