
#include <stdio.h>
#include <string.h>
#include "edge.h"

void EdgeDump(struct Edge *edge, FuncObjDump func_obj_dump)
{
	func_obj_dump(edge->obj);
	printf("(%d ratio %.1lf)", edge->weight, edge->ratio);
}

int FuncEdgeCompare(void *obj1, void *obj2)
{
	struct Edge *edge1 = obj1;
	struct Edge *edge2 = obj2;

	return FuncIntCompare(&edge1->weight, &edge2->weight);
}

void EdgeInit(struct Edge *edge, void *obj, void *vertex_from)
{
	edge->obj = obj;

	edge->weight = 0;

	edge->ratio = 0;

	ListNodeInit(&edge->hook_flow, edge);

	edge->vertex_from = vertex_from;

	ListNodeInit(&edge->hook, edge);
}
