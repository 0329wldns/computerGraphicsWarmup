#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>

using namespace std;

// --- 전역 변수 및 게임 설정 (요구사항: 유니폼 초기화) ---
int boardSize{ 30 };       // 현재 보드 크기
int maxBoardSize{ 40 };    // 보드 최대 크기 
int minBoardSize{ 10 };    // 보드 최소 크기
bool isRunning{ true };    // 프로그램 실행 상태
bool showArea1{};          // 1번 도형 면적 표시 토글 플래그
bool showArea2{};          // 2번 도형 면적 표시 토글 플래그

// 사각형 데이터를 관리하는 구조체 (클래스 배제)
struct Rect
{
	int x{};
	int y{};
	int width{};
	int height{};

	void move(const int&, const int&);
	void scale(const int&, const int&);
};

Rect rect1{};
Rect rect2{};

// --- 함수의 원형(Prototype) 선언부 (요구사항: 선언과 정의 분리, 첫 글자 소문자) ---
void initGame();
void drawBoard();
void processCommand(char cmd);
bool isInside(const Rect& r, int c, int row);

// --- Main 함수 ---
int main()
{
	initGame();

	while (isRunning)
	{
		drawBoard();

		// _getch()를 이용한 엔터 없는 즉각적인 키 입력
		char cmd{ static_cast<char>(_getch()) };
		processCommand(cmd);
	}

	return 0;
}

// --- 함수의 정의부 (Definition) ---

// 게임 초기화 및 좌표 입력 (한국어 가이드 반영)
void initGame()
{
	system("cls");
	int x1{}, y1{}, x2{}, y2{};

	cout << "1번 도형 좌표 입력 (x1 y1 x2 y2): ";
	cin >> x1 >> y1 >> x2 >> y2;
	rect1.x = x1;
	rect1.y = y1;
	rect1.width = x2 - x1 + 1;
	rect1.height = y2 - y1 + 1;

	cout << "2번 도형 좌표 입력 (x1 y1 x2 y2): ";
	cin >> x1 >> y1 >> x2 >> y2;
	rect2.x = x1;
	rect2.y = y1;
	rect2.width = x2 - x1 + 1;
	rect2.height = y2 - y1 + 1;

	// 리셋 시 면적 출력 상태도 초기화
	showArea1 = false;
	showArea2 = false;
}

// 화면 렌더링 함수
void drawBoard()
{
	system("cls");
	cout << "========================================================\n";
	cout << "[공통] +: 보드증가 | -: 보드감소 | r: 리셋 | ESC: 종료\n";
	cout << "[도형1] 이동:q/w 확대축소:e x축:a y축:s 혼합:d 면적:f (토글)\n";
	cout << "[도형2] 이동:p/o 확대축소:i x축:; y축:l 혼합:k 면적:j (토글)\n";
	cout << "(Shift와 함께 누르면 반대 방향/반대 변환이 적용됩니다)\n";
	cout << "========================================================\n";

	// 요구사항: for문 조건에서 전위 증가 연산자(++r, ++c) 사용
	for (int r{}; r < boardSize; ++r)
	{
		for (int c{}; c < boardSize; ++c)
		{
			bool hit1{ isInside(rect1, c, r) };
			bool hit2{ isInside(rect2, c, r) };

			// 요구사항: SetConsoleTextAttribute 직접 사용
			if (hit1 && hit2)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // 빨간색 (충돌)
				cout << "# ";
			}
			else if (hit1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);  // 파란색 (도형 1)
				cout << "O ";
			}
			else if (hit2)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // 초록색 (도형 2)
				cout << "X ";
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // 기본색 (빈 공간)
				cout << "0 ";
			}
		}
		cout << "\n";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // 색상 원상복구

	// 요구사항: 한 줄 코드 옆에 붙이기 & 면적 개별 토글 출력
	if (showArea1) cout << "\n[도형 1 면적] " << rect1.width << " x " << rect1.height << " = " << (rect1.width * rect1.height);
	if (showArea2) cout << "\n[도형 2 면적] " << rect2.width << " x " << rect2.height << " = " << (rect2.width * rect2.height);
	if (showArea1 || showArea2) cout << "\n";
}

// 명령어 라우팅 처리 (요구사항: switch문 활용)
void processCommand(char cmd)
{
	switch (cmd)
	{
		// --- 1번 도형 명령어 ---
	case 'q': rect1.move(1, 0); break;
	case 'Q': rect1.move(-1, 0); break;
	case 'w': rect1.move(0, 1); break;
	case 'W': rect1.move(0, -1); break;
	case 'e': rect1.scale(-1, -1); break;
	case 'E': rect1.scale(1, 1); break;
	case 'a': rect1.scale(1, 0); break;
	case 'A': rect1.scale(-1, 0); break;
	case 's': rect1.scale(0, 1); break;
	case 'S': rect1.scale(0, -1); break;
	case 'd': rect1.scale(1, -1); break;
	case 'D': rect1.scale(-1, 1); break;
	case 'f':
	case 'F': showArea1 = !showArea1; break; // 토글 방식으로 변경

		// --- 2번 도형 명령어 ---
	case 'p': rect2.move(1, 0); break;
	case 'P': rect2.move(-1, 0); break;
	case 'o': rect2.move(0, 1); break;
	case 'O': rect2.move(0, -1); break;
	case 'i': rect2.scale(-1, -1); break;
	case 'I': rect2.scale(1, 1); break;
	case ';': rect2.scale(1, 0); break;
	case ':': rect2.scale(-1, 0); break; // Shift + ; 는 : 로 입력됨
	case 'l': rect2.scale(0, 1); break;
	case 'L': rect2.scale(0, -1); break;
	case 'k': rect2.scale(1, -1); break;
	case 'K': rect2.scale(-1, 1); break;
	case 'j':
	case 'J': showArea2 = !showArea2; break; // 토글 방식으로 변경

		// --- 공통 명령어 ---
	case '+': if (boardSize < maxBoardSize) ++boardSize; break;
	case '-': if (boardSize > minBoardSize) --boardSize; break;
	case 'r':
	case 'R': initGame(); break;
	case 27:  isRunning = false; break; // ESC 키 (ASCII 27)

	default: break;
	}
}

// 도형 이동 로직 (보드 범위를 넘어갈 시 Wrapping 처리)
void Rect::move(const int& dx, const int& dy)
{
	x = (x + dx % boardSize + boardSize) % boardSize;
	y = (y + dy % boardSize + boardSize) % boardSize;
}

// 도형 크기 변경 로직
void Rect::scale(const int& dw, const int& dh)
{
	width += dw;
	height += dh;

	// 크기가 1보다 작아지거나 보드 크기보다 커지지 않도록 제한 (요구사항: 한 줄 코드 옆에 붙이기)
	if (width < 1) width = 1;
	if (height < 1) height = 1;
	if (width > boardSize) width = boardSize;
	if (height > boardSize) height = boardSize;
}

// 특정 좌표 (c, row)가 사각형 내부에 속하는지(Wrap 포함) 체크하는 논리 함수
bool isInside(const Rect& r, int c, int row)
{
	int dx{ (c - r.x % boardSize + boardSize) % boardSize };
	int dy{ (row - r.y % boardSize + boardSize) % boardSize };

	return (dx < r.width && dy < r.height);
}