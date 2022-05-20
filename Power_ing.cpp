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
#define Pibot_x window_size_x / 2
#define Pibot_y window_size_y / 2
#define Reactor_radius window_size * 375
#define Reflector_size_x window_size * 270
#define Reflector_size_y window_size * 68
#define Orb_size window_size * 50
#define Orb_radius Orb_size / 2

#define debug true
#define keyboard true

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

int Cherenkov, Age;
double Score;

struct Power_Orb
{
	double x, y, speed, angle, size;
	double speedx, speedy;
	int type, effect;
};
struct Power_Reflector
{
	double angle, position, speed, size;
	int module[5], age, direction;
};

double AngleOverflow(double Angle)
{
	if (Angle >= 1)
		Angle--;
	else if (Angle < 0)
		Angle++;
	return Angle;
}
double AnglePosition(int x, int y)
{
	return atan2(-y, -x) / M_PI / 2 + 0.5;
}
double DistancePosition(double x, double y)
{
	return (x * x) + (y * y);
}
int Distancecmp(double x, double y, double dis)
{
	if (DistancePosition(x, y) < dis * dis)
		return 1;
	else if (DistancePosition(x, y) == dis * dis)
		return 0;
	else if (DistancePosition(x, y) > dis * dis)
		return -1;
}

double OrbScore(struct Power_Orb Orb)
{
	if (Cherenkov == true) return Orb.speed * 40;
	else return	Orb.speed * 20;
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
	Score += OrbScore(Orb);
	Age += 50;
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
	Score = 0, Cherenkov = false;
	Orb.x = 0, Orb.y = 0, Orb.angle = 0.75, Orb.speed = 5, Orb.size = 50, Orb.type = 0;
	Orb = OrbSpeed(Orb);
	return Orb;
}

struct Power_Orb ReactorReflect(struct Power_Orb Orb)
{
	double Angle = AnglePosition(Orb.x, Orb.y);
	double Relative = AngleOverflow(Orb.angle - Angle);

	Relative = 0.5 - Relative;

	Orb.angle = AngleOverflow(Relative + Angle);
	Orb = OrbSpeed(Orb);
	return Orb;
}
struct Power_Orb ReflectDetect(struct Power_Orb Orb, struct Power_Reflector Reflector)
{
	double Angle = AnglePosition(Orb.x, Orb.y);

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
	Reflector.angle = 0.75, Reflector.position = 1, Reflector.speed = 1, Reflector.size = 0.1, Reflector.age = 0, Reflector.direction = 1;
	Reflector.module[0] = 0, Reflector.module[1] = 0, Reflector.module[2] = 0, Reflector.module[3] = 1, Reflector.module[4] = 0;
	return Reflector;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	srand((int)time(NULL));

	PAINTSTRUCT ps;

	HDC hdc;
	static HDC mem1dc, mem2dc;
	static HBITMAP hBitmap, BitReactor, BitReflector, BitOrb;
	HBITMAP oldBit1, oldBit2;
	RECT win_rect;
	
	LPPOINT lpPoint = NULL;
	TCHAR lpOut[100];

