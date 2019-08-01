#include"Graphics.h"
#include"SuffixTree.h"
#include"StopWord.h"

void textcolor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y) {
	static HANDLE  h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x,y };
	SetConsoleCursorPosition(h, c);
}

void drawTitle()
{
	ifstream fIn("edith.txt");
	int length = 0;
	int x = 52;
	for (int i = 0; i < 5; ++i) //edith
	{
		x += length;
		if (i < 4 && i > 1) length = 12;
		else length = 14;
		textcolor(11);
		for (int j = 0; j < 11; ++j) //height
		{
			gotoxy(x, 0 + j);
			string txt;
			getline(fIn, txt, '\n');
			cout << txt;
		}
		Sleep(500);
	}
	textcolor(15);
}

void searchBar(vector<string>& hisSearching, char arrWord[1000][1000], int &n, int &Check, vector<string> stopword, string& inp) {
	//218 196 191
	//179 set 179
	//192 196 217
	drawTitle();
	int lengthBar = 50;
	int coordX = 60;
	int coordY = 15;
	gotoxy(coordX, coordY);  cout << char(218); for (int i = 0; i < lengthBar; ++i) cout << char(196); cout << char(191) << endl;
	gotoxy(coordX, coordY + 1); cout << char(179) << setw(lengthBar + 1) << char(179) << endl;
	gotoxy(coordX, coordY + 2); cout << char(192); for (int i = 0; i < lengthBar; ++i) cout << char(196); cout << char(217) << endl;

	if (hisSearching.size() != 1)
		DrawBoard(hisSearching, 0, coordX + 1, coordY + 2);

	gotoxy(coordX + 1, coordY + 1);
	string input;
	//char arrWord[1000][1000];
	int key;
	do
	{
		key = _getch();
		if ((key == 0 || key == 224) && hisSearching.size() != 1) {
			int HistoryChoice = CreateMenu(hisSearching, coordX + 2, coordY + 3);
			if (HistoryChoice == hisSearching.size() - 1)
				gotoxy(coordX + 1 + input.size(), coordY + 1);
			else {
				gotoxy(coordX + 1, coordY + 1);
				input.clear();
				input.resize(50, ' ');
				cout << input;
				input.clear();
				gotoxy(coordX + 1, coordY + 1);
				input += hisSearching[hisSearching.size() - 1 - HistoryChoice];
				cout << hisSearching[hisSearching.size() - 1 - HistoryChoice];
			}
		}
		else if (key >= 32 && key <= 126)
		{
			input += char(key);
			cout << char(key);
		}
		else if (key == 8 && input.size() != 0)
		{
			cout << "\b \b";
			input.resize(input.size() - 1);
		}
	} while (key != 13);
	system("cls");
	ifstream fIn("edith.txt");
	int length = 0;
	int x = 52;
	for (int i = 0; i < 5; ++i) //edith
	{
		x += length;
		if (i < 4 && i > 1) length = 12;
		else length = 14;
		textcolor(11);
		for (int j = 0; j < 11; ++j) //height
		{
			gotoxy(x, 0 + j);
			string txt;
			getline(fIn, txt, '\n');
			cout << txt;
		}
	}
	textcolor(15);
	if (hisSearching.size() == 6) {
		hisSearching.erase(hisSearching.begin() + 1);
		hisSearching.push_back(input);
	}
	else hisSearching.push_back(input);

	ofstream fOut;
	fOut.open("HistorySearching.txt");
	fOut.clear();
	for (int i = 1; i < hisSearching.size(); ++i) {
		fOut << hisSearching[i] << endl;
	}
	fOut.close();
	gotoxy(coordX, coordY + 4);
	inp = input;
	int j = 0;
	n = 0;
	int check = 0;	
	for (int k = 0; k < input.size(); ++k) {
		if (input[k] != ' ') {
			arrWord[n][j] = input[k];
			++j;
		}
		else if (arrWord[n][j - 1] != '\0') {
			arrWord[n][j] = '\0';
			if (!CheckStopWord(arrWord[n], stopword)) {
				if (check == 0) {
					check = check_aLL(arrWord[n]);
					if (check == 0)
						++n;
					else {
						Check = check;
						if (check == 10) {

						}
						else if (check != 1 && check != 2)
							++n;
					}
				}
			}
			j = 0;
		}
	}
	arrWord[n][j] = '\0';
	if (n == 0 || check == 0) {
		if (!CheckStopWord(arrWord[n], stopword)) {
			Check = check_aLL(arrWord[n]);
			if (Check == 10) {

			}
		}
		else n--;
	}
	++n;
}

