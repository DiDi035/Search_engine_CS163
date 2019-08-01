#ifndef STOPWORD_H_
#define STOPWORD_H_

#include"Header.h"

void LoadStopWord(const char* path, vector<string>& stopword);
bool CheckStopWord(char n[], vector<string> stopword);

#endif