	POINTS Mouse = MAKEPOINTS(lParam);

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
		BitReactor = (HBITMAP)LoadImage(g_hInst, TEXT("Reactor_750.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);			// 1000 X 1000
		BitReflector = (HBITMAP)LoadImage(g_hInst, TEXT("Reflector_750.bmp"), IMAGE_BITMAP, 0, 0,
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
	case WM_KEYUP:
		if (keyboard == true)
		{
			if (wParam == VK_RIGHT || wParam == VK_LEFT) {
				if (reflector.angle > 0.05 && reflector.angle < 0.45) reflector.direction = -1;
				else if (reflector.angle > 0.55 && reflector.angle < 0.95) reflector.direction = 1;
			}
		}
		break;
	case WM_MOUSEMOVE:
		if (keyboard == false)
		{
			Mouse = MAKEPOINTS(lParam);
			reflector.angle = AnglePosition(Mouse.x - Pibot_x, - Mouse.y + Pibot_y);
		}
	case WM_TIMER:
		switch (wParam) {
		case 0:				//GetAsyncKeyState - 키가 눌린 상태로 진행되는함수 (끊김없이)http://www.silverwolf.co.kr/cplusplus/4842
			if (keyboard == true)
			{
				if (GetAsyncKeyState(VK_RIGHT) & 0x8001) {

					if (GetAsyncKeyState(VK_SHIFT) & 0x8001) {
						reflector.angle += 0.0025 * reflector.speed * reflector.position * reflector.direction;
					}
					else
					{
						reflector.angle += 0.005 * reflector.speed * reflector.position * reflector.direction;
					}
					reflector.angle = AngleOverflow(reflector.angle);
				}
				else if (GetAsyncKeyState(VK_LEFT) & 0x8001) {
					if (GetAsyncKeyState(VK_SHIFT) & 0x8001) {
						reflector.angle -= 0.0025 * reflector.speed * reflector.position * reflector.direction;
					}
					else
					{
						reflector.angle -= 0.005 * reflector.speed * reflector.position * reflector.direction;
					}
					reflector.angle = AngleOverflow(reflector.angle);
				}
				if (reflector.module[3] == 0)
				{
					if (GetAsyncKeyState(VK_UP) & 0x8001 || GetAsyncKeyState(VK_SPACE) & 0x8001 && reflector.position < 1.55 && Age < 49) {
						if (reflector.position < 1.5) reflector.position += 0.05;
						reflector.position += 0.05;
					}
					else if (GetAsyncKeyState(VK_DOWN) & 0x8001 || GetAsyncKeyState(VK_CONTROL) & 0x8001 && reflector.position > 0.825 && Age < 49) {
						if (reflector.position > 0.875) reflector.position -= 0.05;
						reflector.position -= 0.05;
					}
				}
			}
			if (reflector.position > 1)	reflector.position -= 0.05;
			if (reflector.position < 1)	reflector.position += 0.05;
			break;
		case 1:
			if (pow(500, 2) > (pow(orb.x, 2) + pow(orb.y, 2))) {
				if (Distancecmp(orb.x, orb.y, 316 / reflector.position) <= 0 && Distancecmp(orb.x, orb.y, 375 / reflector.position) > 0 && Age <= 0) {
					orb = ReflectDetect(orb, reflector);
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
			if (Age > 0)	Age--;

			orb = OrbPosition(orb);

			break;
		}
		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(mem2dc, BitReactor );
		BitBlt(mem1dc, Pibot_x - window_size_y / 2, Pibot_y - window_size_y / 2, window_size_y + 15, window_size_y + 15, mem2dc, 0, 0, SRCCOPY);
		
		SelectObject(mem2dc, BitReflector);
		BitBlt(mem1dc, Pibot_x + (Reactor_radius / reflector.position) * cos(M_PI * 2 * reflector.angle) - Reflector_size_x / 2, Pibot_y + (Reactor_radius / reflector.position) * -sin(M_PI * 2 * reflector.angle) - Reflector_size_y / 2, Reflector_size_x, Reflector_size_y, mem2dc, 0, 0, SRCCOPY);
		
		SelectObject(mem2dc, BitOrb);
		BitBlt(mem1dc, Pibot_x + orb.x * window_size - Orb_radius, Pibot_y - orb.y * window_size - Orb_radius, Orb_size, Orb_size, mem2dc, 0, 0, SRCCOPY);
		
		BitBlt(hdc, 0, 0, window_size_x, window_size_y, mem1dc, 0, 0, SRCCOPY);

		if (debug == true)
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
			swprintf(lpOut, 100, L"age : %d ", Age);
			TextOut(hdc, 0, 75, lpOut, lstrlen(lpOut));
			swprintf(lpOut, 100, L"score : %.0f ", Score);
			TextOut(hdc, 0, 100, lpOut, lstrlen(lpOut));
		}

		EndPaint(hWnd, &ps);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}