void print_text(SuffixTree* tree, vector<int> file, int numOfFile, vector<pair<int, int>> vitri) {
	for (int i = 0; i < (int)file.size(); ++i) {
		cout << tree[file[i]].fileName << endl;
		ifstream fileNews;
		fileNews.open(tree[file[i]].fileName);
		if (!fileNews.is_open()) {
			cout << "File not found" << endl;
			continue;
		}
		string line;
		vector<string> word;
		int n = 0;
		while (fileNews.good()) {
			getline(fileNews, line);
			//if (line == "\n") cout << line;
			cout << line;
			/*word = removeDupWord(line);
			int maxWidth = 100;
			word = fullJustify(word, maxWidth);
			string temp;
			temp.resize(maxWidth, ' ');
			for (int i = 0; i < word.size(); ++i) {
				if (word[i] != temp) cout << word[i] << endl;
			}*/
		}
		cout << endl << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
		cout << endl << endl;
		fileNews.close();
	}
}

int CreateMenu(vector<string> his, int x, int y) {
	int position = 0;
	DrawBoard(his, 0, x - 1, y - 1);
	while (true) {
		textcolor(15);
		//cout << "Main Menu\n\n";
		for (int i = 0; i < his.size(); ++i) {
			gotoxy(x, y + 2 * i);
			if (i == position) { textcolor(11); cout << his[his.size() - 1 - i]; }
			else { textcolor(15); cout << his[his.size() - 1 - i]; }
		}
		int key = _getch();
		switch (key) {
		case 13://ENTER
			gotoxy(x, y + 2 * position);
			textcolor(15); cout << his[his.size() - 1 - position];
			return position;
		case 72://UP
			if (position - 1 == -1) position = his.size() - 1;
			else position -= 1;
			break;
		case 80://DOWN
			if (position == his.size() - 1) position = 0;
			else position += 1;
			break;
		}
	}
}

void DrawBoard(vector<string> his, int Len, int x, int y)
{
	int length = 0;
	for (int i = 0; i < his.size(); ++i)
		length = max(length, his[i].size());

	for (int i = 0; i < his.size(); ++i)
		if (his[i].size() != length) his[i].resize(length, ' ');

	for (int i = his.size() - 1; i >= 0; --i)
	{
		if (i == his.size() - 1)
		{
			gotoxy(x, y);
			cout << char(218);
			for (int i = 0; i < length; ++i) cout << char(196);
			if (Len != 0) cout << char(194);
			else cout << char(191);
			for (int i = 0; i < Len; ++i)
			{
				cout << char(196);
				if (i == Len - 1) cout << char(191);
			}
			gotoxy(x, ++y);
			cout << char(179) << his[i] << char(179);
			for (int i = 0; i < Len; ++i) {
				cout << " ";
				if (i == Len - 1) cout << char(179);
			}
			gotoxy(x, ++y);
		}
		else
		{
			gotoxy(x, y);
			cout << char(195);
			for (int i = 0; i < length; ++i) cout << char(196);
			if (Len > 0) cout << char(197);
			else cout << char(180);
			for (int i = 0; i < Len; ++i)
			{
				cout << char(196);
				if (i == Len - 1) cout << char(180);
			}
			gotoxy(x, ++y);
			cout << char(179) << his[i] << char(179);
			for (int i = 0; i < Len; ++i)
			{
				cout << " ";
				if (i == Len - 1) cout << char(179);
			}
			gotoxy(x, ++y);
		}
		if (i == 0)
		{
			gotoxy(x, y);
			cout << char(192);
			for (int i = 0; i < length; ++i) cout << char(196);
			if (Len > 0) cout << char(193);
			else cout << char(217);
			for (int i = 0; i < Len; ++i)
			{
				cout << char(196);
				if (i == Len - 1) cout << char(217);
			}
			gotoxy(x, ++y);
		}
	}
}
