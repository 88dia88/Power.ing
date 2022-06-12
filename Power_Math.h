#ifndef _POWER_MATH_
#define _POWER_MATH_
#define _USE_MATH_DEFINES
#include <math.h>
extern const double M_TU, Kelvin, MaxTemp, MaxMole, MaxPressure;
double PointRotationX(double x, double y, double angle);
double PointRotationY(double x, double y, double angle);
double PressureCaculate(double Mole, double Temperture);
double SpeedCaculate(double Power, double Mole, double Temperture);
double OrbScore(double Speed, double Mole, double Pressure, double Crits, bool Cherenkov);
double AngleOverflow(double Angle);
double AnglePosition(double x, double y);
double Reflect(double Angle, double Reflector);
double DistancePosition(double x, double y);
int PNcmp(double num);
bool Distancecmp(double x, double y, double dis);
bool DistanceOvercmp(double x, double y, double dis);
bool DistanceDetect(double x, double y, double Angle, double Distance, double Size);
bool AngleDetect(double x, double y, double Angle);
bool ObtuseDetect(double Angle);
#endif