#define _USE_MATH_DEFINES
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <atlImage.h>

#define window_size 1
#define window_size_x window_size * 1000 //1900
#define window_size_y window_size * 1000
#define Stage_radius window_size_y * 0.375
#define Panel_size_x window_size_y * 0.27
#define Panel_size_y window_size_y * 0.068
#define Orb_size window_size_y * 0.05
#define Orb_radius Orb_size / 2

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
		| WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 0, 0, window_size_x + 15, window_size_y + 40,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

static float Pibot_x = window_size_x / 2, Pibot_y = window_size_y / 2;
struct Power_Orb
{
	double x, y, speed, angle, size;
	double speedx, speedy;
	int type;
};
struct Power_Reflector
{
	double angle, speed, size;
	int module[5];
};

double AngleOverflow(double Angle)
{
	if (Angle >= 1)
		Angle--;
	else if (Angle < 0)
		Angle++;
	return Angle;
}

struct Power_Orb OrbPosition(struct Power_Orb Orb)
{
	Orb.x += Orb.speedx;
	Orb.y += Orb.speedy;
	return Orb;
}
struct Power_Orb OrbSpeed(struct Power_Orb Orb)
{
	Orb.speedx = Orb.speed * cos(M_PI * 2 * Orb.angle);
	Orb.speedy = Orb.speed * sin(M_PI * 2 * Orb.angle);
	return Orb;
}
struct Power_Orb OrbReflect(struct Power_Orb Orb, struct Power_Reflector Reflector)
{
	double Relative = AngleOverflow(Orb.angle - Reflector.angle);
	
	/*
	if (Relative > 0.125 && Relative < 0.5) {Relative -= 0.25;}
	else {Relative = 0.5 - Relative;}
	*/
	
	Relative = 0.5 - Relative;

	Orb.angle = AngleOverflow(Relative + Reflector.angle);
	Orb = OrbSpeed(Orb);
	return Orb;
}

struct Power_Orb ReactorReflect(struct Power_Orb Orb)
{
	double Angle;
	Angle = atan2(-Orb.y, -Orb.x) / M_PI / 2 + 0.5;
	double Relative = AngleOverflow(Orb.angle - Angle);

	Relative = 0.5 - Relative;

	Orb.angle = AngleOverflow(Relative + Angle);
	Orb = OrbSpeed(Orb);
	return Orb;
}

struct Power_Orb ReflectDetect(struct Power_Orb Orb, struct Power_Reflector Reflector)
{
	double Angle;
	Angle = atan2(-Orb.y, -Orb.x) / M_PI / 2 + 0.5;

	if (Reflector.angle + Reflector.size >= 1)
	{
		if (Reflector.angle - Reflector.size < Angle || AngleOverflow(Reflector.angle + Reflector.size) > Angle)
			Orb = OrbReflect(Orb, Reflector);
	}
	else if (Reflector.angle - Reflector.size < 0)
	{
		if (Reflector.angle + Reflector.size > Angle || AngleOverflow(Reflector.angle - Reflector.size) < Angle)
			Orb = OrbReflect(Orb, Reflector);
	}
	else if (Reflector.angle + Reflector.size > Angle && Reflector.angle - Reflector.size < Angle)
		Orb = OrbReflect(Orb, Reflector);
	return Orb;
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
	
	LPPOINT lpPoint = NULL;
	TCHAR lpOut[100];


	static struct Power_Orb orb;
	static struct Power_Reflector reflector;

	// 해야할거 
	// 1. 각도에 따른 패널 만들기

	//--- 메시지 처리하기
	switch (iMsg) {
	case WM_CREATE:

		orb.x = 0, orb.y = 0, orb.angle = 0.75, orb.speed = 3, orb.size = 50;
		orb = OrbSpeed(orb);
		reflector.angle = 0.75, reflector.speed = 2, reflector.size = 0.1;

		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &win_rect);

		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		hBitmap = CreateCompatibleBitmap(hdc, win_rect.right, win_rect.bottom);
		SelectObject(mem1dc, (HBITMAP)hBitmap);
		FillRect(mem1dc, &win_rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
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
		else if (wParam == VK_LEFT) {
			reflector.angle -= 0.005 * reflector.speed;
			reflector.angle = AngleOverflow(reflector.angle);
		}
		else if (wParam == VK_RIGHT) {
			reflector.angle += 0.005 * reflector.speed;
			reflector.angle = AngleOverflow(reflector.angle);
		}
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (pow(500, 2) > (pow(orb.x, 2) + pow(orb.y, 2))) {
				if (pow(316, 2) <= (pow(orb.x, 2) + pow(orb.y, 2)) && pow(375, 2) > (pow(orb.x, 2) + pow(orb.y, 2))) {
					orb = ReflectDetect(orb, reflector);
				}
			}
			else{
				orb.x = 0, orb.y = 0, orb.angle = 0.75;
				orb = OrbSpeed(orb);
			}
			
			/*{
				orb = ReactorReflect(orb);
			}
			*/
			
			orb = OrbPosition(orb);

			break;
		}
		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MoveToEx(mem1dc, Pibot_x, 0, lpPoint);
		LineTo(mem1dc, Pibot_x, window_size_y);
		MoveToEx(mem1dc, 0, Pibot_y, lpPoint);
		LineTo(mem1dc, window_size_x, Pibot_y);
		
		///*
		swprintf(lpOut, 100, L"orb : %g ", orb.angle);
		TextOut(hdc, 0, 0, lpOut, lstrlen(lpOut));
		swprintf(lpOut, 100, L"relative : %g ", atan2(-orb.y, -orb.x) / M_PI / 2 + 0.5);
		TextOut(hdc, 0, 25, lpOut, lstrlen(lpOut));
		swprintf(lpOut, 100, L"reflector : %g ", reflector.angle);
		TextOut(hdc, 0, 50, lpOut, lstrlen(lpOut));
		//*/

		SelectObject(mem2dc, BitStage);
		BitBlt(mem1dc, Pibot_x - window_size_y / 2, Pibot_y - window_size_y / 2, window_size_y + 15, window_size_y + 15, mem2dc, 0, 0, SRCCOPY);
		SelectObject(mem2dc, BitPanel);

		BitBlt(mem1dc, Pibot_x + Stage_radius * cos(M_PI * 2 * reflector.angle) - Panel_size_x / 2, Pibot_y + Stage_radius * -sin(M_PI * 2 * reflector.angle) - Panel_size_y / 2, Panel_size_x, Panel_size_y, mem2dc, 0, 0, SRCCOPY);
		SelectObject(mem2dc, BitOrb);
		BitBlt(mem1dc, Pibot_x + orb.x * window_size - Orb_radius, Pibot_y - orb.y * window_size - Orb_radius, Orb_size, Orb_size, mem2dc, 0, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, window_size_x, window_size_y, mem1dc, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}
