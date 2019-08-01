#include"StopWord.h"

void LoadStopWord(const char* path, vector<string>& stopword)
{
	ifstream fin;
	string temp;
	fin.open(path);
	if (!fin.is_open()) cout << "Can't open file.\n";
	else
	{
		while (!fin.eof())
		{
			getline(fin, temp, '\n');
			if (fin.eof()) break;
			stopword.push_back(temp);
		}
	}
	fin.close();
}

bool CheckStopWord(char n[], vector<string> stopword)
{
	int size = stopword.size();
	for (int i = 0; i < size; i++)
	{
		if (n == stopword[i]) return true;
	}
	return false;
}