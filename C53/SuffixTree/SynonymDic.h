#ifndef SYNONYMDIC_H_
#define SYNONYMDIC_H_
#include"header.h"
#define ALPHABET_SIZE 26

struct TrieNode
{
	struct TrieNode* children[ALPHABET_SIZE];
	string synonym[4];
	// isEndOfWord is true if the node
	// represents end of a word
	bool isEndOfWord;
	~TrieNode() {
		delete[] children;
	}
};

struct TrieNode* getNode(void);
void insert(struct TrieNode* root, string key, string synonym[4]);
bool search(struct TrieNode* root, string key, string synonym[4]);
struct TrieNode* build_Dic();

#endif
