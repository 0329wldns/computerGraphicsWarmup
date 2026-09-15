#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include <windows.h>
#include <conio.h>

using namespace std;

char wordCriterion{ ' ' };
bool keyState[9]{};
string inputWord;

// 문장들을 화면에 출력
void showStrings(const vector<string>&);

// a : 모든 문장의 문자들을 대소문자를 전환
void convertCase(vector<string>&);

// b : 문장에 단어가 몇개인지 반환
int countWords(const string&);

// d : 각 문장 별로 거꾸로 바꾸기
void reverseLines(vector<string>&);

// e : 모든 공백에 “*” 문자 삽입
void insertAsterisk(vector<string>&);

// f : 모든 단어들을 거꾸로 바꾸기
void reverseWords(vector<string>&);

// g : 문자 내부의 특정 문자를 다른 문자로 바꾸기
void replaceCharacter(vector<string>&);

// h : 숫자 뒤에 오는 문장을 다음 줄로 넘기기
void findNum(vector<string>&);

// j : 문장의 순서 바꾸기
void replaceLine(vector<string>&);

// 대소문자 구별없이 문자열 비교
bool compareIgnoreCase(const string&, const string&);

int main()
{
	string fileName{ "data.txt" };
	ifstream inFile(fileName);

	// 완성하고 주석풀기
	/*cout << "파일 이름을 입력하세요: ";
	cin >> fileName;
	inFile.open(fileName, ios::in);*/
	
	if (!inFile.is_open()) 
	{
		cout << "파일을 열 수 없습니다." << endl;
		return -1;
	}
	else cout << "파일 읽기 성공" << endl;

	// 줄 단위로 저장하기 위해 vector<string> 사용
	vector<string> fileContents;

	string line{};
	while (getline(inFile, line)) fileContents.push_back(line);
	inFile.close();

	const vector<string> copy{ fileContents };	// 파일 내용 원본 보존용

	for (const auto& line : fileContents) cout << line << endl;
	cout << "------------------------------------------------------" << endl;

	// 명령어 입력 루프
	while (true)
	{
		cout << "명령어 입력 a ~ j, q: 종료" << endl;

		if (_kbhit)
		{
			int key = _getch();

			switch (key)
			{
			case 'a':
				convertCase(fileContents);
				break;
			case 'd':
				reverseLines(fileContents);
				break;
			case 'e':
				insertAsterisk(fileContents);
				break;
			case 'f':
				reverseWords(fileContents);
				break;
			case 'g':
				if (!keyState[6]) replaceCharacter(fileContents);
				else
				{
					for (int i = 0; i < 9; ++i) keyState[i] = false;
					fileContents = copy;
					key = 0;
					inputWord.clear();
				}
				break;
			case 'h':
				if (!keyState[7]) findNum(fileContents);
				else
				{
					for (int i = 0; i < 9; ++i) keyState[i] = false;
					fileContents = copy;
					key = 0;
					inputWord.clear();
				}
				break;
			case 'i':
				if (!keyState[8])
				{
					cout << "찾을 단어 입력: ";
					cin >> inputWord;
				}
				else inputWord.clear();
				break;
			case 'j':
				replaceLine(fileContents);
				break;
			case 'q':
				return 0;
				break;
			default:
				break;
			}
			if (key >= 'a' && key <= 'i') keyState[key - 'a'] = !keyState[key - 'a'];

			system("cls");
			if (keyState[0]) cout <<		"a: 대소문자 전환 됨";					if (keyState[5]) cout << "\t\t\tf: 단어 거꾸로 출력됨";
			if (keyState[1]) cout << endl << "b: 각 줄의 단어개수 출력됨";			if (keyState[6]) cout << "\t\tg: 특정 문자 변환됨";
			if (keyState[2]) cout << endl << "c: 대문자로 시작하는 단어 강조됨";	if (keyState[7]) cout << "\th: 숫자 뒤 문장 넘겨짐";
			if (keyState[3]) cout << endl << "d: 각 문장 거꾸로 출력됨";			if (keyState[8]) cout << "\t\ti: 입력받은 단어 강조됨";
			if (keyState[4]) cout << endl << "e: 공백 -> * 로 전환됨";
			cout << endl;

			showStrings(fileContents);

			cout << "------------------------------------------------------" << endl;
		}
	}
	return 0;
}

