/*
	x1 = (Reactor_radius - Reflector_size_y / 2) * cos(M_PI * 2 * reflector.angle) + (Reflector_size_x / 2) * sin(M_PI * 2 * Orb.angle);
	y1 = (Reactor_radius - Reflector_size_y / 2) * sin(M_PI * 2 * reflector.angle) + (Reflector_size_x / 2) * cos(M_PI * 2 * Orb.angle);
	x2 = (Reactor_radius-Reflector_size_y / 2) * cos(M_PI * 2 * reflector.angle) - (Reflector_size_x / 2) * sin(M_PI * 2 * Orb.angle);
	y2 = (Reactor_radius - Reflector_size_y / 2) * sin(M_PI * 2 * reflector.angle) - (Reflector_size_x / 2) * cos(M_PI * 2 * Orb.angle);
	x3 = (Reactor_radius + Reflector_size_y / 2) * cos(M_PI * 2 * reflector.angle) - (-Reflector_size_x / 2) * sin(M_PI * 2 * Orb.angle);
	y3 = (Reactor_radius + Reflector_size_y / 2) * sin(M_PI * 2 * reflector.angle) - (-Reflector_size_x / 2) * cos(M_PI * 2 * Orb.angle);
*/

#define _USE_MATH_DEFINES
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <atlImage.h>

#define window_size 0.75
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
	int type, effect;
};
struct Power_Reflector
{
	double angle, position, speed, size;
	int module[5], age;
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
	
	if (Relative > 0.125 && Relative <= 0.25) Relative += 0.25;
	else if (Relative < 0.875 && Relative >= 0.75) Relative -= 0.25;
	else Relative = 0.5 - Relative;

	Orb.angle = AngleOverflow(Relative + Reflector.angle);
	Orb.x += Orb.speed * cos(M_PI * 2 * Orb.angle);
	Orb.y += Orb.speed * sin(M_PI * 2 * Orb.angle);
	Orb = OrbSpeed(Orb);
	return Orb;
}
struct Power_Orb OrbReset(struct Power_Orb Orb)
{
	Orb.x = 0, Orb.y = 0, Orb.angle = 0.75, Orb.speed = 5, Orb.size = 50, Orb.type = 0;
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

struct Power_Reflector ReflectorReset(struct Power_Reflector Reflector)
{
	Reflector.angle = 0.75, Reflector.position = 0, Reflector.speed = 1, Reflector.size = 0.1, Reflector.age = 0;
	Reflector.module[0] = 0, Reflector.module[1] = 0, Reflector.module[2] = 0, Reflector.module[3] = 0, Reflector.module[4] = 0;
	return Reflector;
}
struct Power_Reflector AgeDetect(struct Power_Orb Orb, struct Power_Reflector Reflector)
{
	double Angle;
	Angle = atan2(-Orb.y, -Orb.x) / M_PI / 2 + 0.5;

	if (Reflector.angle + Reflector.size >= 1)
	{
		if (Reflector.angle - Reflector.size < Angle || AngleOverflow(Reflector.angle + Reflector.size) > Angle)
			Reflector.age += 50;
	}
	else if (Reflector.angle - Reflector.size < 0)
	{
		if (Reflector.angle + Reflector.size > Angle || AngleOverflow(Reflector.angle - Reflector.size) < Angle)
			Reflector.age += 50;
	}
	else if (Reflector.angle + Reflector.size > Angle && Reflector.angle - Reflector.size < Angle)
		Reflector.age += 50;
	return Reflector;
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
	int debugmod = 1;

	static struct Power_Orb orb;
	static struct Power_Reflector reflector;

	// 해야할거 
	// 1. 각도에 따른 패널 만들기

