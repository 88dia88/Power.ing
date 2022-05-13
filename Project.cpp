#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <atlImage.h>

#define window_size_x 1900
#define window_size_y 1000
#define Stage_radius 375
#define Center_radius 500
#define Panel_size_x 270
#define Panel_size_y 68
#define Orb_size_x 50
#define Orb_size_y 50
#define Orb_radius 25

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 1";

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME
		| WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 0, 0, window_size_x, window_size_y,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	srand((int)time(NULL));

	PAINTSTRUCT ps;

	HDC hdc;
	static HDC mem1dc, mem2dc;
	static HBITMAP hBitmap, BitStage, BitPanel, BitOrb;
	HBITMAP oldBit1, oldBit2;
	RECT win_rect;

	static float Pibot_x = window_size_x / 2, Pibot_y = window_size_y / 2;
	static float Orb_x = Pibot_x, Orb_y = Pibot_y;		// 공의 크기 50
	static float speed_x = 2, speed_y = 2;

	// 충돌처리
	if (pow(Stage_radius - Orb_radius, 2) < (pow(Orb_x - Pibot_x, 2) + pow(Orb_y - Pibot_y, 2))) {
		speed_x = speed_x * -1;		// ((rand() % 21 - 10) / 10)
		speed_y = speed_y * -1;
	}

	// 해야할거 
	// 1. 각도에 따른 패널 만들기
	// 2. 각도가 있는 패널에 각도가 있는 공 튕기기

	//--- 메시지 처리하기
	switch (iMsg) {
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &win_rect);

		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		hBitmap = CreateCompatibleBitmap(hdc, win_rect.right, win_rect.bottom);
		SelectObject(mem1dc, (HBITMAP)hBitmap);

		BitStage = (HBITMAP)LoadImage(g_hInst, TEXT("Stage.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);			// 1000 X 1000
		BitPanel = (HBITMAP)LoadImage(g_hInst, TEXT("Panel.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);			// 270 X 68
		BitOrb = (HBITMAP)LoadImage(g_hInst, TEXT("Orb.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		ReleaseDC(hWnd, hdc);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			SetTimer(hWnd, 1, 10, NULL);
		}

		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			//Orb_x += speed_x;
			Orb_y += speed_y;

			break;
		}

		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		//FillRect(mem2dc, &win_rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		SelectObject(mem2dc, BitStage);
		BitBlt(mem1dc, window_size_x / 2 - 500, window_size_y / 2 - 500, 1000, 1000, mem2dc, 0, 0, SRCCOPY);
		SelectObject(mem2dc, BitPanel);
		BitBlt(mem1dc, 815, 875, Panel_size_x, Panel_size_y, mem2dc, 0, 0, SRCCOPY);
		SelectObject(mem2dc, BitOrb);
		BitBlt(mem1dc, Orb_x - 25, Orb_y - 25, Orb_size_x, Orb_size_y, mem2dc, 0, 0, SRCCOPY);


		BitBlt(hdc, 0, 0, window_size_x, window_size_y, mem1dc, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}