void showStrings(const vector<string>& lines)
{
	vector<vector<string>> words;

	// 문장을 단어 단위로 쪼갬
	for (const auto& line : lines)
	{
		vector<string> tempLine;
		string word;
		for (const auto& ch : line)
		{
			if (ch == wordCriterion)
			{
				if (!word.empty())
				{
					tempLine.push_back(word);
					word.clear();
				}
			}
			else word += ch;
		}
		if (!word.empty())
		{
			tempLine.push_back(word);
			word.clear();
		}
		words.push_back(tempLine);
	}

	int sameWordCnt{};
	int upperWordCnt{};
	for (const auto& line : words)
	{
		for (const auto& word : line)
		{
			if (keyState[2] && isupper(word[0]))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				++upperWordCnt;
			}

			if (keyState[8] && !_stricmp(word.c_str(), inputWord.c_str()))				// _stricmp : 두 문자열이 같으면 flase를 반환함
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				++sameWordCnt;
			}
			cout << word;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << wordCriterion;
		}
		if (keyState[1]) cout << " 단어 수: " << line.size();
		cout << endl;
	}
	if (keyState[2]) cout << "대문자로 시작하는 단어 수: " << upperWordCnt << endl;
	if (keyState[8]) cout << "일치하는 단어 수: " << sameWordCnt << endl;
}

void convertCase(vector<string>& lines)
{
	for (auto& line : lines)
	{
		for (auto& ch : line)
		{
			if (isupper(ch)) ch = tolower(ch);
			else if (islower(ch)) ch = toupper(ch);
		}
	}
}

int countWords(const string& line)
{
	int wordCount = 0;
	bool inWord = false;

	for (const auto& ch : line)
	{
		if (ch == wordCriterion)
		{
			if (inWord)
			{
				++wordCount;
				inWord = false;
			}
		}
		else inWord = true;
	}
	if (inWord) ++wordCount; // 마지막 단어 처리
	
	return wordCount;
}

void reverseLines(vector<string>& lines)
{
	for (auto& line : lines) reverse(line.begin(), line.end());
}

void insertAsterisk(vector<string>& lines)
{
	for (auto& line : lines)
	{
		string modifiedLine;
		for (const auto& ch : line)
		{
			if (!keyState[4])
			{
				if (isspace(ch)) modifiedLine += "*";
				else modifiedLine += ch;
			}
			else
			{
				if (ch == '*') modifiedLine += " ";
				else modifiedLine += ch;
			}
		}
		line = modifiedLine;
	}
	if (!keyState[4]) wordCriterion = '*';
	else wordCriterion = ' ';
}

void reverseWords(vector<string>& lines)
{
	for (auto& line : lines)
	{
		string reversedLine;
		string word;
		for (const auto& ch : line)
		{
			if (ch == wordCriterion)
			{
				if (!word.empty())
				{
					reverse(word.begin(), word.end());
					reversedLine += word + wordCriterion;
					word.clear();
				}
			}
			else word += ch;
		}
		if (!word.empty())
		{
			reverse(word.begin(), word.end());
			reversedLine += word;
		}
		line = reversedLine;
	}
}

void replaceCharacter(vector<string>& lines)
{
	char targetChar, replacementChar;
	
	cout << "바꿀 문자 입력: ";
	cin >> targetChar;
	cout << "대체할 문자 입력: ";
	cin >> replacementChar;
	for (auto& line : lines)
	{
		for (auto& ch : line)
		{
			if (ch == targetChar) ch = replacementChar;
		}
	}
}

void findNum(vector<string>& lines)
{
	vector<string> result;
	int cnt{};
	for (const auto& line : lines)
	{
		string tempStr;
		for (const auto& ch : line)
		{
			tempStr += ch;
			if (isdigit(ch))
			{
				result.push_back(tempStr);
				tempStr.clear();
			}
		}
		result.push_back(tempStr);
	}
	lines = result;
}

void replaceLine(vector<string>& lines)
{
	string temp{ lines[0] };
	for (int i = 0; i < lines.size() - 1; ++i) lines[i] = lines[i + 1];
	lines[lines.size() - 1] = temp;
}

bool compareIgnoreCase(const string& str1, const string& str2)
{
	
	return false;
}
