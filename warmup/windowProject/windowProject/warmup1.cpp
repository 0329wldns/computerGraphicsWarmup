#include <Windows.h>
#include <random>
#include <array>
#include <string>

using namespace std;

// 윈도우 크기 및 스타일 정의
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_STYLE WS_OVERLAPPED | WS_MINIMIZEBOX | WS_BORDER | WS_SYSMENU

HINSTANCE g_hinst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Matrix Calculator";

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> num(1, 9);

// 행렬의 크기
const int gRow{4};
const int gCol{4};

const int gColWidth{30};	// 숫자 간격
const int gLineHeight{25};	// 줄 간격

// 그리기 시작 위치
const int gStartX{50};
const int gStartY{30};

// 행, 열 크기를 받아서 랜덤 행렬을 생성하는 함수
array<array<int, gRow>, gCol> genRandMatrix(const int, const int);

// 행렬을 출력하는 함수
void printMatrix(HDC, const int, const int, const array<array<int, gRow>, gCol>&);

// 두 행렬을 더하는 함수
array<array<int, gRow>, gCol> addMatrix(const array<array<int, gRow>, gCol>&, const array<array<int, gRow>, gCol>&);

// 두 행렬을 빼는 함수
array<array<int, gRow>, gCol> subMatrix(const array<array<int, gRow>, gCol>&, const array<array<int, gRow>, gCol>&);

// 두 행렬을 곱하는 함수
array<array<int, gRow>, gCol> mulMatrix(const array<array<int, gRow>, gCol>&, const array<array<int, gRow>, gCol>&);

// 3x3 행렬의 행렬식을 구하는 함수
int get3x3Determinant(const array<array<int, 3>, 3>&);

// 행렬식을 구하는 함수
int get4x4Determinant(const array<array<int, gRow>, gCol>&);

// 전치행렬을 구하는 함수
array<array<int, gRow>, gCol> transMatrix(const array<array<int, gRow>, gCol>&);

LRESULT CALLBACK WndProc(HWND nWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass{};
	g_hinst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WINDOW_STYLE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 해상도에 맞게 윈도우 크기 조정
	RECT rt{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 0);
	SetWindowPos(hWnd, NULL, 100, 50, rt.right - rt.left, rt.bottom - rt.top, 0);

	while (GetMessage(&Message, 0, 0, 0)) 
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	HDC mDC;
	HBITMAP hBitmap;
	PAINTSTRUCT ps;

	static wstring currentState = L"";		// 현재 상태를 나타내는 변수

	static array<array<int, gRow>, gCol> matrix1 = genRandMatrix(gRow, gCol);
	static array<array<int, gRow>, gCol> matrix2 = genRandMatrix(gRow, gCol);
	static array<array<int, gRow>, gCol> resultMatrix1{};
	static array<array<int, gRow>, gCol> resultMatrix2{};

	static int det1{};
	static int det2{};

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'A':
			currentState = L"+";
			resultMatrix1 = addMatrix(matrix1, matrix2);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'D':
			currentState = L"-";
			resultMatrix1 = subMatrix(matrix1, matrix2);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'M':
			currentState = L"x";
			resultMatrix1 = mulMatrix(matrix1, matrix2);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'R':
			currentState = L"d";
			det1 = get4x4Determinant(matrix1);
			det2 = get4x4Determinant(matrix2);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'T':
			currentState = L"t";
			resultMatrix1 = transMatrix(matrix1);
			resultMatrix2 = transMatrix(matrix2);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'S':
			currentState = L"";
			matrix1 = genRandMatrix(gRow, gCol);
			matrix2 = genRandMatrix(gRow, gCol);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'Q':
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, SCREEN_WIDTH, SCREEN_HEIGHT);
		SelectObject(mDC, (HBITMAP)hBitmap);
		Rectangle(mDC, -1, -1, SCREEN_WIDTH + 1, SCREEN_HEIGHT + 1);

		// ... (여기에 그리기 작업을 수행)
		TextOut(mDC, gStartX, gStartY, L"matrix 1", static_cast<int>(wcslen(L"matrix 1")));
		printMatrix(mDC, gStartX, gStartY + 20, matrix1);
		TextOut(mDC, gStartX + ((gCol + 1) * gColWidth), gStartY, L"matrix 2", static_cast<int>(wcslen(L"matrix 2")));
		printMatrix(mDC, gStartX + ((gCol + 1) * gColWidth), gStartY + 20, matrix2);

		if (currentState != L"")
		{
			if (currentState == L"+" || currentState == L"-" || currentState == L"x")	// 더하기, 빼기, 곱하기 연산
			{
				TextOut(mDC, gStartX + ((gCol + 1) * gColWidth) * 2, gStartY, L"result", static_cast<int>(wcslen(L"result")));
				TextOut(mDC, gStartX + (gCol * gColWidth), gStartY + (gRow * gLineHeight) / 2, currentState.c_str(), static_cast<int>(currentState.length()));
				TextOut(mDC, gStartX + ((gCol + 0.5) * gColWidth) * 2, gStartY + (gRow * gLineHeight) / 2, L"=", 1);
				printMatrix(mDC, gStartX + ((gCol + 1) * gColWidth) * 2, gStartY + 20, resultMatrix1);
			}
			else if (currentState == L"d")	// 행렬식 연산
			{
				wstring detStr1 = L"det(matrix1) = " + to_wstring(det1);
				wstring detStr2 = L"det(matrix2) = " + to_wstring(det2);
				TextOut(mDC, gStartX + ((gCol + 1) * gColWidth) * 2, gStartY, detStr1.c_str(), static_cast<int>(detStr1.length()));
				TextOut(mDC, gStartX + ((gCol + 1) * gColWidth) * 2, gStartY + 30, detStr2.c_str(), static_cast<int>(detStr2.length()));
			}
			else if (currentState == L"t")	// 전치행렬 연산
			{
				TextOut(mDC, gStartX + ((gCol + 1) * gColWidth) * 2, gStartY, L"transpose(matrix1)", static_cast<int>(wcslen(L"transpose(matrix1)")));
				printMatrix(mDC, gStartX + ((gCol + 1) * gColWidth) * 2, gStartY + 20, resultMatrix1);
				TextOut(mDC, gStartX + ((gCol + 1) * gColWidth) * 3, gStartY, L"transpose(matrix2)", static_cast<int>(wcslen(L"transpose(matrix2)")));
				printMatrix(mDC, gStartX + ((gCol + 1) * gColWidth) * 3, gStartY + 20, resultMatrix2);
			}
		}

		// ... (여기에 그리기 작업을 수행)

		BitBlt(hDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, mDC, 0, 0, SRCCOPY);

		DeleteDC(mDC);
		DeleteObject(hBitmap);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(hWnd, iMsg, wParam, lParam);
}

