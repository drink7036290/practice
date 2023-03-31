
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

// gcc with ../lib/helpers.c

struct Pair
{
	int r;
	int c;
};

int PairIsValid(struct Pair *pair, int num_row, int num_col)
{
	if ((pair->r < 0) || (pair->r >= num_row))
		return 0;

	if ((pair->c < 0) || (pair->c >= num_col))
		return 0;

	return 1;
}

int PairBefore(struct Pair *from, struct Pair *to)
{
	return ((from->r <= to->r) && (from->c <= to->c));
}

void PairDump(struct Pair *pair)
{
	printf("r %d c %d\n", pair->r, pair->c);
}

void PairInit(struct Pair *pair, int r, int c)
{
	pair->r = r;
	pair->c = c;
}

struct Params
{
	int **matrix;
	int num_row;
	int num_col;
	int val;
};

void ParamsInit(struct Params *params, int **matrix, int num_row, int num_col, int val)
{
	params->matrix = matrix;
	params->num_row = num_row;
	params->num_col = num_col;
	params->val = val;
}

struct Pair* FindValHelper(struct Params *params, struct Pair *from, struct Pair *to);

struct Pair* PartitionFind(struct Params *params, struct Pair *from, struct Pair *to, struct Pair *pivot)
{
	struct Pair sub_from, sub_to, *result = NULL;

	PairInit(&sub_from, pivot->r, from->c);
	PairInit(&sub_to, to->r, pivot->c - 1);

	printf("parition 1\n");
	PairDump(&sub_from);
        PairDump(&sub_to);

	if ((result = FindValHelper(params, &sub_from, &sub_to)) != NULL)
		return result;

	PairInit(&sub_from, from->r, pivot->c);
	PairInit(&sub_to, pivot->r - 1, to->c);

	printf("parition 2\n");
        PairDump(&sub_from);
        PairDump(&sub_to);

	return FindValHelper(params, &sub_from, &sub_to);
}

struct Pair* FindValHelper(struct Params *params, struct Pair *from, struct Pair *to)
{
	int diff = 0;
	struct Pair sub_from, sub_to;

	if (! PairIsValid(from, params->num_row, params->num_col))
		return NULL;

	if (! PairIsValid(to, params->num_row, params->num_col))
		return NULL;

	if (params->matrix[from->r][from->c] > params->val)
		return NULL;

	if (params->matrix[to->r][to->c] < params->val)
		return NULL;

	PairInit(&sub_from, from->r, from->c);
	PairInit(&sub_to, to->r, to->c);

	// adjust to for rectangle
	diff = Min(to->r - from->r, to->c - from->c);
	PairInit(&sub_to, from->r + diff, from->c + diff);

	while (PairBefore(&sub_from, &sub_to))
	{
		struct Pair mid;
		PairInit(&mid, (sub_from.r + sub_to.r) >> 1, (sub_from.c + sub_to.c) >> 1);

		if (params->matrix[mid.r][mid.c] == params->val)
		{
			struct Pair *result = calloc(1, sizeof(struct Pair));
			PairInit(result, mid.r, mid.c);
			return result;
		}

		if (params->matrix[mid.r][mid.c] > params->val)
			PairInit(&sub_to, mid.r - 1, mid.c - 1);
		else
			PairInit(&sub_from, mid.r + 1, mid.c + 1);
	}

	// sub_from now larger than val
	return PartitionFind(params, from, to, &sub_from);
}

struct Pair* FindVal(int **matrix, int num_row, int num_col, int val)
{
	struct Params params;
	struct Pair from, to;

	ParamsInit(&params, matrix, num_row, num_col, val);
	PairInit(&from, 0, 0);
	PairInit(&to, num_row - 1, num_col - 1);

	return FindValHelper(&params, &from, &to);
}


int main(int argc, char *argv[])
{
	struct Pair *result = NULL;

//#define kNumRow 3
//#define kNumCol 3
//
//	int matrix_ref[kNumRow][kNumCol] = {
//		  {1, 2, 3}
//		, {4, 5, 6}
//		, {7, 8, 9}
//	};
//	int target = 4;	

//#define kNumRow 5
//#define kNumCol 5
//        
//	int matrix_ref[kNumRow][kNumCol] = {
//		  {15, 30,  50,  70,  73} 
//		, {35, 40, 100, 102, 120}
//		, {36, 42, 105, 110, 125}
//		, {46, 51, 106, 111, 130}
//		, {48, 55, 109, 140, 150}
//	};
//        int target = 42;

//#define kNumRow 5
//#define kNumCol 5
//        
//	int matrix_ref[kNumRow][kNumCol] = {
//		  { 1, 3, 5, 7, 9} 
//		, { 2, 4, 6, 8,10}
//		, {11,13,15,17,19}
//		, {12,14,16,18,20}
//		, {21,22,23,24,25}
//	};
//        int target = 13;

#define kNumRow 5
#define kNumCol 1
        
	int matrix_ref[kNumRow][kNumCol] = {
		  {15} 
		, {35}
		, {36}
		, {46}
		, {48}
	};
        int target = 48;

	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	for (i = 0; i < kNumRow; ++i)
	{
		matrix[i] = calloc(kNumCol, sizeof(int));
	
		for (j = 0; j < kNumCol; ++j)
			matrix[i][j] = matrix_ref[i][j];
	}

	if ((result = FindVal(matrix, kNumRow, kNumCol, target)) == NULL)
		printf("Cannot find %d\n", target);
	else
		PairDump(result);

	return 0;
}

