
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib/helpers.h"

#include "lib/graph.h"

void LinkSets(struct Vertex *vertex1, struct Vertex *vertex1_org, struct Vertex *vertex2, struct Vertex *vertex2_org, double val_1to2)
{
	if (vertex1->rank > vertex2->rank)
	{
		vertex2->set = vertex1;
		vertex2_org->ratio_to_set = (1 / val_1to2) * vertex1_org->ratio_to_set;

		return;
	}

	vertex1->set = vertex2;
	vertex1_org->ratio_to_set = val_1to2 * vertex2_org->ratio_to_set;

	if (vertex1->rank == vertex2->rank)
		++vertex2->rank;
}

void UnionSets(struct Vertex *vertex1, struct Vertex *vertex2, double val_1to2)
{
	struct Vertex *set1 = VertexFindSet(vertex1);
	struct Vertex *set2 = VertexFindSet(vertex2);

	LinkSets(set1, vertex1, set2, vertex2, val_1to2);
}

void PrepareEdge(struct Graph *graph, char *from, char *to, double val)
{
	struct Edge *edge = GraphAddEdge(graph, from, to);
	edge->ratio = val;
}

void PrepareGraph(struct Graph *graph, char ***equations, int num_equ_row, double *values)
{
	int i = 0;

	for (i = 0; i < num_equ_row; ++i)
	{
		struct Vertex *vertex_from = GraphAddVertex(graph, equations[i][0]);
		struct Vertex *vertex_to = GraphAddVertex(graph, equations[i][1]);

		UnionSets(vertex_from, vertex_to, values[i]);

		PrepareEdge(graph, equations[i][0], equations[i][0], 1);

		PrepareEdge(graph, equations[i][0], equations[i][1], values[i]);

		PrepareEdge(graph, equations[i][1], equations[i][0], 1 / values[i]);

		PrepareEdge(graph, equations[i][1], equations[i][1], 1);
	}
}

double GetEdge(struct Vertex *vertex, struct Vertex *vertex_next)
{
	struct Edge *edge = HashGet(&vertex->hash_edge, vertex_next->obj);

	return edge->ratio;
}

void Calculate(struct Graph *graph, struct ListNode *list_path, double *result)
{
	struct ListNode *head = list_path, *p = NULL;
	double tmp_result = 1;

	for (p = head->next; p != head; p = p->next)
	{
		struct Vertex *vertex = p->container;
		struct Vertex *vertex_next = NULL;
		double edge_val = 0;

		if (p->next == head)
			break;

		vertex_next = p->next->container;

		printf("from");
		FuncStrDump(vertex->obj);
		printf(" to");
		FuncStrDump(vertex_next->obj);

		edge_val = GetEdge(vertex, vertex_next);
		printf(" val %lf\n", edge_val);
	
		tmp_result *= edge_val;
	}

	*result = tmp_result;
}

void DumpListPath(struct ListNode *list_path)
{
	struct ListNode *head = list_path, *p = NULL;

	printf("%s", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct Vertex *vertex = p->container;

		FuncStrDump(vertex->obj);
	}
	printf("\n");
}

void GetResult(struct Graph *graph, char **query, double *result)
{
/*
	struct ListNode list_path;
	struct Vertex *vertex_DFS = NULL;

	ListInit(&list_path);

	vertex_DFS = GraphDFS(graph, &list_path, query[0], query[1]);

	if (vertex_DFS == NULL) // no result
		return;

	DumpListPath(&list_path);
	Calculate(graph, &list_path, result);
*/
	struct Vertex *vertex_from = HashGet(&graph->hash_vertex, query[0]);
	struct Vertex *vertex_to = HashGet(&graph->hash_vertex, query[1]);

	if ((vertex_from == NULL) || (vertex_to == NULL))
		return;

	if (VertexFindSet(vertex_from) != VertexFindSet(vertex_to))
		return;

	*result = vertex_from->ratio_to_set / vertex_to->ratio_to_set;
}

void GetResults(struct Graph *graph, char ***queries, int num_query_row)
{
	double *results = calloc(num_query_row, sizeof(double));
	int i = 0;

	for (i = 0; i < num_query_row; ++i)
	{
		results[i] = -1;
		GetResult(graph, queries[i], &results[i]);
	}

	printf("result");
	for (i = 0; i < num_query_row; ++i)
		printf(" %lf", results[i]);
	printf("\n");
}

void Evaluate(char ***equations, int num_equ_row, int num_equ_col
		, double *values, int num_val
		, char ***queries, int num_query_row, int num_query_col)
{
	struct Graph graph;

	GraphInit(&graph, FuncStrToInt, FuncStrCompare);
	PrepareGraph(&graph, equations, num_equ_row, values);

	GraphDump(&graph, FuncStrDump);

	GetResults(&graph, queries, num_query_row);
}

int main(int argc, char *argv[])
{
/*
#define kNumEqu 2
#define kNumQuery 5
	double values[kNumEqu] = {2.0, 3.0};
*/

#define kNumEqu 3
#define kNumQuery 4
	double values[kNumEqu] = {1.5, 2.5, 5.0};

	char ***equations = calloc(kNumEqu, sizeof(char **));
	char ***queries = calloc(kNumQuery, sizeof(char **));

	int i = 0;
	
	for (i = 0; i < kNumEqu; ++i)
	{
		equations[i] = calloc(2, sizeof(char *));

		equations[i][0] = calloc(10, sizeof(char));
		equations[i][1] = calloc(10, sizeof(char));
	}

	for (i = 0; i < kNumQuery; ++i)
	{
		queries[i] = calloc(2, sizeof(char *));

		queries[i][0] = calloc(10, sizeof(char));
		queries[i][1] = calloc(10, sizeof(char));
	}
/*
	equations[0][0] = "a";
	equations[0][1] = "b";

	equations[1][0] = "b";
        equations[1][1] = "c";

	queries[0][0] = "a";
	queries[0][1] = "c";

	queries[1][0] = "b";
	queries[1][1] = "a";

	queries[2][0] = "a";
	queries[2][1] = "e";

	queries[3][0] = "a";
	queries[3][1] = "a";

	queries[4][0] = "x";
	queries[4][1] = "x";
*/

	equations[0][0] = "a";
	equations[0][1] = "b";

	equations[1][0] = "b";
        equations[1][1] = "c";

	equations[2][0] = "bc";
        equations[2][1] = "cd";

	queries[0][0] = "a";
	queries[0][1] = "c";

	queries[1][0] = "c";
	queries[1][1] = "b";

	queries[2][0] = "bc";
	queries[2][1] = "cd";

	queries[3][0] = "cd";
	queries[3][1] = "bc";

	Evaluate(equations, kNumEqu, 2, values, kNumEqu, queries, kNumQuery, 2);

	return 0;
}

