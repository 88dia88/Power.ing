#ifndef _POWER_ORB_
#define _POWER_ORB_
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <tchar.h>
#include <atlImage.h>
#include "Power_Math.h"
#include "resource.h"
//--------------------------------------------------------------------------------------------------------------//
extern HINSTANCE g_hInst;
extern LPCTSTR lpszClass, lpszWindowName;

extern bool debug, keyboard, ChargedMod;

extern int Reflector1Left, Reflector1Right, Reflector1Up, Reflector1Down, Player1RGB[3], Player1Charge[3];

extern double window_half, window_size_x, window_size_y, Pibot_x, Pibot_y;
extern double window_size;

extern const int Reactor_size, Rail_size, Orb_size;
extern const int Controllroom_size_x, Reflector_size_x;
extern const int Controllroom_size_y, Reflector_size_y;

extern int Reactor_window, Rail_window, Orb_window;
extern int Controllroom_window_x, Reflector_window_x;
extern int Controllroom_window_y, Reflector_window_y;

extern int Reactor_half, Rail_half, Orb_half;
extern int Controllroom_half_x, Reflector_half_x;
extern int Controllroom_half_y, Reflector_half_y;

extern HDC hdc, memdc;
extern HBITMAP hBitmap;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK Module_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Option_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//--------------------------------------------------------------------------------------------------------------//
// 
// Power_Orb
// 
//--------------------------------------------------------------------------------------------------------------//
struct Power_Cherenkov
{
	bool cherenkov, levertrigger;
	int meter, counter, lever;
};
struct Power_Effect
{
	double x, y, score;
	int age;
	struct Power_Effect* next;
};
struct Power_Orb
{
	bool major;
	int type, effect, effect_count;
	double speedx, speedy, shellx, shelly;
	double x, y, speed, angle, power, size;
	double afterx[50], aftery[50];
	struct Power_Orb* next;
};
struct Power_Reflector
{
	double angle, position, size, speed;
	int module[5], age, effect;
	bool module_charged[5]; 
	struct Power_Reflector* next;
};
extern bool GameStart;
extern double Score, Temperture, Mole;
extern int Time, PreTime, ReactorEffect, Button[5], OrbType, Orbcount;
extern struct Power_Cherenkov Cherenkov;
extern struct Power_Orb* OrbHead;
extern struct Power_Reflector* ReflectorHead;
//--------------------------------------------------------------------------------------------------------------//
void CherenkovCheck();
void GeneralReset();
void GameRestart();
void ButtonActive();
//--------------------------------------------------------------------------------------------------------------//
bool PressureCheck();
//--------------------------------------------------------------------------------------------------------------//
struct Power_Orb* OrbPosition(struct Power_Orb* Orb);
struct Power_Orb* OrbSpeed(struct Power_Orb* Orb);
void CollisionDetect(struct Power_Orb* Orb);
bool OrbMajorCheck(struct Power_Orb* Orb);
void OrbCreate(struct Power_Orb* Orb, int Type, bool Major, double x, double y, double Angle);
void OrbRemove(struct Power_Orb* NextOrb, struct Power_Orb* Orb);
void OrbClear(struct Power_Orb* Orb);
struct Power_Orb* OrbApply(struct Power_Orb* Orb, int Type, bool Major, double x, double y, double Angle);
//--------------------------------------------------------------------------------------------------------------//
void ReflectDetect(struct Power_Orb* Orb, struct Power_Reflector* Reflector);
void ReflectReflector(struct Power_Orb* Orb, struct Power_Reflector* Reflector);
struct Power_Orb* ReflectReflectorOrb(struct Power_Orb* Orb, struct Power_Reflector* Reflector);
struct Power_Orb* ReflectOrb(struct Power_Orb* Orb, double Angle);
//--------------------------------------------------------------------------------------------------------------//
void ReflectorPosition(struct Power_Reflector* Reflector, short Left, short Right, short Up, short Down);
void ReflectorCreate(struct Power_Reflector* Reflector, int Count);
void ReflectorRemove(struct Power_Reflector* NextReflector, struct Power_Reflector* Reflector);
void ReflectClear(struct Power_Reflector* Reflector);
struct Power_Reflector* ReflectorReset(struct Power_Reflector* Reflector);
struct Power_Reflector* ReflectorApply(struct Power_Reflector* Reflector, int Count);
//--------------------------------------------------------------------------------------------------------------//
// 
// Power_Display
// 
//--------------------------------------------------------------------------------------------------------------//
extern CImage ReactorImg, Reactor_EffectImg, ReflectorImg, Reflector_EffectImg, OrbImg, PressureImg, CherenkovImg;
extern CImage Reflector_Mask_Img, Reflector_Effect_Mask_Img, Reflector_Color_Mask_Img, Reflector_Light_Mask_Img;
extern CImage Reactor_RailImg, Reflector_ColorImg, Reflector_LightImg, Reflector_ColorChargeImg, Reflector_LightChargeImg, Reflector_ColorOffImg, Reflector_LightOffImg;
extern CImage Button_PressureImg, Button_DialImg, Button_ValveImg, Button_OrbImg, Cherenkov_LeverImg, TempertureImg, DoorImg;
extern CImage Pressure_Mask_Img, Cherenkov_Mask_Img, Button_Valve_Mask_Img, Button_Dial_Mask_Img, Temperture_Mask_Img;
extern CImage Reflector_Module_Img, Reflector_Module_Mask_Img;
extern struct Power_Effect* EffectHead;
//--------------------------------------------------------------------------------------------------------------//
void CreateEffect(struct Power_Effect* Effect, double x, double y, double Score);
void RemoveEffect(struct Power_Effect* Effect, struct Power_Effect* NextEffect);
void EffectPrint(struct Power_Effect* Effect);
//--------------------------------------------------------------------------------------------------------------//
void DisplayLoad();
void DisplayColorApply();
void DisplayWindow();
//--------------------------------------------------------------------------------------------------------------//
void DisplayOrb(struct Power_Orb* Orb);
void DisplayReflector(struct Power_Reflector* Reflector);
void DisplayRotatedImage(double x, double y, double Sizex, double Sizey, double Angle, int Type);
//--------------------------------------------------------------------------------------------------------------//
void UIMenu(bool Start, bool Module, bool Option, bool Quit, bool Esc);
void UIButton(int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30]);
bool UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse);
void UIEndMessage();
void UIScore();
void UIDebugInfo();
//--------------------------------------------------------------------------------------------------------------//
POINT RotatePaint1(double x, double y, double sizex, double sizey, double angle);
POINT RotatePaint2(double x, double y, double sizex, double sizey, double angle);
POINT RotatePaint3(double x, double y, double sizex, double sizey, double angle);
POINT ReflectorPaint1(struct Power_Reflector* Reflector, double Vertical);
POINT ReflectorPaint2(struct Power_Reflector* Reflector, double Vertical);
POINT ReflectorPaint3(struct Power_Reflector* Reflector, double Vertical);
//--------------------------------------------------------------------------------------------------------------//
#endif