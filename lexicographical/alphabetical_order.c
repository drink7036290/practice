
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define DEBUG_ON

#define kWordsSize 6
	char *words[kWordsSize] = {"i", "love", "leetcode", "i", "love", "coding"};

void DumpWords(void)
{
	int i = 0;
	
	printf("=====\n");
	for (i = 0; i < kWordsSize; ++i)
		printf(" %s", words[i]);
	printf("\n=====\n");
}

struct CharInfo
{
	char c_left;
	char c;

	char *str;
};

void CharInfoDump(struct CharInfo *char_info)
{
	printf("c_left %c c %c str %s\n", char_info->c_left, char_info->c, char_info->str);
}

void CharInfoInit(struct CharInfo *char_info, char c_left, char c, char *str)
{
	char_info->c_left = c_left;
	char_info->c = c;

	char_info->str = str;
}

struct SortInfo
{
	struct CharInfo *char_infos;
	struct CharInfo *helper;

	char **strs;
	int num_str;
};

int SortInfoShift(struct SortInfo *sort_info, int shift)
{
	int i = 0, valid = 0;

	for (i = 0; i < sort_info->num_str; ++i)
	{
		int j = 0;
		char *pos = sort_info->strs[i], c_left = '\0';

		while (j < shift)
		{
			if (*pos == '\0')
			{
				c_left = '\0'; // [IMPORTANT]
				break;
			}

			c_left = *pos;

			++pos;
			++j;
		}

		CharInfoInit(&sort_info->char_infos[i], c_left, *pos, sort_info->strs[i]);

		if (*pos == '\0')
			continue;

		valid = 1;
	}

	return valid;
}

void SortInfoInit(struct SortInfo *sort_info, char **strs, int num_str)
{
	sort_info->char_infos = calloc(num_str, sizeof(struct CharInfo));
	sort_info->helper = calloc(num_str, sizeof(struct CharInfo));
	
	sort_info->strs = strs;
	sort_info->num_str = num_str;

	int i = 0;

	for (i = 0; i < num_str; ++i)
		CharInfoInit(&sort_info->char_infos[i], '\0', *strs[i], strs[i]); // init as first char
#ifdef DEBUG_ON
	for (i = 0; i < num_str; ++i)
		CharInfoDump(&sort_info->char_infos[i]);
#endif
}

void MergePick(struct SortInfo *sort_info, int *dst, int *src)
{
	struct CharInfo *char_infos = sort_info->char_infos;
        struct CharInfo *helper = sort_info->helper;
	
	char **strs = sort_info->strs;

	char_infos[*dst] = helper[*src];
	strs[*dst] = helper[*src].str;

	++*dst;
	++*src;

#ifdef DEBUG_ON
	DumpWords();
#endif
}

void Merge(struct SortInfo *sort_info, int start, int mid, int end)
{
	int i = 0, j = 0, k = 0;

	struct CharInfo *char_infos = sort_info->char_infos;
        struct CharInfo *helper = sort_info->helper;

	// copy to helper
	for (i = start; i <= end; ++i)
		helper[i] = char_infos[i];

	// assign back
	i = k = start;
	j = mid + 1;
#ifdef DEBUG_ON
	printf("i %d mid %d j %d end %d k %d\n", i, mid, j, end, k);
#endif
	while ((i <= mid) && (j <= end))
	{
		char c1 = helper[i].c, c2 = helper[j].c;
#ifdef DEBUG_ON
		printf("c1 %c(%d) c2 %c(%d)\n", c1, c1, c2, c2);
#endif
		if (helper[i].c_left != helper[j].c_left)
		{
			MergePick(sort_info, &k, &i);
			continue;
		}

		if (c1 <= c2)
			MergePick(sort_info, &k, &i);
		else
			MergePick(sort_info, &k, &j);
	}
#ifdef DEBUG_ON
	printf("i %d mid %d k %d\n", i, mid, k);
#endif
	while (i <= mid)
		MergePick(sort_info, &k, &i);
#ifdef DEBUG_ON
	printf("merge done\n");
#endif
}

void MergeSortHelper(struct SortInfo *sort_info, int start, int end)
{
	int mid = -1;
#ifdef DEBUG_ON
	printf("start %d(%s) end %d(%s)\n", start, sort_info->strs[start], end, sort_info->strs[end]);
#endif
	if (start >= end)
		return;

	mid = (start + end) / 2;

	MergeSortHelper(sort_info, start, mid);
	MergeSortHelper(sort_info, mid + 1, end);
	Merge(sort_info, start, mid, end);
}

void MergeSort(struct SortInfo *sort_info)
{
	MergeSortHelper(sort_info, 0, sort_info->num_str - 1);
}

void AlphabeticalSort(char **strs, int num_str)
{
	struct SortInfo sort_info;
	int shift = 0;

	SortInfoInit(&sort_info, strs, num_str);

	while (1)
	{
#ifdef DEBUG_ON
		int i = 0;

		printf("before shift\n");
		for (i = 0; i < num_str; ++i)
	                CharInfoDump(&sort_info.char_infos[i]);
#endif
		if (! SortInfoShift(&sort_info, shift))
			break;
#ifdef DEBUG_ON
		printf("after shift\n");
		for (i = 0; i < num_str; ++i)
	                CharInfoDump(&sort_info.char_infos[i]);

		printf("before sort\n");
#endif
		// stable
		MergeSort(&sort_info);

		++shift;
	}
}

int main(int argc, char *argv[])
{
	int max_val = 40;
	int i = max_val, len = 0;
	char **strs = calloc(max_val, sizeof(char*));

	while (i > 0)
	{
		i /= 10;
		++len;
	}

	for (i = 0; i < max_val; ++i)
	{
		strs[i] = calloc(len + 1, sizeof(char));
		sprintf(strs[i], "%d", i + 1);
	}

	for (i = 0; i < max_val; ++i)
		printf(" %s", strs[i]);
	printf("\n");

	AlphabeticalSort(strs, max_val);

	for (i = 0; i < max_val; ++i)
		printf(" %s", strs[i]);
	printf("\n");

// ====================================

	DumpWords();

	AlphabeticalSort(words, kWordsSize);

	DumpWords();



	return 0;
}

