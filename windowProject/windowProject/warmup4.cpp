#include <Windows.h>

using namespace std;

// 윈도우 크기 및 스타일 정의
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_STYLE WS_OVERLAPPED | WS_MINIMIZEBOX | WS_BORDER | WS_SYSMENU

// 그리기 시작 위치
const int gStartX{ 50 };
const int gStartY{ 30 };

HINSTANCE g_hinst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"File Reader";

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

	switch (iMsg)
	{
	case WM_CREATE:
	{
		
		break;
	}
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'A':
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'b':
			break;
		case 'C':
			break;
		case 'D':
			break;
		case 'E':
			break;
		case 'F':
			break;
		case 'G':
			break;
		case 'H':
			break;
		case 'I':
			break;
		case 'J':
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