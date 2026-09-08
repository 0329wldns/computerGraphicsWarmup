#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <random>
#include <windows.h>

using namespace std;

random_device rd;
mt19937 gen(rd());

const int scorePerMatch{ 10 };     // 짝 맞춤 시 획득 점수
const int penaltyPerMiss{ 2 };     // 실패 시 감점
int maxAttempts{};			// 최대 시도 횟수
int currentScore{};			// 현재 점수
int currentAttempts{};		// 현재 시도 횟수
int boardWidth{};			// 보드 가로 크기
int boardHeight{};			// 보드 세로 크기
bool isRunning{ true };		// 게임 진행 상태 플래그

// 카드 정보를 담는 구조체
struct Card
{
	char value{};
	bool isRevealed{};
	bool isMatched{};
};

Card board[6][6]{}; // 최대 크기 6x6 보드

void initGame();
void drawBoard(bool showHint);
void processCommand(char cmd);
void processSelection(const string& input1, const string& input2);
void shuffleCards(char* pool, int size);



int main()
{
	initGame();

	while (isRunning)
	{
		drawBoard(false);
		cout << "\n명령어(r: 리셋, h: 힌트, q: 종료) 또는\n";
		cout << "카드 좌표 2개 입력 (예: a1 b2): ";

		string input1{};
		cin >> input1;

		// 길이가 1이면 명령어로 간주, 그 이상이면 좌표로 간주하여 처리
		if (input1.length() == 1) processCommand(input1[0]);
		else
		{
			string input2{};
			cin >> input2;
			processSelection(input1, input2);
		}
	}

	cout << "\n게임을 종료합니다. 최종 점수: " << currentScore << "\n";
	return 0;
}



// 게임 초기화 및 보드 생성
void initGame()
{
	currentScore = 0;
	currentAttempts = 0;
	boardWidth = 0;
	boardHeight = 0;

	// 보드 크기 입력 (3~6 범위 강제)
	while (boardWidth < 3 || boardWidth > 6 || boardHeight < 3 || boardHeight > 6)
	{
		system("cls");
		cout << "보드의 가로 크기를 입력하세요 (3~6): ";
		cin >> boardWidth;
		cout << "보드의 세로 크기를 입력하세요 (3~6): ";
		cin >> boardHeight;
	}

	maxAttempts = boardWidth * boardHeight * 3;
	int totalCards{ boardWidth * boardHeight };
	int pairs{ totalCards / 2 };

	char pool[36]{};
	int index{};

	// 소문자 쌍 생성
	for (int i{}; i < pairs; ++i)
	{
		pool[index++] = 'a' + i;
		pool[index++] = 'a' + i;
	}

	// 홀수 개수일 경우 마지막 한 칸에 조커 '@' 추가
	if (totalCards % 2) pool[index] = '@';

	shuffleCards(pool, totalCards);

	// 보드에 무작위로 섞인 카드 배치
	index = 0;
	for (int r{}; r < boardHeight; ++r)
	{
		for (int c{}; c < boardWidth; ++c)
		{
			board[r][c].value = pool[index++];
			board[r][c].isRevealed = false;
			board[r][c].isMatched = false;
		}
	}
}

// 카드 무작위 섞기 (Fisher-Yates 알고리즘 응용)
void shuffleCards(char* pool, int size)
{
	uniform_int_distribution<> num(0, size - 1);
	for (int i{}; i < size; ++i)
	{
		int r{ num(gen) };
		char temp{ pool[i] };
		pool[i] = pool[r];
		pool[r] = temp;
	}
}

// 화면 출력 함수 (showHint가 true면 모든 카드를 보여줌)
void drawBoard(bool showHint)
{
	system("cls");
	cout << "점수: " << currentScore << " | 남은 횟수: " << (maxAttempts - currentAttempts) << "\n\n";

	// 열(알파벳) 인덱스 출력
	cout << "  ";
	for (int c{}; c < boardWidth; ++c) cout << (char)('a' + c) << " ";
	cout << "\n";

	for (int r{}; r < boardHeight; ++r)
	{
		// 행(숫자) 인덱스 출력
		cout << r + 1 << " ";

		for (int c{}; c < boardWidth; ++c)
		{
			// 힌트 모드이거나, 뒤집혔거나, 짝을 맞춘 경우 문자 출력
			if (showHint || board[r][c].isRevealed || board[r][c].isMatched)
			{
				if (board[r][c].isMatched)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
					cout << (char)toupper(board[r][c].value);
				}
				else if (board[r][c].isRevealed)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
					cout << (char)toupper(board[r][c].value);
				}
				else cout << board[r][c].value;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << " ";
			}
			else cout << "* "; // 한 줄 코드
		}
		cout << "\n";
	}
}

// 단일문자 명령어 처리
void processCommand(char cmd)
{
	switch (cmd)
	{
	case 'r':
		initGame();
		break;
	case 'h':
		drawBoard(true);
		Sleep(2000); // 2초간 힌트 보여줌
		break;
	case 'q':
		isRunning = false;
		break;
	default:
		break;
	}
}

// 카드 좌표 처리 및 짝 맞춤 로직
void processSelection(const string& input1, const string& input2)
{
	if (input1.length() < 2 || input2.length() < 2) return;

	// 문자열(a1)을 인덱스(0, 0)로 파싱
	int c1{ input1[0] - 'a' };
	int r1{ input1[1] - '1' };
	int c2{ input2[0] - 'a' };
	int r2{ input2[1] - '1' };

	// 예외 처리 (범위 초과, 같은 카드 선택, 이미 맞춘 카드 선택 시 무시)
	if (c1 < 0 || c1 >= boardWidth || r1 < 0 || r1 >= boardHeight) return;
	if (c2 < 0 || c2 >= boardWidth || r2 < 0 || r2 >= boardHeight) return;
	if (c1 == c2 && r1 == r2) return;
	if (board[r1][c1].isMatched || board[r2][c2].isMatched) return;

	// 선택한 두 카드 임시 뒤집기
	board[r1][c1].isRevealed = true;
	board[r2][c2].isRevealed = true;

	drawBoard(false); // 뒤집은 상태 보여주기
	cout << "\n선택한 카드를 확인 중입니다...\n";
	Sleep(1000); // 1초 대기

	// 카드 매칭 확인 (값이 같거나, 둘 중 하나가 조커 '@'인 경우)
	if (board[r1][c1].value == board[r2][c2].value || board[r1][c1].value == '@' || board[r2][c2].value == '@')
	{
		board[r1][c1].isMatched = true;
		board[r2][c2].isMatched = true;
		currentScore += scorePerMatch;
	}
	else
	{
		// 불일치 시 다시 뒤집기
		board[r1][c1].isRevealed = false;
		board[r2][c2].isRevealed = false;
		currentScore -= penaltyPerMiss;
	}

	currentAttempts++;
	if (currentAttempts >= maxAttempts) isRunning = false;
}