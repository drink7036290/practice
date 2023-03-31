
#ifndef EDGE_H_
#define EDGE_H_

#include "helpers.h"
#include "list_node.h"

struct Edge
{
	void *obj;

	int weight;

	double ratio; // evaluate division

	// max flow
	struct ListNode hook_flow;

	void *vertex_from;

	struct ListNode hook;
};

void EdgeDump(struct Edge *edge, FuncObjDump func_obj_dump);

int FuncEdgeCompare(void *obj1, void *obj2);

void EdgeInit(struct Edge *edge, void *obj, void *vertex_from);

#endif
