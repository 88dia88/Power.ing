#include "Power_ing.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//--------------------------------------------------------------------------------------------------------------//
	srand((int)time(NULL));
	PAINTSTRUCT ps;

	POINTS Mouse = MAKEPOINTS(lParam);

	//PlgBlt = 회전하기 위한 3개(좌상,우상,좌하)의 좌표 필요

	//--------------------------------------------------------------------------------------------------------------//
	// 해야할거 
	// 
	// ------------------------------ 안어려움
	// 
	// || 충돌 A || 시작 A ||
	// 
	// 벙커 개패 A
	// 
	// 원소 UI || 공 종류 UI
	// 
	// ------------------------------ 어려울 수 있음
	//
	// 온도 UI
	// 
	// 공 트레일 *
	// 
	// 공 종류  *
	// 
	// ------------------------------ 어려움
	// 
	// 게임 시작 / 오버 UI -----------
	// 
	// -----------
	// 공 여러개
	// 아이템
	// 2.1 spliter
	// -----------
	// 
	// 2.2 launcher
	// 4.1 reserver, 4.2
	// 
	// ------------------------------ 벽돌깨기
	// 
	//--------------------------------------------------------------------------------------------------------------//메시지 처리하기

	switch (iMsg) {
	case WM_CREATE:
		GameStart = false;
		EffectHead->next = EffectHead;
		OrbHead->next = OrbHead;
		ReflectorHead = ReflectorReset(ReflectorHead);
		ReflectorHead->next = ReflectorHead;
		ReflectorCreate(ReflectorHead, 0);
		GeneralReset();
		DisplayLoad();
		SetTimer(hWnd, 0, 10, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			if (Time == -1)
			{
				SetTimer(hWnd, 3, 10, NULL);
			}
			else if (Time == 0 && PressureCheck())
			{
				OrbCreate(OrbHead, 0, 0, 0, 0.25);
				GameStart = true;
			}
			if (Cherenkov.meter >= 875 && Cherenkov.lever < 6 && Cherenkov.cherenkov == false)
			{
				Cherenkov.lever++;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		Mouse = MAKEPOINTS(lParam);
		if (Mouse.y >= Pibot_y + (251 * window_size) && Mouse.y <= Pibot_y + (301 * window_size))
		{
			if (Mouse.x >= Pibot_x + (653.5 * window_size) && Mouse.x <= Pibot_x + (703.5 * window_size)) Button[3] = 10;
			else if (Mouse.x >= Pibot_x + (796.5 * window_size) && Mouse.x <= Pibot_x + (846.5 * window_size)) Button[4] = 10;
		}
		if (Time == 0 && Mouse.y >= Pibot_y + (240 * window_size) && Mouse.y <= Pibot_y + (340 * window_size))
		{
			if (Mouse.x > Pibot_x - (821.5 * window_size) && Mouse.x <= Pibot_x - (781.5 * window_size)) Button[1] = 1;
			else if (Mouse.x >= Pibot_x - (861.5 * window_size) && Mouse.x < Pibot_x - (821.5 * window_size)) Button[1] = -1;
			else if (Mouse.x > Pibot_x - (678.5 * window_size) && Mouse.x <= Pibot_x - (638.5 * window_size)) Button[2] = 1;
			else if (Mouse.x >= Pibot_x - (718.5 * window_size) && Mouse.x < Pibot_x - (678.5 * window_size)) Button[2] = -1;
		}
		break;
	case WM_LBUTTONUP:
		Button[1] = 0;
		Button[2] = 0;
		break;
	case WM_MOUSEMOVE:
		if (Button[1] != 0)
		{
			Mouse = MAKEPOINTS(lParam);
			if (Mouse.x < Pibot_x - 178.5 * window_size)
			{
				if (Mouse.x > Pibot_x - 821.5 * window_size) Button[1] = (int)(DistancePosition(Mouse.x - (Pibot_x - 821.5 * window_size), Mouse.y - (Pibot_x - 300 * window_size)) / 12.5) + 1;
				else Button[1] = -(int)(DistancePosition(Mouse.x - (Pibot_x - 821.5 * window_size), Mouse.y - (Pibot_x - 300 * window_size)) / 12.5) - 1;
			}
		}
		if (Button[2] != 0)
		{
			Mouse = MAKEPOINTS(lParam);
			if (Mouse.x < Pibot_x - 321.5 * window_size)
			{
				if (Mouse.x > Pibot_x - 678.5 * window_size) Button[2] = (int)(DistancePosition(Mouse.x - (Pibot_x - 678.5 * window_size), Mouse.y - (Pibot_x - 300 * window_size)) / 25) + 1;
				else Button[2] = -(int)(DistancePosition(Mouse.x - (Pibot_x - 678.5 * window_size), Mouse.y - (Pibot_x - 300 * window_size)) / 25) - 1;
			}
		}
		if (keyboard == false)
		{
			Mouse = MAKEPOINTS(lParam);
			//Reflector->angle = AnglePosition(Mouse.x - Pibot_x, Mouse.y - Pibot_y);
		}
		break;
	case WM_TIMER:
		switch (wParam) {
		case 0:				//GetAsyncKeyState - 키가 눌린 상태로 진행되는함수 (끊김없이)http://www.silverwolf.co.kr/cplusplus/4842
			if (keyboard)	ReflectorPosition(ReflectorHead, (GetAsyncKeyState(VK_LEFT) & 0x8001), (GetAsyncKeyState(VK_RIGHT) & 0x8001), (GetAsyncKeyState(VK_UP) & 0x8001 || GetAsyncKeyState(VK_SPACE) & 0x8001), (GetAsyncKeyState(VK_DOWN) & 0x8001 || GetAsyncKeyState(VK_SHIFT) & 0x8001));
			ButtonActive();
			if (GameStart)
			{
				Time++;
				CherenkovCheck();
				CollisionDetect(OrbHead);
				if (ReactorEffect > 5) GameOver();
			}
			break;
		case 1:
			break;
		case 3:
			if (ReactorEffect > 6) GameRestart();
			else if (ReactorEffect == 6)
			{
				GeneralReset();
				KillTimer(hWnd, 3);
			}
		}
		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		//-----------------------------------------------------------------------------------------------------------------------------//

		memdc = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, (int)window_size_x, (int)window_size_y);

		SelectObject(memdc, hBitmap);
		SetStretchBltMode(memdc, COLORONCOLOR);

		ReactorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
		Reactor_EffectImg.Draw(memdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size* (ReactorEffect % 6), Reactor_size* (int)(ReactorEffect / 6), Reactor_size, Reactor_size);

		Cherenkov_LeverImg.Draw(memdc, Pibot_x - (740 * window_size), Pibot_y - (100 * window_size), 200 * window_size, 200 * window_size, 200 * Cherenkov.lever, 0, 200, 200);

		Button_PressureImg.Draw(memdc, Pibot_x - (861.5 * window_size), Pibot_y + (260 * window_size), 80 * window_size, 80 * window_size, 80 * (PNcmp(Button[1]) + 1), 0, 80, 80);
		Button_PressureImg.Draw(memdc, Pibot_x - (718.5 * window_size), Pibot_y + (260 * window_size), 80 * window_size, 80 * window_size, 80 * (PNcmp(Button[2]) + 1), 0, 80, 80);
		Button_OrbImg.Draw(memdc, Pibot_x + (653.5 * window_size), Pibot_y + (251 * window_size), 50 * window_size, 50 * window_size, 50 * (int)(Button[3] / 3), 0, 50, 50);
		Button_OrbImg.Draw(memdc, Pibot_x + (796.5 * window_size), Pibot_y + (251 * window_size), 50 * window_size, 50 * window_size, 50 * (int)(Button[4] / 3), 50, 50, 50);

		DisplayRotatedImage(-860, 0, 20, 110, Cherenkov.meter / 1500.0 - 1.0 / 3.0, 1);
		DisplayRotatedImage(-700, -405, 14, 80, PressureCaculate(Mole, Temperture), 2);
		DisplayRotatedImage(-821.5, 300, 80, 80, (Temperture - Kelvin) / (MaxTemp - Kelvin), 3);
		DisplayRotatedImage(-678.5, 300, 80, 80, Mole / MaxPressure * Kelvin / 3 - 1.0 / 6.0, 4);
		DisplayRotatedImage(-825, -355, 15, 190, 0, 5);

		DisplayOrb(OrbHead);
		DisplayReflector(ReflectorHead);

		SetBkMode(memdc, TRANSPARENT);
		ScoreUI();
		EffectPrint(EffectHead);
		if (debug == true)	DebugInfo();

		BitBlt(hdc, 0, 0, (int)window_size_x, (int)window_size_y, memdc, 0, 0, SRCCOPY);

		DeleteObject(hBitmap);
		DeleteDC(memdc);

		//-----------------------------------------------------------------------------------------------------------------------------//

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}