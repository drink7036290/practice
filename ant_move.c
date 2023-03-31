
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "lib/helpers.h"

#include "lib/hash.h"

enum Direction
{
	  kDirRight = 0
	, kDirDown
	, kDirLeft
	, kDirUp
	, kDirCount
};

struct PosState
{
	int row;
	int col;
	enum Direction direction;

	int is_white;
};

void PosStateDump(struct PosState *pos_state)
{
	printf("(%d, %d) dir %d is_white %d\n", pos_state->row, pos_state->col, pos_state->direction, pos_state->is_white);
}

void PosStateInit(struct PosState *pos_state, int row, int col, enum Direction direction)
{
	pos_state->row = row;
	pos_state->col = col;
	pos_state->direction = direction;

	pos_state->is_white = (rand() % 2);
}

// 0 means equal
int FuncPosStateCompare(void *obj1, void *obj2)
{
	struct PosState *pos_state1 = obj1;
	struct PosState *pos_state2 = obj2;

	if (pos_state1->row != pos_state2->row)
		return 1;

	if (pos_state1->col != pos_state2->col)
		return 1;

	return 0;
}

int FuncPosStateToInt(void *obj)
{
	struct PosState *pos_state = obj;

	return pos_state->row + pos_state->col;
}

void DumpHash(struct Hash *hash)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	printf("%s\n", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		struct PosState *pos_state = hash_node->val;

		PosStateDump(pos_state);
	}
}

void PrintKMoves(int k)
{
	struct Hash hash_pos;
	struct PosState *start_pos = calloc(1, sizeof(struct PosState)), *last_pos = NULL;

	HashInit(&hash_pos, FuncPosStateToInt, FuncPosStateCompare);

	PosStateInit(start_pos, 0, 0, kDirRight);
	HashInsert(&hash_pos, start_pos, start_pos);

	srand(time(NULL));

	last_pos = start_pos;

	while (k > 0)
	{
		struct PosState tmp_pos;

		int new_dir = last_pos->direction;
		int new_row = last_pos->row;
		int new_col = last_pos->col;

		printf("last is_white %d\n", last_pos->is_white);
		if (last_pos->is_white)
		{
			if (++new_dir >= kDirCount)
				new_dir -= kDirCount;
		}
		else
		{
			if (--new_dir < 0)
				new_dir += kDirCount;
		}

		last_pos->is_white = ! last_pos->is_white;

		if (new_dir == kDirRight)
			++new_col;
		else if (new_dir == kDirDown)
			++new_row;
		else if (new_dir == kDirLeft)
			--new_col;
		else if (new_dir == kDirUp)
			--new_row;

		PosStateInit(&tmp_pos, new_row, new_col, new_dir);

		if ((last_pos = HashGet(&hash_pos, &tmp_pos)) == NULL)
		{
			last_pos = calloc(1, sizeof(struct PosState));

			PosStateInit(last_pos, new_row, new_col, new_dir);
			HashInsert(&hash_pos, last_pos, last_pos);

			printf("new\n");
		}
		else
		{
			printf("over write\n");
			PosStateInit(last_pos, new_row, new_col, new_dir);
		}

		PosStateDump(last_pos);

		--k;
	}

	DumpHash(&hash_pos);
}

int main(int argc, char *argv[])
{
	PrintKMoves(atoi(argv[1]));

	return 0;
}