array<array<int, gRow>, gCol> genRandMatrix(const int row, const int col)
{
	array<array<int, gRow>, gCol> matrix{};
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			matrix[i][j] = num(gen);
		}
	}
	return matrix;
}

void printMatrix(HDC hDC, const int startX, const int startY, const array<array<int, gRow>, gCol>& matrix)
{
	int currentY = startY;

	for (int r = 0; r < gCol; ++r) {
		for (int c = 0; c < gRow; ++c) {
			wstring valStr = to_wstring(matrix[r][c]);

			int currentX = startX + (c * gColWidth);
			TextOut(hDC, currentX, currentY, valStr.c_str(), static_cast<int>(valStr.length()));
		}
		currentY += gLineHeight; // 다음 행으로 이동
	}
}

array<array<int, gRow>, gCol> addMatrix(const array<array<int, gRow>, gCol>& matrix1, const array<array<int, gRow>, gCol>& matrix2)
{
	array<array<int, gRow>, gCol> result{};
	for (int i = 0; i < gRow; ++i) {
		for (int j = 0; j < gCol; ++j) {
			result[i][j] = matrix1[i][j] + matrix2[i][j];
		}
	}
	return result;
}

array<array<int, gRow>, gCol> subMatrix(const array<array<int, gRow>, gCol>& matrix1, const array<array<int, gRow>, gCol>& matrix2)
{
	array<array<int, gRow>, gCol> result{};
	for (int i = 0; i < gRow; ++i) {
		for (int j = 0; j < gCol; ++j) {
			result[i][j] = matrix1[i][j] - matrix2[i][j];
		}
	}

	return result;
}

array<array<int, gRow>, gCol> mulMatrix(const array<array<int, gRow>, gCol>& matrix1, const array<array<int, gRow>, gCol>& matrix2)
{
	array<array<int, gRow>, gCol> result{};
	for (int i = 0; i < gRow; ++i) {
		for (int j = 0; j < gCol; ++j) {
			for (int k = 0; k < gCol; ++k) {
				result[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
	return result;
}

int get3x3Determinant(const array<array<int, 3>, 3>& matrix) {
	return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
		- matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
		+ matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

int get4x4Determinant(const array<array<int, 4>, 4>& matrix) {
	int determinant{};
	int sign{1};

	// 첫 번째 행(matrix[0][i])을 기준으로 여인수 전개
	for (int col = 0; col < 4; ++col) {
		array<array<int, 3>, 3> subMatrix{};

		// 0번 행과 col번 열을 제외한 3x3 소행렬 추출
		for (int r = 1; r < 4; ++r) 
		{
			int subCol{};
			for (int c = 0; c < 4; ++c) {
				if (c == col) continue; // 선택된 열은 제외
				subMatrix[r - 1][subCol++] = matrix[r][c];
			}
		}

		// 행렬식 누적: (+1, -1, +1, -1) * matrix[0][col] * det(subMatrix)
		determinant += sign * matrix[0][col] * get3x3Determinant(subMatrix);
		sign = -sign; // 부호 반전
	}

	return determinant;
}

array<array<int, gRow>, gCol> transMatrix(const array<array<int, gRow>, gCol>& matrix)
{
	array<array<int, gRow>, gCol> transposed{};
	for (int i = 0; i < gRow; ++i) {
		for (int j = 0; j < gCol; ++j) {
			transposed[j][i] = matrix[i][j];
		}
	}
	return transposed;
}