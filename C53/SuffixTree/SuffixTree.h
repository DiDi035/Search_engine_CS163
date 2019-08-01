#ifndef _SUFFIXTREE_
#define _SUFFIXTREE_
#define MAX_CHAR 95
#include"Header.h"

using namespace std;

typedef struct SuffixTreeNode {
	struct SuffixTreeNode* children[MAX_CHAR];
	struct SuffixTreeNode* suffixLink;
	int start;
	int* end;
	int suffixIndex;
	~SuffixTreeNode() {
		delete[] children;
	}
}Node;

struct SuffixTree {
	string fileName;
	Node* Root;
	char text[200000];
	Node* lastNewNode;
	Node* activeNode;

	int activeEdge;
	int activeLength;

	int remainingSuffixCount;
	int leafEnd;
	int* rootEnd;
	int* splitEnd;
	int size;
	SuffixTree();
	void buildTree(char* textt);
	bool search_linear(char Input[], vector<pair<int, int>>& position, int& frequency);
	// ```````````````````````````````````````````
};

/////////////////////////////////    Implement Tree     ////////////////////////////////////////

typedef struct SuffixTreeNode Node;

Node* newNode(int start, int* end, Node* root);

int edgeLength(Node* n, Node* root);

int walkDown(Node* currNode, int& activeEdge, Node*& activeNode, int& activeLength, Node* root);

void extendSuffixTree(int pos, Node* root, int& leafEnd, int& remainingSuffixCount,
	Node*& lastNewNode, int& activeLength, int& activeEdge, Node*& activeNode, int*& splitEnd, char text[]);

void print(int i, int j, char text[]);

void setSuffixIndexByDFS(Node* n, int labelHeight, Node* root, int& size);

void buildSuffixTree(Node*& root, int& leafEnd, int& remainingSuffixCount,
	Node*& lastNewNode, int& activeLength, int& activeEdge, Node*& activeNode,
	int*& splitEnd, int& size, char text[], int*& rootEnd);


int traverseEdge(char* str, int idx, int start, int end, char text[]);

int doTraversal(Node* n, char* str, int idx, Node* root,char text[], vector<pair<int, int>>& position, int& frequency);

int countLeaf(Node* n, vector<pair<int, int>>& position, char* str);

int do_traversal_to_leaf(Node* node, vector<pair<int, int>>& position, char* str);

bool checkForSubString(char* str, Node* root,char text[], vector<pair<int, int>>& position, int& frequency);

int get_file_name(SuffixTree*& tree);

///////////////////////////////   SearchMachine    ///////////////////////////////////////////

void doSth();

vector<int> search_normal(SuffixTree*& tree, int numOfFile, char input[1000][1000], int& numOfSearchFile, int numOfWord);
vector<int> filetype_operation(SuffixTree*& tree, int numOfFile, char arrWord[1000][1000], int& numOfSearchFile, int numOfWord);
vector<int> range_of_number(SuffixTree*& tree, int numOfFile, char arrWord[1000][1000], int& numOfSearchFile);
vector<int> Intitle_Search(SuffixTree*& tree, int numOfFile, char input[1000][1000], int& numOfSearchFile, int numOfWord);
void search_asterisk(SuffixTree*& tree, string str, int numOfFile);
int check_aLL(char n[]);
bool Check_And(char n[]);
bool Check_Or(char n[]);
bool Check_Minus(char n[]);
bool Check_Intitle(char n[]);
bool Check_Plus(char n[]);
bool Check_Filetype(char n[]); 
bool Check_Quote(char n[]);
bool Check_Asterisk(char n[]);
bool Check_In_Range(char n[]);
bool Check_Synonym(char n[]);
bool Check_$(char n[]);

#endif // !_SUFFIXTREE_

