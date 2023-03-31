
#include "lib/trie.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void ConvertDictToTrie(char **dict, int dict_size, struct Trie *trie)
{
	int i = 0;

	for (i = 0; i < dict_size; ++i)
		TrieAddStr(trie, dict[i]);

	TrieDump(trie);
}

char* ReplaceWord(char *word, struct Trie *trie)
{
	char *ret_word = TrieGet(trie, word, TrieMatchFirstEnds);

	if (ret_word == NULL)
		return word;

	return ret_word;	
}

char* ReplaceWords(char *sentence, char **dict, int dict_size)
{
	struct Trie trie;
	int sentence_len = 0;
	char *word = NULL, *new_word = NULL;

	if ((sentence_len = strlen(sentence)) <= 0)
		return sentence;

	TrieInit(&trie);
	
	ConvertDictToTrie(dict, dict_size, &trie);

	new_word = calloc(sentence_len + 1, sizeof(char)); // \0

	word = strtok(sentence, " ");
	while (word != NULL)
	{
		strcat(new_word, ReplaceWord(word, &trie));
		strcat(new_word, " ");

		word = strtok(NULL, " ");
	}

	return new_word;
}

#define kInputSize 4

int main(int argc, char *argv[])
{
	char *dict[kInputSize] = {"catt", "cat", "ba" ,"rat"};
	char sentence[] = "the cattle was rattled by the battery"; // TRICKEY: char *sentence will cause strtok crash !

//	char *dict[kInputSize] = {"a", "b", "c"};
//	char sentence[] = "aadsfasf absbs bbab cadsfafs"; // TRICKEY: char *sentence will cause strtok crash !

	printf("[before] %s\n", sentence);
	printf("[after] %s\n", ReplaceWords(sentence, dict, kInputSize));

	return 0;
}

