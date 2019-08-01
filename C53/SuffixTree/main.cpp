#include"SuffixTree.h"
#include"Header.h"
#include"Graphics.h"
#include"SynonymDic.h"
#include"StopWord.h"

int main() {
	int numOfWord;
	string inp;
	SuffixTree* tree;
	vector<string> stopword;
	textcolor(3);
	LoadStopWord("StopWord.txt", stopword);
	clock_t begin = clock();
	int numOfFile = get_file_name(tree);
	clock_t end = clock();
	cout << "Time run: " << (float)(end - begin) / CLOCKS_PER_SEC << " s" << endl;
	struct TrieNode* root = build_Dic();
	system("pause");
	textcolor(7);
	system("cls");
	ifstream fIn;
	fIn.open("HistorySearching.txt");
	vector<string> hisSearching;
	hisSearching.push_back("Back");
	string line;
	for (int i = 4; i >= 0; --i) {
		getline(fIn, line);
		if (line == "\0") break;
		hisSearching.push_back(line);
	}
	char arrWord[1000][1000];
	int check = 0;
	while (1) {
		system("cls");
		searchBar(hisSearching, arrWord, numOfWord, check, stopword, inp);
		gotoxy(0, 0);
		system("cls");
		int numOfFileSearch = 0;
		vector<int> file;
		if (check == 2) {
			file = search_normal(tree, numOfFile, arrWord, numOfFileSearch, numOfWord);
		}
		else if (check == 11) {
			file = range_of_number(tree, numOfFile, arrWord, numOfFileSearch);
		}
		else if (check == 7 || check == 6 || check == 1 || check == 0 || check == 8 || check == 5) {
			file = filetype_operation(tree, numOfFile, arrWord, numOfFileSearch, numOfWord);
		}
		else if (check == 9) {
			int size = strlen(arrWord[numOfWord - 1]);
			if ((int)arrWord[numOfWord - 1][size - 1] == 34) {
				arrWord[numOfWord - 1][size - 1] = '\0';
			}
			size = strlen(arrWord[0]);
			for (int i = 1; i < numOfWord; i++) {
				arrWord[0][size] = ' ';
				size++;
				for (int j = 0; j < strlen(arrWord[i]); j++) {
					arrWord[0][size] = arrWord[i][j];
					size++;
				}
				arrWord[0][size] = '\0';
			}
			numOfWord = 1;
			file = search_normal(tree, numOfFile, arrWord, numOfFileSearch, numOfWord);
		}
		else if (check == 3) {
			set<pair<int, pair<int, vector<pair<int, int>>>>> topFiveSearch;
			// <frequence, <file i-th, position<index text, length>>>
			vector<pair<int, int>> position;
			int frequency = 0;
			for (int i = 0; i < numOfFile; i++) {
				if (!tree[i].search_linear(arrWord[numOfWord - 1], position, frequency)) {
					for (int j = 0; j < numOfWord - 1; j++) {
						if (tree[i].search_linear(arrWord[j], position, frequency)) {
							sort(position.begin(), position.end());
							topFiveSearch.insert(mp(frequency, mp(i, position)));
							file.push_back(i);
							numOfFileSearch++;
							break;
						}

					}
				}
				position.clear();
				frequency = 0;
			}
			//Indexing /////////////////////////////////
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
		else if (check == 12) {
			string key;
			string synonym[4];
			key = arrWord[0];
			bool kiemtra = search(root, key, synonym);
			if (!kiemtra)
				numOfFileSearch = 0;
			else {
				char convert[1000][1000];
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < synonym[i].size(); j++) {
						convert[i][j] = synonym[i][j];
					}
					convert[i][synonym[i].size()] = '\0';
				}
				file = search_normal(tree, numOfFile, convert, numOfFileSearch, 4);
			}
		}
		else if (check == 4) {
			file = Intitle_Search(tree, numOfFile, arrWord, numOfFileSearch, numOfWord);
		}
		else if (check == 10)
			search_asterisk(tree, inp, numOfFile);
		if (numOfFileSearch == 0)
			cout << "Oops! We cannot find what you want to search :(" << endl;
		system("pause");
	}
	delete[]tree;
	return 0;
}