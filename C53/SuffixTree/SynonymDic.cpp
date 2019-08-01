#include"SynonymDic.h"

struct TrieNode* getNode(void)
{
	struct TrieNode* pNode = new TrieNode;

	pNode->isEndOfWord = false;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = NULL;

	return pNode;
}

void insert(struct TrieNode* root, string key, string synonym[4])
{
	struct TrieNode* pCrawl = root;

	for (int i = 0; i < key.length(); i++)
	{
		int index = key[i] - 'a';
		if (pCrawl->children[index] == NULL)
			pCrawl->children[index] = getNode();

		pCrawl = pCrawl->children[index];
	}

	// mark last node as leaf 
	pCrawl->isEndOfWord = true;
	for (int i = 0; i < 4; i++)
		pCrawl->synonym[i] = synonym[i];
}

bool search(struct TrieNode* root, string key, string synonym[4])
{
	struct TrieNode* pCrawl = root;

	for (int i = 0; i < key.length(); i++)
	{
		int index = key[i] - 'a';
		if (!pCrawl->children[index])
			return false;

		pCrawl = pCrawl->children[index];
	}
	if (pCrawl != NULL && pCrawl->isEndOfWord) {
		for (int i = 0; i < 4; i++)
			synonym[i] = pCrawl->synonym[i];
		return true;
	}
	return false;
}

struct TrieNode* build_Dic() {
	TrieNode* root = getNode();
	ifstream fin;
	fin.open("synonym.txt");
	string synonym[4];
	string word;
	while (!fin.eof()) {
		getline(fin, word);
		for (int i = 0; i < 4; i++)
			getline(fin, synonym[i]);
		insert(root, word, synonym);
	}
	fin.close();
	return root;
}