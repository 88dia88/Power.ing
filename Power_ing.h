#ifndef _POWER_ORB_
#define _POWER_ORB_
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <tchar.h>
#include <atlImage.h>
#include "Power_Math.h"
//--------------------------------------------------------------------------------------------------------------//
extern HINSTANCE g_hInst;
extern LPCTSTR lpszClass, lpszWindowName;

extern const bool debug, keyboard;
extern const double window_size, window_half, window_size_x, window_size_y, Pibot_x, Pibot_y;

extern const double Reactor_size, Rail_size, Orb_size;
extern const double Controllroom_size_x, Reflector_size_x;
extern const double Controllroom_size_y, Reflector_size_y;

extern const double Reactor_window, Rail_window, Orb_window;
extern const double Controllroom_window_x, Reflector_window_x;
extern const double Controllroom_window_y, Reflector_window_y;

extern const double Reactor_half, Rail_half, Orb_half;
extern const double Controllroom_half_x, Reflector_half_x;
extern const double Controllroom_half_y, Reflector_half_y;

extern HDC hdc, memdc;
extern HBITMAP hBitmap;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wparam, LPARAM lparam);
//--------------------------------------------------------------------------------------------------------------//
// 
// Power_Orb
// 
//--------------------------------------------------------------------------------------------------------------//
struct Power_Cherenkov
{
	bool cherenkov;
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
	double x, y, speed, angle, power, size;
	double speedx, speedy, shellx, shelly;
	int type, effect;
	struct Power_Orb* next;
};
struct Power_Reflector
{
	double angle, position, size, speed;
	int module[5], age, effect;
	// 0.1 - accellator, 0.2 - deccellator || 1.1 double 1.2 triple || 2.1 spliter, 2.2 launcher || 3.1 gear, 3.2 magnet || 4.1 reserver, 4.2  // 
	struct Power_Reflector* next;
};
extern bool GameStart;
extern double Score, Temperture, Mole;
extern int Time, ReactorEffect, Button[5];
extern struct Power_Cherenkov Cherenkov;
extern struct Power_Orb* OrbHead;
extern struct Power_Reflector* ReflectorHead;
//--------------------------------------------------------------------------------------------------------------//
void CherenkovCheck();
void GeneralReset();
void GameRestart();
void GameOver();
void ButtonActive();
//--------------------------------------------------------------------------------------------------------------//
bool PressureCheck();
//--------------------------------------------------------------------------------------------------------------//
struct Power_Orb* OrbPosition(struct Power_Orb* Orb);
struct Power_Orb* OrbSpeed(struct Power_Orb* Orb);
void CollisionDetect(struct Power_Orb* Orb);
void OrbCreate(struct Power_Orb* Orb, int Type, double x, double y, double Angle);
void OrbRemove(struct Power_Orb* NextOrb, struct Power_Orb* Orb);
struct Power_Orb* OrbApply(struct Power_Orb* Orb, int Type, double x, double y, double Angle);
//--------------------------------------------------------------------------------------------------------------//
void ReflectDetect(struct Power_Orb* Orb, struct Power_Reflector* Reflector);
void ReflectReflector(struct Power_Orb* Orb, struct Power_Reflector* Reflector);
struct Power_Orb* ReflectReflectorOrb(struct Power_Orb* Orb, struct Power_Reflector* Reflector);
struct Power_Orb* ReflectOrb(struct Power_Orb* Orb, double Angle);
//--------------------------------------------------------------------------------------------------------------//
void ReflectorPosition(struct Power_Reflector* Reflector, bool Left, bool Right, bool Up, bool Down);
void ReflectorCreate(struct Power_Reflector* Reflector, int Count);
void ReflectorRemove(struct Power_Reflector* NextReflector, struct Power_Reflector* Reflector);
struct Power_Reflector* ReflectorReset(struct Power_Reflector* Reflector);
struct Power_Reflector* ReflectorApply(struct Power_Reflector* Reflector, int Count);
//--------------------------------------------------------------------------------------------------------------//
// 
// Power_Display
// 
//--------------------------------------------------------------------------------------------------------------//
extern CImage ReactorImg, ReflectorImg, OrbImg, PressureImg, CherenkovImg, Reactor_EffectImg, Reflector_EffectImg, Reflector_Mask_Img, Reflector_Effect_Mask_Img;
extern CImage Button_PressureImg, Button_DialImg, Button_ValveImg, Button_OrbImg, Cherenkov_LeverImg, TempertureImg;
extern CImage Pressure_Mask_Img, Cherenkov_Mask_Img, Button_Valve_Mask_Img, Button_Dial_Mask_Img, Temperture_Mask_Img;
extern CImage Reflector_Module1_Img, Reflector_Module2_Img, Reflector_Module3_Img, Reflector_Module4_Img, Reflector_Module5_Img;
extern CImage Reflector_Module1_Mask_Img, Reflector_Module2_Mask_Img, Reflector_Module3_Mask_Img, Reflector_Module4_Mask_Img, Reflector_Module5_Mask_Img;
extern struct Power_Effect* EffectHead;
//--------------------------------------------------------------------------------------------------------------//
void CreateEffect(struct Power_Effect* Effect, double x, double y, double Score);
void RemoveEffect(struct Power_Effect* Effect, struct Power_Effect* NextEffect);
void EffectPrint(struct Power_Effect* Effect);
//--------------------------------------------------------------------------------------------------------------//
void DisplayLoad();
void Display();
//--------------------------------------------------------------------------------------------------------------//
void DisplayOrb(struct Power_Orb* Orb);
void DisplayReflector(struct Power_Reflector* Reflector);
void DisplayRotatedImage(double x, double y, double Sizex, double Sizey, double Angle, int Type);
//--------------------------------------------------------------------------------------------------------------//
void ScoreUI();
void DebugInfo();
//--------------------------------------------------------------------------------------------------------------//
POINT RotatePaint1(double x, double y, double sizex, double sizey, double angle);
POINT RotatePaint2(double x, double y, double sizex, double sizey, double angle);
POINT RotatePaint3(double x, double y, double sizex, double sizey, double angle);
POINT ReflectorPaint1(struct Power_Reflector* Reflector, double Vertical);
POINT ReflectorPaint2(struct Power_Reflector* Reflector, double Vertical);
POINT ReflectorPaint3(struct Power_Reflector* Reflector, double Vertical);
//--------------------------------------------------------------------------------------------------------------//
#endif