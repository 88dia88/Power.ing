#include "Power_ing.h"

double window_size = 0.96;
const bool debug = true, keyboard = true, ChargedMod = false;
int Reflector1Left = 0x25;
int Reflector1Right = 0X27;
int Reflector1Up = 0X26;
int Reflector1Down = 0X28;

int Player1RGB[3] = {255, 255, 0};
int Player1Charge[3] = {1,2,3};

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Power.ing";
LPCTSTR lpszWindowName = L"Power.ing";

HDC hdc, memdc;
HBITMAP hBitmap;

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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_POPUP, 0, 0, (int)(window_size_x), (int)(window_size_y),
		NULL, (HMENU)NULL, hInstance, NULL); //WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX |
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}