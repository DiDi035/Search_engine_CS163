#include"Header.h"
#include"SuffixTree.h"
#include"Graphics.h"

void doSth() {
	ofstream fout;
	fout.open("test.txt");
	for (int i = 51; i <= 100; i++)
		fout << "Group18News" << i << endl;
	fout.close();
}

Node* newNode(int start, int* end, Node* root) {
	Node* node = new Node;
	int i;
	for (i = 0; i < MAX_CHAR; i++)
		node->children[i] = NULL;

	node->suffixLink = root;
	node->start = start;
	node->end = end;

	node->suffixIndex = -1;
	return node;
}

int edgeLength(Node* n, Node* root) {
	if (n == root)
		return 0;
	return *(n->end) - (n->start) + 1;
}

int walkDown(Node* currNode, int& activeEdge, Node*& activeNode, int& activeLength, Node* root) {
	if (activeLength >= edgeLength(currNode, root)) {
		activeEdge += edgeLength(currNode, root);
		activeLength -= edgeLength(currNode, root);
		activeNode = currNode;
		return 1;
	}
	return 0;
}

void extendSuffixTree(int pos, Node* root, int& leafEnd, int& remainingSuffixCount,
	Node*& lastNewNode, int& activeLength, int& activeEdge, Node*& activeNode, int*& splitEnd, char text[])
{
	leafEnd = pos;
	remainingSuffixCount++;
	lastNewNode = NULL;

	while (remainingSuffixCount > 0) {

		if (activeLength == 0)
			activeEdge = pos;

		if (activeNode->children[text[activeEdge] - 32] == NULL) {
			activeNode->children[text[activeEdge] - 32] = newNode(pos, &leafEnd, root);

			if (lastNewNode != NULL) {
				lastNewNode->suffixLink = activeNode;
				lastNewNode = NULL;
			}
		}
		else {
			Node* next = activeNode->children[text[activeEdge] - 32];
			if (walkDown(next, activeEdge, activeNode, activeLength, root)) {
				continue;
			}
			if (text[next->start + activeLength] == text[pos]) {
				if (lastNewNode != NULL && activeNode != root) {
					lastNewNode->suffixLink = activeNode;
					lastNewNode = NULL;
				}

				activeLength++;
				break;
			}

			splitEnd = new int;
			*splitEnd = next->start + activeLength - 1;

			Node* split = newNode(next->start, splitEnd, root);
			activeNode->children[text[activeEdge] - 32] = split;

			split->children[text[pos] - 32] = newNode(pos, &leafEnd, root);
			next->start += activeLength;
			split->children[text[next->start] - 32] = next;

			if (lastNewNode != NULL) {
				lastNewNode->suffixLink = split;
			}

			lastNewNode = split;
		}

		remainingSuffixCount--;
		if (activeNode == root && activeLength > 0) {
			activeLength--;
			activeEdge = pos - remainingSuffixCount + 1;
		}
		else if (activeNode != root) {
			activeNode = activeNode->suffixLink;
		}
	}
}

void print(int i, int j, char text[]) {
	int k;
	for (k = i; k <= j; k++)
		printf("%c", text[k]);
}

void setSuffixIndexByDFS(Node* n, int labelHeight, Node* root, int& size) {
	if (n == NULL)  return;

	if (n->start != -1) {
	}
	int leaf = 1;
	for (int i = 0; i < MAX_CHAR; i++) {
		if (n->children[i] != NULL) {

			leaf = 0;
			setSuffixIndexByDFS(n->children[i], labelHeight +
				edgeLength(n->children[i], root), root, size);
		}
	}
	if (leaf == 1)
		n->suffixIndex = size - labelHeight;
}

void buildSuffixTree(Node*& root, int& leafEnd, int& remainingSuffixCount,
	Node*& lastNewNode, int& activeLength, int& activeEdge, Node*& activeNode,
	int*& splitEnd, int& size, char text[], int*& rootEnd)
{
	size = strlen(text);
	int i;
	rootEnd = new int;
	*rootEnd = -1;

	root = newNode(-1, rootEnd, NULL);

	activeNode = root;
	for (i = 0; i < size - 1; i++)
		extendSuffixTree(i, root, leafEnd, remainingSuffixCount,
			lastNewNode, activeLength, activeEdge, activeNode, splitEnd, text);
	int labelHeight = 0;
	setSuffixIndexByDFS(root, labelHeight, root, size);
}

