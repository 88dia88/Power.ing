#define _USE_MATH_DEFINES
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include <math.h>

#define Stage_radius 400
#define circle_radius 25

#define TheX 1900
#define TheY 1000

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
		| WS_HSCROLL | WS_VSCROLL | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 0, 0, TheX, TheY, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

static float Pibot_x = TheX/2, Pibot_y = TheY / 2;
struct Power_Orb
{
	//��
	float x, y, speed, angle, size;
	float speedx, speedy;
	int type;
};
struct Power_Reflector
{
	//����
	float angle, speed, size;
	int module[5];
};

struct Power_Orb OrbSpeed(struct Power_Orb Orb)
{
	//�����Լ�
	Orb.speedx = Orb.speed * cos(M_2_PI * Orb.angle);
	Orb.speedy = Orb.speed * sin(M_2_PI * Orb.angle);
	return Orb;
}
struct Power_Orb OrbReflect(struct Power_Orb Orb, struct Power_Reflector Reflector)
{
	//�浹�Լ�
	int Relative = Orb.angle - Reflector.angle;
	if (Relative < 0.125)
		Orb.angle = 0.75 + Relative + Reflector.angle;
	else if (Relative > 0.375)
		Orb.angle = 0.25 + Relative + Reflector.angle;
	else
		Orb.angle = 1 - Relative + Reflector.angle;
	Orb = OrbSpeed(Orb);
	return Orb;
}
int ReflectDetect(struct Power_Orb Orb, struct Power_Reflector Reflector)
{
	float fAngle;
	fAngle = atan2(Pibot_y - Orb.y, Pibot_x - Orb.x) / M_2_PI;
	if (Reflector.angle + 1 / Reflector.size > fAngle && Reflector.angle - 1 / Reflector.size < fAngle)
		OrbReflect(Orb, Reflector);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	srand((int)time(NULL));

	HDC hdc;
	PAINTSTRUCT ps;

	static struct Power_Orb orb;
	orb.x = Pibot_x, orb.y = Pibot_y, orb.angle = 0.125, orb.speed = 2, orb.size=50;
	orb = OrbSpeed(orb);
	static struct Power_Reflector reflector;
	reflector.angle = 0, reflector.speed = 1, reflector.size = 16;

	static float circle_x = 500, circle_y = 500;		// ���� ũ�� 50
	static float speed_x = 2, speed_y = 2;




	// �浹ó��
	if (pow(Stage_radius - circle_radius, 2) < (pow(circle_x - Pibot_x, 2) + pow(circle_y - Pibot_y, 2))) {
		speed_x = speed_x * -1;		// ((rand() % 21 - 10) / 10)
		speed_y = speed_y * -1;
	}



	if (pow(Stage_radius - circle_radius, 2) < (pow(circle_x, 2) + pow(circle_y, 2))) {
		speed_x = speed_x * -1;		// ((rand() % 21 - 10) / 10)
		speed_y = speed_y * -1;
	}




	//�浹�Լ� orb = OrbReflect(orb, reflector);

	// �ؾ��Ұ� 
	// 1. ������ ���� �г� �����
	// 2. ������ �ִ� �гο� ������ �ִ� �� ƨ���

	//--- �޽��� ó���ϱ�
	switch (iMsg) {
	case WM_CREATE:

		break;

	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			SetTimer(hWnd, 1, 10, NULL);
		}

		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			circle_x += speed_x;
			circle_y += speed_y;

			break;
		}
		InvalidateRect(hWnd, NULL, true);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Ellipse(hdc, Pibot_x - Stage_radius, Pibot_y - Stage_radius, Pibot_x + Stage_radius, Pibot_y + Stage_radius);
		Ellipse(hdc, circle_x - 25 + Pibot_x, circle_y - 25, circle_x + 25, circle_y + 25);


		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam); //--- ���� �� �޽��� ���� ������ �޽����� OS��
}