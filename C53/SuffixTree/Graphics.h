#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include"Header.h"
#include"SuffixTree.h"

void gotoxy(int x, int y);
void textcolor(int color);
void searchBar(vector<string>& hisSearching, char arrWord[1000][1000], int& n, int& Check, vector<string> stopword, string& inp);
void print_text(SuffixTree* tree, vector<int> file, int numOfFile, vector<pair<int, int>> vitri);
int CreateMenu(vector<string> his, int x, int y);
void DrawBoard(vector<string> his, int Len, int x, int y);
void drawTitle();

#endif // !GRAPHICS_H_

