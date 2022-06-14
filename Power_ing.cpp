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
		GeneralReset();
		PreTime = -25;
		DisplayLoad();
		DisplayColorApply();

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
				SetTimer(hWnd, 5, 50, NULL);
			}
			else if (Start_scene == false)
			{
				EscMode = true;
				PreTime = 15;
			}
		}
		if (wParam == VK_RETURN) {
			if (GameStart)
			{
				if (Cherenkov.lever == 0 || Cherenkov.meter >= 875 && Cherenkov.lever < 6 && Cherenkov.cherenkov == false)
				{
					Cherenkov.levertrigger = true;
					Cherenkov.lever++;
				}
			}
			else if (PressureCheck())
			{
				Time = 0;
				GameStart = true;
			}
		}
		break;
	case WM_KEYUP:
		if (wParam == VK_RETURN) {
			Cherenkov.levertrigger = false;
		}
		break;
	case WM_CHAR:
		if (wParam == 'u') {				// 엔드 씬 추가부
			SetTimer(hWnd, 100, 50, NULL);
		}
		break;
	case WM_MOUSEMOVE:
		if (Start_scene)
		{
			Mouse = MAKEPOINTS(lParam);
			start_button = (!EscMode && UIButtonSelected(-700, -135, 750, 150, Mouse));
			module_button = UIButtonSelected(-700, 15, 750, 150, Mouse);
			option_button = UIButtonSelected(-700, 165, 750, 150, Mouse);
			quit_button = UIButtonSelected(-700, 315, 750, 150, Mouse);
		}
		else if (Time == 0)
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
		}
		else if (Cherenkov.levertrigger)
		{
			if (Cherenkov.lever == 0 || Cherenkov.meter >= 875 && Cherenkov.lever < 6 && Cherenkov.cherenkov == false)
			{
				if (UIButtonSelected(-737.5, -67.5, 200, 167.5, Mouse) && Cherenkov.lever == 0) Cherenkov.lever = 1;
				else if (UIButtonSelected(-737.5, -42.5, 200, 142.5, Mouse) && Cherenkov.lever == 1) Cherenkov.lever = 2;
				else if (UIButtonSelected(-737.5, -15, 200, 115, Mouse) && Cherenkov.lever == 2) Cherenkov.lever = 3;
				else if (UIButtonSelected(-737.5, 15, 200, 85, Mouse) && Cherenkov.lever == 3) Cherenkov.lever = 4;
				else if (UIButtonSelected(-737.5, 42.5, 200, 57.5, Mouse) && Cherenkov.lever == 4) Cherenkov.lever = 5;
				else if (UIButtonSelected(-737.5, 67.5, 200, 32.5, Mouse) && Cherenkov.lever == 5) Cherenkov.lever = 6;
			}
		}
		else if (keyboard == false)
		{
			Mouse = MAKEPOINTS(lParam);
			//Reflector->angle = AnglePosition(Mouse.x - Pibot_x, Mouse.y - Pibot_y);
		}
		break;
	case WM_LBUTTONDOWN:
		if (Start_scene)
		{
			if (start_button) {
				Start_scene = false;
				PreTime = -25;
			}
			else if (module_button) {
				if (EscMode)
				{
					EscMode = false;
					Start_scene = false;
				}
				else
				{
					// 모듈 클릭시 대화상자
					DialogBoxW(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)&Module_Proc);
				}
			}
			else if (option_button)
			{
				// 옵션 클릭시 대화상자
				DialogBoxW(g_hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, (DLGPROC)&Option_Proc);
			}
			else if (quit_button)
			{
				if (EscMode)
				{
					SetTimer(hWnd, 5, 50, NULL);
				}
				else PostQuitMessage(0);
			}
		}
		else if (endscene)
		{
			SetTimer(hWnd, 5, 50, NULL);
		}
		else if (Time == 0)
		{
			Mouse = MAKEPOINTS(lParam);
			if (UIButtonSelected(-821.5, 260, 40, 80, Mouse)) Button[1] = 1;
			else if (UIButtonSelected(-861.5, 260, 40, 80, Mouse)) Button[1] = -1;
			else if (UIButtonSelected(-678.5, 260, 40, 80, Mouse)) Button[2] = 1;
			else if (UIButtonSelected(-718.5, 260, 40, 80, Mouse)) Button[2] = -1;
			else if (UIButtonSelected(653.5, 250, 50, 50, Mouse))
			{
				Button[3] = 10;
			}
			else if (UIButtonSelected(796.5, 250, 50, 50, Mouse))
			{
				Button[4] = 10;
			}
		}
		else
		{
			Mouse = MAKEPOINTS(lParam);
			if (UIButtonSelected(-737.5, -100, 200, 200, Mouse)) Cherenkov.levertrigger = true;
			if (UIButtonSelected(653.5, 250, 50, 50, Mouse)) Button[3] = 10;
			else if (UIButtonSelected(796.5, 250, 50, 50, Mouse)) Button[4] = 10;
		}
		break;
	case WM_LBUTTONUP:
		Button[1] = 0;
		Button[2] = 0;
		Cherenkov.levertrigger = false;
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
				ReflectorPosition(ReflectorHead, GetAsyncKeyState(Reflector1Left), GetAsyncKeyState(Reflector1Right), GetAsyncKeyState(Reflector1Up), GetAsyncKeyState(Reflector1Down));
				if (GameStart)
				{
					if (Time == 25) OrbCreate(OrbHead, 0, true, 0, 0, 0.25);
					Time++;
					CherenkovCheck();
					CollisionDetect(OrbHead);
					if (ReactorEffect > 5)
					{
						if (ReactorEffect < 15)
						{
							if (Time % 3 == 0)	ReactorEffect++;
						}
						else
						{
							ReactorEffect = 12;
							Time = -1;
							GameStart = false;
							SetTimer(hWnd, 3, 10, NULL);
						}
					}
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
			if (endscene)
			{
				PreTime = 5;
				endscene = false;
				Start_scene = true;
				ReflectorClear(ReflectorHead);
			}
			else if (EscMode)
			{
				KillTimer(hWnd, 0);
				GeneralReset();
				EscMode = false;
				PreTime = 5;
				Start_scene = true;
				ReflectorClear(ReflectorHead);
			}
			else if (Start_scene)
			{
				if (PreTime > -25) PreTime--;
				else if (rand() % 50 == 0) PreTime = 5;
			}
			else
			{
				if (PreTime < 25) PreTime++;
				else
				{
					ReflectorCreate(ReflectorHead, 0);
					SetTimer(hWnd, 0, 10, NULL);
					KillTimer(hWnd, 5);
				}
			}
			break;
		case 100:			// -----엔드씬 추가부
			if (endscene)
			{
				if (PreTime > -5) PreTime--;
				else
				{
					PreTime = -25;
					KillTimer(hWnd, 100);
				}
			}
			else
			{
				endscene = true;
				PreTime == 25;
				KillTimer(hWnd, 0);
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
		else if (endscene && PreTime == -25)
		{
			DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
			UIEndMessage();
		}
		else 
		{
			ReactorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
			Reactor_RailImg.Draw(memdc, Pibot_x - 782 * window_half, Pibot_y - 782 * window_half, 782 * window_size, 782 * window_size, 0, 0, 782, 782);

			DisplayReflector(ReflectorHead);

			if (ReactorEffect < 12) Reactor_EffectImg.Draw(memdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size* (ReactorEffect % 6), Reactor_size * (int)(ReactorEffect / 6), Reactor_size, Reactor_size);
			else Reactor_EffectImg.Draw(memdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, 5000, 1000, Reactor_size, Reactor_size);

			DisplayOrb(OrbHead);

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

BOOL CALLBACK Module_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

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
		case IDC_RADIO1: ReflectorHead->module[0] = 0;
			break;
		case IDC_RADIO2: ReflectorHead->module[0] = 1;
			break;
		case IDC_RADIO3: ReflectorHead->module[0] = 2;
			break;
		case IDC_RADIO4: ReflectorHead->module[1] = 0;
			break;
		case IDC_RADIO5: ReflectorHead->module[1] = 1;
			break;
		case IDC_RADIO6: ReflectorHead->module[1] = 2;
			break;
		case IDC_RADIO7: ReflectorHead->module[2] = 0;
			break;
		case IDC_RADIO8: ReflectorHead->module[2] = 1;
			break;
		case IDC_RADIO9: ReflectorHead->module[2] = 2;
			break;
		case IDC_RADIO10: ReflectorHead->module[3] = 0;
			break;
		case IDC_RADIO11: ReflectorHead->module[3] = 1;
			break;
		case IDC_RADIO12: ReflectorHead->module[3] = 2;
			break;
		case IDC_RADIO13: ReflectorHead->module[4] = 0;
			break;
		case IDC_RADIO14: ReflectorHead->module[4] = 1;
			break;
		case IDC_RADIO15: ReflectorHead->module[4] = 2;
			break;
		
		case IDOK:
			
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
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

BOOL CALLBACK Option_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	HWND Button1 = GetDlgItem(hDlg, IDC_CHECK1);

	switch (iMsg) {
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO20, IDC_RADIO22, IDC_RADIO20);		//1001 부터 1003까지는 라디오 박스 번호
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_RADIO20:
			window_size = 0.96;
			break;

		case IDC_RADIO21:
			window_size = 0.96 / 1.25;
			break;

		case IDC_RADIO22:
			window_size = 0.96 / 1.5;
			break;

		case IDC_RADIO23:		// 빨강	 R, G, B
			Player1RGB[0] = 255;
			Player1RGB[1] = 0;
			Player1RGB[2] = 0;
			break;
		case IDC_RADIO24:		// 파랑
			Player1RGB[0] = 0;
			Player1RGB[1] = 0;
			Player1RGB[2] = 255;
			break;
		case IDC_RADIO25:		// 녹색
			Player1RGB[0] = 0;
			Player1RGB[1] = 255;
			Player1RGB[2] = 0;
			break;
		case IDC_RADIO26:		// 자홍
			Player1RGB[0] = 255;
			Player1RGB[1] = 0;
			Player1RGB[2] = 255;
			break;
		case IDC_RADIO27:		// 청록
			Player1RGB[0] = 0;
			Player1RGB[1] = 255;
			Player1RGB[2] = 255;
			break;
		case IDC_RADIO28:		// 노랑
			Player1RGB[0] = 255;
			Player1RGB[1] = 255;
			Player1RGB[2] = 0;
			break;
		case IDOK: //--- 버튼
			if (SendMessage(Button1, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				SendMessage(Button1, BM_SETCHECK, BST_CHECKED, 0);
				//reflector_reverse = true;
			}
			else if (SendMessage(Button1, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(Button1, BM_SETCHECK, BST_UNCHECKED, 0);
				//reflector_reverse = false;
			}
			//해상도 버튼 - 라디오		현재 1 -> 1,  0.6 , 0.8
			DisplayWindow();		// 이미지 다시 불러오기
			DisplayColorApply();	// 컬러 다시 불러오기
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