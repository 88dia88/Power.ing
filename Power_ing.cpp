#include "Power_ing.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//--------------------------------------------------------------------------------------------------------------//
	srand((int)time(NULL));
	PAINTSTRUCT ps;

	POINTS Mouse = MAKEPOINTS(lParam);

	// 추가변수--------
	static bool endscene = false;		// 엔드씬
	// -- 스타트 씬 제작부
	static bool Start_scene = true;		// 스타트 씬
	static bool EscMode = false;
	static bool start_button = false;	// 각 버튼위에 마우스가 올라가 있으면 값이 변경됨
	static bool module_button = false;
	static bool option_button = false;
	static bool quit_button = false;
	// -------------------

	//PlgBlt = 회전하기 위한 3개(좌상,우상,좌하)의 좌표 필요

	//--------------------------------------------------------------------------------------------------------------//
	// 해야할거 
	// 
	// ------------------------------ 안어려움
	// 쿨타임 A - 완료
	// 
	// 체렌코프 발동 A || 게임오버 A - 완료
	// 
	// 충돌 A  - 완료
	// 
	// 시작 A 
	// 
	// 벙커 개패 A - 완료
	// 
	// 원소 UI || 공 종류 UI
	// 
	// 마스크 노가다 - 완료
	// 
	// ------------------------------ 어려울 수 있음
	//
	// 온도 UI - 완료
	// 
	// 공 트레일 (잔상효과) - 완료
	// 
	// 1.1 double 1.2 triple - 완료
	// 
	// 공 종류
	// 
	// ------------------------------ 어려움
	// 
	// 게임 시작 / 오버 UI - 완료
	// 
	// -----------
	// 공 여러개 - 기반 제작 완료
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
		ReflectorHead->next = ReflectorHead;
		ReflectorHead = ReflectorReset(ReflectorHead);
		ReflectorCreate(ReflectorHead, 0);
		GeneralReset();
		PreTime = -25;
		DisplayLoad();

		SetTimer(hWnd, 5, 50, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (EscMode)
			{
				EscMode = false;
				Start_scene = false;
			}
			else if (endscene)
			{
				PreTime = 5;
				endscene = false;
				Start_scene = true;
				KillTimer(hWnd, 0);
				SetTimer(hWnd, 5, 50, NULL);
			}
			else if (Start_scene == false)
			{
				EscMode = true;
				PreTime = 15;
			}
		}
		if (wParam == VK_RETURN) {
			if (Time == -1)
			{
				SetTimer(hWnd, 3, 10, NULL);
			}
			else if (Time == 0 && PressureCheck())
			{
				GameStart = true;
			}
			if (Cherenkov.meter >= 875 && Cherenkov.lever < 6 && Cherenkov.cherenkov == false)
			{
				Cherenkov.lever++;
			}
		}
		break;
	case WM_CHAR:
		if (wParam == 'u') {				// 엔드 씬 추가부
			PreTime == 25;
			SetTimer(hWnd, 100, 50, NULL);
		}
		else if (wParam == 'p') {
			Start_scene = true;				// 스타트씬 강제 표출 디버그용
		}
		break;
	case WM_MOUSEMOVE:
		if (Start_scene)
		{
			Mouse = MAKEPOINTS(lParam);
			start_button = UIButtonSelected(-700, -135, 750, 150, Mouse);
			module_button = UIButtonSelected(-700, 15, 750, 150, Mouse);
			option_button = UIButtonSelected(-700, 165, 750, 150, Mouse);
			quit_button = (!EscMode && UIButtonSelected(-700, 315, 750, 150, Mouse));
		}
		else
		{
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
		}
		break;
	case WM_LBUTTONDOWN:
		Mouse = MAKEPOINTS(lParam);
		if (Start_scene)
		{
			if (start_button) {
				if (EscMode)
				{
					EscMode = false;
					Start_scene = false;
				}
				else
				{
					Start_scene = false;
					PreTime = -25;
				}
			}
			else if (module_button) {
				// 모듈 클릭시 대화상자
				//DialogBoxW(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)&Dlalog_Proc);
			}
			else if (option_button)
			{
				if (EscMode)
				{
					GeneralReset();
					EscMode = false;
					PreTime = 5;
					Start_scene = true;
					KillTimer(hWnd, 0);
					SetTimer(hWnd, 5, 50, NULL);
				}
				// 옵션 클릭시 대화상자
			}
			else if (quit_button)
				PostQuitMessage(0);
		}
		else if (endscene)
		{
			PreTime = 5;
			endscene = false;
			Start_scene = true;
			KillTimer(hWnd, 0);
			SetTimer(hWnd, 5, 50, NULL);
		}
		else
		{
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
		}
		break;
	case WM_LBUTTONUP:
		Button[1] = 0;
		Button[2] = 0;
		break;
	case WM_TIMER:
		switch (wParam) {
		case 0:				//GetAsyncKeyState - 키가 눌린 상태로 진행되는함수 (끊김없이)http://www.silverwolf.co.kr/cplusplus/4842
			
			if (EscMode)
			{
				if (PreTime > -5) PreTime--;
				else if (Start_scene == false) Start_scene = true;
			}
			else if (PreTime < 16) PreTime++;
			else
			{
				if (Time == 0 && PreTime < 25) PreTime = 25;
				ButtonActive();
				ReflectorPosition(ReflectorHead, (GetAsyncKeyState(VK_LEFT) & 0x8001), (GetAsyncKeyState(VK_RIGHT) & 0x8001), (GetAsyncKeyState(VK_UP) & 0x8001 || GetAsyncKeyState(VK_SPACE) & 0x8001), (GetAsyncKeyState(VK_DOWN) & 0x8001 || GetAsyncKeyState(VK_SHIFT) & 0x8001));
				if (GameStart)
				{
					if (Time == 0) OrbCreate(OrbHead, 0, 0, 0, 0.25);
					if (Time == 0) {
						OrbCreate(OrbHead, 2, 0, 0, 0);
						OrbCreate(OrbHead, 2, 0, 0, 0.5);
						OrbCreate(OrbHead, 2, 0, 0, 0.75);
					}
					Time++;
					CherenkovCheck();
					CollisionDetect(OrbHead);
					if (ReactorEffect > 5) GameOver();
				}
				else
				{
					OrbClear(OrbHead);
				}
			}
			break;
		case 3:
			if (ReactorEffect > 6) GameRestart();
			else if (ReactorEffect == 6)
			{
				GeneralReset();
				KillTimer(hWnd, 3);
			}
		case 5:				// 스타트 버튼 누르면 1초뒤에 문열리는 애니메이션
			if (Start_scene)
			{
				if (PreTime > -25) PreTime--;
				else if (rand() % 10 == 0) PreTime = 5;
			}
			else
			{
				if (PreTime < 25) PreTime++;
				else
				{
					SetTimer(hWnd, 0, 10, NULL);
					KillTimer(hWnd, 5);
				}
			}
			break;
		case 100:			// -----엔드씬 추가부
			if (PreTime > -5) PreTime--;
			else
			{
				endscene = true;
				PreTime = -25;
				KillTimer(hWnd, 100);
			}
			break;
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
		SetBkMode(memdc, TRANSPARENT);

		if (Start_scene)
		{
			// 스타트 씬 
			DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
			if (PreTime < 0) UIMenu(start_button, module_button, option_button, quit_button, EscMode);
		}
		else if (endscene)
		{
			DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
			UIEndMessage();
		}
		else 
		{
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

			if (debug)	UIDebugInfo();
			UIScore();
			EffectPrint(EffectHead);

			// 문 애니메이션 씬
			if (PreTime < 0)
			{
				DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
				if (int(PreTime * 0.2 + 1) & 1) UIMenu(true, false, false, false, false);
			}
			else if (PreTime <= 25) {
				DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y,
					Controllroom_size_x * (PreTime % 5), Controllroom_size_y * (int)(PreTime / 5), Controllroom_size_x, Controllroom_size_y); // 3000 * (PreTime % 5), 2000 * (int)(PreTime / 5)
			}
		}

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

/*
BOOL CALLBACK Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND Button1 = GetDlgItem(hDlg, IDC_CHECK1);

	switch (iMsg) {
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, 1001, 1003, 1001);		//1001 부터 1015까지는 라디오 박스 번호
		CheckRadioButton(hDlg, 1004, 1006, 1004);
		CheckRadioButton(hDlg, 1007, 1009, 1007);
		CheckRadioButton(hDlg, 1010, 1012, 1010);
		CheckRadioButton(hDlg, 1013, 1015, 1013);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDOK: //--- 버튼
			if (SendMessage(Button1, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				SendMessage(Button1, BM_SETCHECK, BST_CHECKED, 0);
				//reflector_reverse = true;
			}
			else if (SendMessage(Button1, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(Button1, BM_SETCHECK, BST_UNCHECKED, 0);
				//reflector_reverse = false;
			}
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL: //--- 버튼
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	return 0;
}
*/

