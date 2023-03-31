
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

int ValueBetween(double left, double mid, double right)
{
	if (left <= right)
		return ((left <= mid) && (mid <= right));

	return ((right <= mid) && (mid <= left));
}

struct Point
{
	double x;
	double y;
};

void PointDump(struct Point *point)
{
	printf("(%.2lf, %.2lf)\n", point->x, point->y);
}

int PointBetween(struct Point *left, struct Point *mid, struct Point *right)
{
	if (! ValueBetween(left->x, mid->x, right->x))
		return 0;

	if (! ValueBetween(left->y, mid->y, right->y))
		return 0;

	return 1;
}

void PointInit(struct Point *point, double x, double y)
{
	point->x = x;
	point->y = y;
}

struct Line
{
	struct Point *p1;
	struct Point *p2;

	double slope;
	double y_intercept;
};

void LineDump(struct Line *line)
{
	PointDump(line->p1);
	PointDump(line->p2);

	printf("slope %.2lf y_intercept %.2lf\n", line->slope, line->y_intercept);
}

struct Point* LineIntersects(struct Line *line1, struct Line *line2)
{
	if (PointBetween(line1->p1, line2->p1, line1->p2))
		return line2->p1;

	if (PointBetween(line1->p1, line2->p2, line1->p2))
		return line2->p2;

	return NULL;
}

void LineInit(struct Line *line, struct Point *p1, struct Point *p2)
{
	double x_diff = p2->x - p1->x;

	line->p1 = p1;
	line->p2 = p2;

	if (x_diff == 0)
	{
		line->slope = INT_MAX;
		line->y_intercept = INT_MAX; // not exists, unused
		return;
	}

	line->slope = (p2->y - p1->y) / x_diff;
	line->y_intercept = p1->y - line->slope * p1->x;
}

void GetIntersectionHelper(struct Line *line1, struct Line *line2, struct Point *point)
{
	// diff slope here

	if (line1->slope == INT_MAX)
	{
		point->x = line1->p1->x;
		point->y = line2->slope * point->x + line2->y_intercept;

		return;
	}

	if (line2->slope == INT_MAX)
	{
		point->x = line2->p1->x;
		point->y = line1->slope * point->x + line1->y_intercept;

		return;
	}

	point->x = (line2->y_intercept - line1->y_intercept) / (line1->slope - line2->slope);
	point->y = line1->slope * point->x + line1->y_intercept;
}

struct Point* GetIntersection(struct Line *line1, struct Line *line2)
{
	struct Point point_tmp, *point_result = NULL;

	// same slope 
	if (line1->slope == line2->slope)
	{
		if (line1->y_intercept == line2->y_intercept)
			return LineIntersects(line1, line2);

		return NULL;
	}

	// diff slope
	GetIntersectionHelper(line1, line2, &point_tmp);

	if (! PointBetween(line1->p1, &point_tmp, line1->p2))
		return NULL;

	if (! PointBetween(line2->p1, &point_tmp, line2->p2))
		return NULL;

	point_result = calloc(1, sizeof(struct Point));
	PointInit(point_result, point_tmp.x, point_tmp.y);

	return point_result;
}

int main(int argc, char *argv[])
{
	struct Point A1, A2, B1, B2, *point = NULL;
	struct Line L1, L2;
	int range = 5;

	srand(time(NULL));

	PointInit(&A1, rand() % range, rand() % range);
	PointInit(&A2, rand() % range, rand() % range);

	PointInit(&B1, rand() % range, rand() % range);
	PointInit(&B2, rand() % range, rand() % range);

	LineInit(&L1, &A1, &A2);
	LineInit(&L2, &B1, &B2);

	LineDump(&L1);
	LineDump(&L2);

	if ((point = GetIntersection(&L1, &L2)) != NULL)
		PointDump(point);
	else
		printf("no intersection\n");

	return 0;
}