	//--- 메시지 처리하기
	switch (iMsg) {
	case WM_CREATE:

		orb = OrbReset(orb);
		reflector = ReflectorReset(reflector);

		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &win_rect);

		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		hBitmap = CreateCompatibleBitmap(hdc, win_rect.right, win_rect.bottom);
		SelectObject(mem1dc, (HBITMAP)hBitmap);
		FillRect(mem1dc, &win_rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		BitStage = (HBITMAP)LoadImage(g_hInst, TEXT("Reactor_750.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);			// 1000 X 1000
		BitPanel = (HBITMAP)LoadImage(g_hInst, TEXT("Reflector_750.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);			// 270 X 68
		BitOrb = (HBITMAP)LoadImage(g_hInst, TEXT("Orb_750.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		SetTimer(hWnd, 0, 10, NULL);

		ReleaseDC(hWnd, hdc);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			SetTimer(hWnd, 1, 10, NULL);
		}
		if (reflector.module[3] == 1)
		{
			if (wParam == VK_UP) {
				if (reflector.speed < 2) reflector.speed += 0.5;
			}
			else if (wParam == VK_DOWN) {
				if (reflector.speed > 0) reflector.speed -= 0.5;
			}
		}
		break;

	case WM_TIMER:
		switch (wParam) {
		case 0:				//GetAsyncKeyState - 키가 눌린 상태로 진행되는함수 (끊김없이)
			if (GetAsyncKeyState(VK_RIGHT) & 0x8001) {		//http://www.silverwolf.co.kr/cplusplus/4842
				
				if (GetAsyncKeyState(VK_SHIFT) & 0x8001) {
					reflector.angle += 0.0025 * reflector.speed;
				}
				else
				{
					reflector.angle += 0.005 * reflector.speed;
				}
				reflector.angle = AngleOverflow(reflector.angle);
			}
			else if (GetAsyncKeyState(VK_LEFT) & 0x8001) {
				if (GetAsyncKeyState(VK_SHIFT) & 0x8001) {
					reflector.angle -= 0.0025 * reflector.speed;
				}
				else
				{
					reflector.angle -= 0.005 * reflector.speed;
				}
				reflector.angle = AngleOverflow(reflector.angle);
			}
			if (reflector.module[3] == 0)
			{
				if (GetAsyncKeyState(VK_UP) & 0x8001 && reflector.position < 60 && reflector.age < 25) {
					if (reflector.position < 50) reflector.position += 5;
					reflector.position += 10;
				}
				else if (GetAsyncKeyState(VK_DOWN) & 0x8001 && reflector.position > -60 && reflector.age < 25) {
					if (reflector.position > -50) reflector.position -= 5;
					reflector.position -= 10;
				}
			}
			if (reflector.position > 0)	reflector.position -= 10;
			if (reflector.position < 0)	reflector.position += 10;
			break;
		case 1:
			if (pow(500, 2) > (pow(orb.x, 2) + pow(orb.y, 2))) {
				if (pow(316 - reflector.position, 2) <= (pow(orb.x, 2) + pow(orb.y, 2)) && pow(375 - reflector.position, 2) > (pow(orb.x, 2) + pow(orb.y, 2)) && reflector.age <= 0) {
					orb = ReflectDetect(orb, reflector);
					reflector = AgeDetect(orb, reflector);
				}
			}
			else if (orb.type == 1 || orb.effect == 1)
			{
				orb = ReactorReflect(orb);
				if (orb.effect == 1) orb.effect = 0;
			}
			else
			{
				orb = OrbReset(orb);
			}
			if (reflector.age > 0)	reflector.age--;

			orb = OrbPosition(orb);

			break;
		}
		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (debugmod == 1)
		{
			MoveToEx(mem1dc, Pibot_x, 0, lpPoint);
			LineTo(mem1dc, Pibot_x, window_size_y);
			MoveToEx(mem1dc, 0, Pibot_y, lpPoint);
			LineTo(mem1dc, window_size_x, Pibot_y);

			swprintf(lpOut, 100, L"orb : %g ", orb.angle);
			TextOut(hdc, 0, 0, lpOut, lstrlen(lpOut));
			swprintf(lpOut, 100, L"relative : %g ", atan2(-orb.y, -orb.x) / M_PI / 2 + 0.5);
			TextOut(hdc, 0, 25, lpOut, lstrlen(lpOut));
			swprintf(lpOut, 100, L"reflector : %g ", reflector.angle);
			TextOut(hdc, 0, 50, lpOut, lstrlen(lpOut));
			swprintf(lpOut, 100, L"age : %d ", reflector.age);
			TextOut(hdc, 0, 75, lpOut, lstrlen(lpOut));
		}

		SelectObject(mem2dc, BitStage);
		BitBlt(mem1dc, Pibot_x - window_size_y / 2, Pibot_y - window_size_y / 2, window_size_y + 15, window_size_y + 15, mem2dc, 0, 0, SRCCOPY);
		SelectObject(mem2dc, BitPanel);

		BitBlt(mem1dc, Pibot_x + (Stage_radius - reflector.position) * cos(M_PI * 2 * reflector.angle) - Panel_size_x / 2, Pibot_y + (Stage_radius - reflector.position) * -sin(M_PI * 2 * reflector.angle) - Panel_size_y / 2, Panel_size_x, Panel_size_y, mem2dc, 0, 0, SRCCOPY);
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