SuffixTree::SuffixTree() {
	Root = NULL;
	lastNewNode = NULL;
	activeNode = NULL;

	activeEdge = -1;
	activeLength = 0;

	remainingSuffixCount = 0;
	leafEnd = -1;
	rootEnd = NULL;
	splitEnd = NULL;
	size = -1;
}

void SuffixTree::buildTree(char* textt) {
	strcpy(text, textt);
	buildSuffixTree(Root, leafEnd, remainingSuffixCount, lastNewNode, activeLength, activeEdge,
		activeNode, splitEnd, size, text, rootEnd);
}


int traverseEdge(char* str, int idx, int start, int end, char text[]) {
	int k = 0;
	for (k = start; k <= end && str[idx] != '\0'; k++, idx++)
	{
		if (text[k] != str[idx])
			return -1;
	}
	if (str[idx] == '\0')
		return 1;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int doTraversal(Node* n, char* str, int idx, Node* root, char text[], vector<pair<int, int>>& position, int& frequency) {
	if (n == nullptr)
		return -1;
	int res = -1;
	if (n->start != -1) {
		res = traverseEdge(str, idx, n->start, *(n->end), text);
		if (res == -1)
			return -1;
		if (res == 1) {
			if (n->suffixIndex > -1) {
				frequency += 1;
				position.push_back(make_pair(n->suffixIndex - 1, strlen(str)));
			}
			else {
				int count = countLeaf(n, position, str);
				frequency += count;
			}
			return 1;
		}
	}

	idx = idx + edgeLength(n, root);
	if (n->children[str[idx] - 32] != nullptr)
		return doTraversal(n->children[str[idx] - 32], str, idx, root, text, position, frequency);
	else
		return -1;
}

int do_traversal_to_leaf(Node* node, vector<pair<int, int>>& position, char* str) {
	if (node == NULL)
		return 0;
	if (node->suffixIndex > -1) {
		position.push_back(make_pair(node->suffixIndex - 1, strlen(str)));
		return 1;
	}
	int count = 0;
	for (int i = 0; i < MAX_CHAR; i++) {
		if (node->children[i] != nullptr)
			count += do_traversal_to_leaf(node->children[i], position, str);
	}
	return count;
}

int countLeaf(Node* n, vector<pair<int, int>>& position, char* str) {
	if (n == NULL)
		return 0;
	return do_traversal_to_leaf(n, position, str);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool checkForSubString(char* str, Node* root, char text[], vector<pair<int, int>>& position, int& frequency) {
	int res = doTraversal(root, str, 0, root, text, position, frequency);
	if (res == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool SuffixTree::search_linear(char Input[], vector<pair<int, int>> &position, int &frequency) {
	return checkForSubString(Input, Root, text, position, frequency);
}

vector<int> search_normal(SuffixTree*& tree, int numOfFile, char input[1000][1000], int& numOfSearchFile, int numOfWord) {
	vector<int> allFile;
	set<pair<int, pair<int, vector<pair<int, int>>>>> topFiveSearch;
	// <frequence, <file i-th, position<index text, length>>>
	numOfSearchFile = 0;
	vector<pair<int, int>> position;
	int frequency = 0;
	clock_t begin = clock();
	for (int i = 0; i < numOfFile; i++) {
		bool check = false;
		for (int j = 0; j < numOfWord; j++) {
			bool null = tree[i].search_linear(input[j], position, frequency);
			if (null) {
				if (!check) {
					check = true;
				}
			}
		}
		if (check) {
			sort(position.begin(), position.end());
			topFiveSearch.insert(mp(frequency, mp(i, position)));
			allFile.push_back(i);
			numOfSearchFile++;
		}
		frequency = 0;
		position.clear();
	}
	clock_t end = clock();
	cout << "Time run: " << (float)(end - begin) / CLOCKS_PER_SEC << " s" << endl;
	int top5kithu = 5;
	string sentence;
	sentence.resize(170, '~');
	set<pair<int, pair<int, vector<pair<int, int>>>>>::reverse_iterator it = topFiveSearch.rbegin();
	for (; it != topFiveSearch.rend() && top5kithu > 0; ++it) {
		int j = 0;
		int i = (*it).ss.ff;
		vector<pair<int, int>> indexing = (*it).ss.ss;
		--top5kithu;
		//write txt and indexing
		textcolor(23);
		cout << tree[i].fileName << endl;
		textcolor(15);
		for (int k = 0; k < strlen(tree[i].text); ++k) {
			if (j < indexing.size()) {
				if (k == indexing[j].ff)                    textcolor(100);
				if (k == indexing[j].ss + indexing[j].ff) { textcolor(15); ++j; }
			}
			if (tree[i].text[k] == '^') cout << endl;
			else cout << tree[i].text[k];
		}
		cout << endl << sentence << endl;
	}
	topFiveSearch.clear();

	return allFile;
}

int get_file_name(SuffixTree*& tree) {
	ifstream fin1, fin2;
	fin1.open("filename.txt");
	if (fin1.is_open())
		cout << "Opened file" << endl;
	int numOfFile;
	fin1 >> numOfFile;
	tree = new SuffixTree[numOfFile];
	string file;
	char* textt;
	string tmp;
	int countChar, countFile = 0;
	for (int i = 0; i < numOfFile; i++) {
		textt = new char[30000];
		fin1 >> file;
		fin2.open(file);
		countChar = 0;
		while (!fin2.eof()) {
			getline(fin2, tmp);
			for (int i = 0; i < tmp.size(); i++) {
				if (tmp[i] >= 32 && tmp[i] <= 126) {
					textt[countChar] = tmp[i];
					countChar++;
				}
				else {
					textt[countChar] = '?';
					countChar++;
				}
			}
			textt[countChar] = '^';
			countChar++;
		}
		textt[countChar] = '\0';
		countChar++;
		fin2.close();
		tree[i].fileName = file;
		tree[i].buildTree(textt);
		cout << "Build successfully: " << tree[i].fileName << endl;
		delete[] textt;
	}
	fin1.close();
	return numOfFile;
}

vector<int> filetype_operation(SuffixTree*& tree, int numOfFile, char arrWord[1000][1000], int& numOfSearchFile, int numOfWord) {
	vector<int> allFile;
	set<pair<int, pair<int, vector<pair<int, int>>>>> topFiveSearch;
	// <frequence, <file i-th, position<index text, length>>>
	numOfSearchFile = 0;
	vector<pair<int, int>> position;
	int frequency = 0;
	clock_t begin = clock();
	for (int i = 0; i < numOfFile; i++) {
		bool check = true;
		for (int j = 0; j < numOfWord; j++) {
			bool null = tree[i].search_linear(arrWord[j], position, frequency);
			if (!null) {
				check = false;
				break;
			}
		}
		if (check) {
			sort(position.begin(), position.end());
			topFiveSearch.insert(mp(frequency, mp(i, position)));
			allFile.push_back(i);
			//numOfSearchFile++;
		}
		position.clear();
		frequency = 0;
	}
	clock_t end = clock();
	cout << "Time run: " << (float)(end - begin) / CLOCKS_PER_SEC << " s" << endl;
	int top5kithu = 5;
	string sentence;
	sentence.resize(170, '~');
	set<pair<int, pair<int, vector<pair<int, int>>>>>::reverse_iterator it = topFiveSearch.rbegin();
	for (; it != topFiveSearch.rend() && top5kithu > 0; ++it) {
		int j = 0;
		int i = (*it).ss.ff;
		vector<pair<int, int>> indexing = (*it).ss.ss;
		--top5kithu;
		//write txt and indexing
		textcolor(23);
		cout << tree[i].fileName << endl;
		textcolor(15);
		for (int k = 0; k < strlen(tree[i].text); ++k) {
			if (j < indexing.size()) {
				if (k == indexing[j].ff)                    textcolor(100);
				if (k == indexing[j].ss + indexing[j].ff) { textcolor(15); ++j; }
			}
			if (tree[i].text[k] == '^') cout << endl;
			else cout << tree[i].text[k];
		}
		cout << endl << sentence << endl;
	}
	topFiveSearch.clear();

	return allFile;
}

vector<int> range_of_number(SuffixTree*& tree, int numOfFile, char arrWord[1000][1000], int& numOfSearchFile) {
	set<pair<int, pair<int, vector<pair<int, int>>>>> topFiveSearch;
	numOfSearchFile = 0;
	char input[1000];
	int size1 = strlen(arrWord[0]), size2 = strlen(arrWord[1]);
	strcpy(input, arrWord[0]);
	int num1 = 0, num2 = 0, tmp;
	int i;
	vector<pair<int, int>> position;
	clock_t begin = clock();
	for (i = 1; arrWord[1][i] != '.'; i++) {
		tmp = arrWord[1][i] - '0';
		num1 = num1 * 10 + tmp;
	}
	for (i = i + 3; i < size2; i++) {
		tmp = arrWord[1][i] - '0';
		num2 = num2 * 10 + tmp;
	}
	vector<int> result;
	vector<int> temp;
	char convertNumToChar[1000];
	for (int j = num1; j <= num2; j++) {
		_itoa_s(j, convertNumToChar, 10);
		int k;
		tmp = 0;
		arrWord[1][0] = '$';
		for (k = 1; k - 1 < strlen(convertNumToChar); k++) {
			arrWord[1][k] = convertNumToChar[tmp];
			tmp++;
		}
		arrWord[1][k] = ' ';
		arrWord[1][k + 1] = '\0';
		tmp = 0;
		int frequency = 0;
		for (int i = 0; i < numOfFile; i++) {
			bool check = true;
			for (int j = 0; j < 2; j++) {
				bool null = tree[i].search_linear(arrWord[j], position, frequency);
				if (!null) {
					check = false;
					break;
				}
			}
			if (check) {
				for (int p = 0; p < temp.size(); p++) {
					if (i == temp[p]) {
						check = false;
						break;
					}
				}
				if (check) {
					temp.push_back(i);
					sort(position.begin(), position.end());
					topFiveSearch.insert(mp(frequency, mp(i, position)));
					//allFile.push_back(i);
					//numOfSearchFile++;
				}
			}
			position.clear();
			frequency = 0;
		}
	}
	clock_t end = clock();
	cout << "Time run: " << (float)(end - begin) / CLOCKS_PER_SEC << " s" << endl;
	int top5kithu = 5;
	string sentence;
	sentence.resize(170, '~');
	set<pair<int, pair<int, vector<pair<int, int>>>>>::reverse_iterator it = topFiveSearch.rbegin();
	for (; it != topFiveSearch.rend() && top5kithu > 0; ++it) {
		int j = 0;
		int o = (*it).ss.ff;
		vector<pair<int, int>> indexing = (*it).ss.ss;
		--top5kithu;
		//write txt and indexing
		textcolor(23);
		cout << tree[o].fileName << endl;
		textcolor(15);
		for (int k = 0; k < strlen(tree[o].text); ++k) {
			if (j < indexing.size()) {
				if (k == indexing[j].ff)                    textcolor(100);
				if (k == indexing[j].ss + indexing[j].ff) { textcolor(15); ++j; }
			}
			if (tree[o].text[k] == '^') cout << endl;
			else cout << tree[o].text[k];
		}
		cout << endl << sentence << endl;
	}
	topFiveSearch.clear();
	return result;
}

vector<int> Intitle_Search(SuffixTree*& tree, int numOfFile, char input[1000][1000], int& numOfSearchFile, int numOfWord)
{
	vector<int> allFile;
	set<pair<int, pair<int, vector<pair<int, int>>>>> topFiveSearch;
	// <frequence, <i, position <coordinate, length> >
	numOfSearchFile = 0;
	vector<pair<int, int>> position;
	int frequency = 0;
	clock_t begin = clock();
	for (int i = 0; i < numOfFile; i++) {
		ifstream fileText;
		fileText.open(tree[i].fileName);
		string strTitle;
		getline(fileText, strTitle);
		for (int j = 0; j < numOfWord; j++) {
			for (int k = 0; k < strTitle.size(); k++) {
				if (strTitle[k] == input[j][0]) {
					bool kiemtra = true;
					for (int l = 0; l < strlen(input[j]); l++) {
						if (strTitle[k + l] != input[j][l]) {
							kiemtra = false;
							break;
						}
					}
					if (kiemtra) {
						position.push_back(make_pair(k, strlen(input[j])));
						frequency++;
					}
				}
			}
		}
		if (frequency > 0) {
			sort(position.begin(), position.end());
			topFiveSearch.insert(mp(frequency, mp(i, position)));
		}
		frequency = 0;
		position.clear();
	}
	clock_t end = clock();
	cout << "Time run: " << (float)(end - begin) / CLOCKS_PER_SEC << " s" << endl;
	int top5kithu = 5;
	string sentence;
	sentence.resize(170, '~');
	set<pair<int, pair<int, vector<pair<int, int>>>>>::reverse_iterator it = topFiveSearch.rbegin();
	for (; it != topFiveSearch.rend() && top5kithu > 0; ++it) {
		int j = 0;
		int i = (*it).ss.ff;
		vector<pair<int, int>> indexing = (*it).ss.ss;
		--top5kithu;
		//write txt and indexing
		textcolor(23);
		cout << tree[i].fileName << endl;
		textcolor(15);
		for (int k = 0; k < strlen(tree[i].text); ++k) {
			if (j < indexing.size()) {
				if (k == indexing[j].ff)                    textcolor(100);
				if (k == indexing[j].ss + indexing[j].ff) { textcolor(15); ++j; }
			}
			if (tree[i].text[k] == '^') cout << endl;
			else cout << tree[i].text[k];
		}
		cout << endl << sentence << endl;
	}
	topFiveSearch.clear();

	return allFile;
}

void search_asterisk(SuffixTree*& tree, string str, int numOfFile) {
	set<pair<int, pair<int, vector<pair<int, int>>>>> topFiveSearch;
	char a[100];
	char b[100];
	bool check = false;
	int i = 0; //find '*'
	int j = 0;
	int l = 0;
	clock_t begin = clock();
	for (; i < str.size(); i++) {
		if (str[i] == '*') {
			check = true;
			i++;
			continue;
		}
		if (!check) {
			a[l] = str[i];
			l++;
		}
		else {
			b[j] = str[i];
			j++;
		}
	}
	l--;
	a[l] = NULL;
	b[j] = NULL;

	for (int indx = 0; indx < numOfFile; indx++) {
		int frequency = 0;
		vector<pair<int, int>> aPosition;
		vector<pair<int, int>> bPosition;
		vector<int> file;
		int aFrequency = 0, bFrequency = 0;
		checkForSubString(a, tree[indx].Root, tree[indx].text, aPosition, aFrequency);
		if (aFrequency == 0) continue;
		checkForSubString(b, tree[indx].Root, tree[indx].text, bPosition, bFrequency);
		if (bFrequency == 0) continue;
		sort(aPosition.begin(), aPosition.end());
		sort(bPosition.begin(), bPosition.end());
		int u = 0, o = 0;
		vector<pair<int, int>> mergePosition;
		while (u < aFrequency && o < bFrequency) {
			while (u < aFrequency) {
				if (bPosition[o].first > aPosition[u].first)
					u++;
				else break;
			}
			u--;
			if (u == -1) break;
			mergePosition.push_back({ aPosition[u].first,
				bPosition[o].first - aPosition[u].first + bPosition[o].second });
			frequency++;
			o++;
			u++;
		}
		if (u == -1 && frequency != 0) continue;
		/*cout << tree[indx].fileName << endl;
		for (int m = 0; m < mergePosition.size(); m++) {
			cout << "\nPosition : " << mergePosition[m].first << "\tlength: " << mergePosition[m].second;
		}*/
		//print_1_text_from_tree(tree, indx, mergePosition,frequency);
		sort(mergePosition.begin(), mergePosition.end());
		topFiveSearch.insert(mp(frequency, mp(indx, mergePosition)));
		mergePosition.clear();
	}
	clock_t end = clock();
	cout << "Time run: " << (float)(end - begin) / CLOCKS_PER_SEC << " s" << endl;
	int top5kithu = 5;
	string sentence;
	sentence.resize(170, '~');
	set<pair<int, pair<int, vector<pair<int, int>>>>>::reverse_iterator it = topFiveSearch.rbegin();
	for (; it != topFiveSearch.rend() && top5kithu > 0; ++it) {
		int j = 0;
		int i = (*it).ss.ff;
		vector<pair<int, int>> indexing = (*it).ss.ss;
		--top5kithu;
		//write txt and indexing
		textcolor(23);
		cout << tree[i].fileName << endl;
		textcolor(15);
		for (int k = 0; k < strlen(tree[i].text); ++k) {
			if (j < indexing.size()) {
				if (k == indexing[j].ff)                    textcolor(100);
				if (k == indexing[j].ss + indexing[j].ff) { textcolor(15); ++j; }
			}
			if (tree[i].text[k] == '^') cout << endl;
			else cout << tree[i].text[k];
		}
		cout << endl << sentence << endl;
	}
	topFiveSearch.clear();
}

bool Check_And(char n[])
{
	char m[] = "AND";
	if (strcmp(n, m) == 0) return true;
	else return false;
}

bool Check_Or(char n[])
{
	char m[] = "OR";
	if (strcmp(n, m) == 0) return true;
	else return false;
}

bool Check_Minus(char n[])
{
	int l = strlen(n);
	if (n[0] == '-')
	{
		for (int i = 0; i <= l; i++)
		{
			n[i] = n[i + 1];
		}
		return true;
	}
	return false;
}

bool Check_Intitle(char n[])
{
	int l = strlen(n);
	char compare[] = "intitle:";
	char temp[9], update[100];
	for (int i = 0; i < 8; i++)
	{
		temp[i] = n[i];
	}
	temp[8] = '\0';
	if (strcmp(temp, compare) == 0)
	{
		for (int i = 8; i < l; i++)
		{
			update[i - 8] = n[i];
		}
		update[l - 8] = '\0';
		strcpy_s(n, l, update);
		return true;
	}
	return false;
}

bool Check_Plus(char n[])
{
	int l = strlen(n);
	if (n[0] == '+')
	{
		for (int i = 0; i <= l; i++)
		{
			n[i] = n[i + 1];
		}
		return true;
	}
	return false;
}

bool Check_Filetype(char n[])
{
	int l = strlen(n);
	char compare[] = "filetype:";
	char temp[10], update[100];
	for (int i = 0; i < 9; i++)
	{
		temp[i] = n[i];
	}
	temp[9] = '\0';
	if (strcmp(temp, compare) == 0)
	{
		update[0] = '.';
		for (int i = 10; i < l + 1; i++)
		{
			update[i - 9] = n[i - 1];
		}
		update[l - 9 + 1] = '\0';
		strcpy_s(n, l, update);
		return true;
	}
	return false;
}

bool Check_Quote(char n[])
{
	int l = strlen(n);
	if ((int)n[0] == 34)
	{
		for (int i = 0; i <= l; i++)
		{
			n[i] = n[i + 1];
		}
		return true;
	}
	return false;
}

bool Check_Asterisk(char n[])
{
	if (n[0] == '*') return true;
	return false;
}

bool Check_In_Range(char n[])
{
	int l = strlen(n);
	int location = -1;
	if (n[l - 1] == '$' || n[l - 1] == '.')
		return false;// if last character is "$" or "." return false
	if (n[0] != '$')
		return false;//if first character isn't "$" return false
	else
	{
		if (n[1] == '.') return false;//return false  if after $ is "." or the last character is "."
		for (int i = 2; i < l; i++)//find the first character "."
		{
			if (n[i] == '.') {
				location = i;
				break;
			}
		}
		if (location == -1) return false;//if it doesn't have any "." return false
		else
		{
			for (int i = 2; i < location; i++)// check character from first "$" to first "."  
			{
				if (n[i] < 48 || n[i] > 57) return false;//return false if the character after $ isn't a number
			}
			int next = location + 1;
			if (n[next] != '.') return false;//if it only has 1 character "." return false
			else
			{
				if (n[next + 1] != '$') return false;//if character after ".." isn't "$" return false;
				else
				{
					for (int i = next + 2; i < l; i++)
					{
						if (n[i] < 48 || n[i]>57) return false;// return false if the character after $ isn't a number
					}
				}
			}
		}
	}
	return true;
}

bool Check_Synonym(char n[])
{
	int l = strlen(n);
	if (n[0] == '~')
	{
		for (int i = 0; i <= l; i++)
		{
			n[i] = n[i + 1];
		}
		return true;
	}
	return false;
}

bool Check_$(char n[]) {

	if (n[0] == '$') {
		int size = strlen(n);
		for (int i = 0; i < size; i++) {
			if (n[i] == '.')
				return false;
		}
		return true;
	}
	return false;
}

int check_aLL(char n[]) {
	if (Check_And(n))
		return 1;
	else if (Check_Or(n))
		return 2;
	else if (Check_Minus(n))
		return 3;
	else if (Check_Intitle(n))
		return 4;
	else if (Check_Plus(n))
		return 5;
	else if (Check_Filetype(n))
		return 6;
	else if (Check_$(n))
		return 7;
	else if (Check_Quote(n))
		return 9;
	else if (Check_Asterisk(n))
		return 10;
	else if (Check_In_Range(n))
		return 11;
	else if (Check_Synonym(n))
		return 12;
	else return 0;
}






