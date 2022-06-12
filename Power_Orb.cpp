#include "Power_ing.h"
//--------------------------------------------------------------------------------------------------------------//
bool GameStart;
double Score, Temperture, Mole;
int Time, ReactorEffect, Button[5];
struct Power_Cherenkov Cherenkov;
struct Power_Orb* OrbHead = (Power_Orb*)malloc(sizeof(struct Power_Orb));
struct Power_Reflector* ReflectorHead = (Power_Reflector*)malloc(sizeof(struct Power_Reflector));
//--------------------------------------------------------------------------------------------------------------//
void CherenkovCheck()
{
	if (Cherenkov.cherenkov)
	{
		if (ReactorEffect < 5 && Time % 3 == 0)	ReactorEffect ++;
		if (Cherenkov.counter > 0) Cherenkov.counter = 0;
		if (Cherenkov.meter > 0)
			Cherenkov.meter -= 2;
		else
		{
			Cherenkov.lever = 5;
			Cherenkov.cherenkov = false;
		}
	}
	else
	{
		if (Cherenkov.meter < 875 && Cherenkov.lever > 0 && Time % 3 == 0) Cherenkov.lever--;
		if (Cherenkov.lever == 6) (Cherenkov.cherenkov = true);
		if (ReactorEffect > 0 && ReactorEffect < 6 && Time % 3 == 0)
		{
			ReactorEffect--;
		}
		if (Cherenkov.meter > 1000)
		{
			Cherenkov.counter = 0;
			Cherenkov.meter = 1000;
		}
		else if (Cherenkov.counter > 0)
		{
			Cherenkov.meter++;
			Cherenkov.counter--;
		}
	}
}
void GeneralReset()
{
	Cherenkov.cherenkov = false, Cherenkov.meter = 0, Cherenkov.counter = 0, Cherenkov.lever = 0;
	for (int i = 0; i < 5; i++) Button[i] = 0;
	Time = 0, Score = 0, ReactorEffect = 0, Temperture = Kelvin, Mole = MaxMole / 2;
}
void GameRestart()
{
	Cherenkov.meter -= Cherenkov.meter / (31 + Time);
	Score -= Score / (31.0 + Time);
	Temperture -= (Temperture - Kelvin) / (31.0 + Time);
	Mole -= (Mole - MaxMole / 2) / (31.0 + Time);
	if (Time % 5 == 0) ReactorEffect--;
	Time--;
}
void GameOver()
{
	if (ReactorEffect < 11)
	{
		if (Time % 3 == 0)	ReactorEffect++;
	}
	else
	{
		Time = -1;
		GameStart = false;
	}
}
void ButtonActive()
{
	if ((Button[1] < 0 && Temperture + Button[1] > Kelvin) || (Button[1] > 0 && Temperture + Button[1] < MaxTemp && PressureCaculate(Mole, Temperture + Button[1]) < 1)) Temperture += Button[1];
	else Button[1] = 0;
	if ((Button[2] < 0 && Mole + Button[2] >= 0) || (Button[2] > 0 && Mole + Button[2] < MaxMole && PressureCaculate(Mole + Button[2], Temperture) < 1)) Mole += Button[2];
	else Button[2] = 0;
	if (Button[3] > 0) Button[3]--;
	if (Button[4] > 0) Button[4]--;
}
//--------------------------------------------------------------------------------------------------------------//
bool PressureCheck()
{
	return (PressureCaculate(Mole, Temperture) <= 0.875 && (PressureCaculate(Mole, Temperture) >= 0.375 || PressureCaculate(Mole, Temperture) <= 0));
}
//--------------------------------------------------------------------------------------------------------------//
struct Power_Orb* OrbPosition(struct Power_Orb* Orb)
{
	if (Cherenkov.cherenkov)
	{
		Orb->x += Orb->speedx / 2;
		Orb->y += Orb->speedy / 2;
	}
	else
	{
		Orb->x += Orb->speedx;
		Orb->y += Orb->speedy;
	}
	return Orb;
}
struct Power_Orb* OrbSpeed(struct Power_Orb* Orb)
{
	Orb->speed = SpeedCaculate(Orb->power, Mole, Temperture);
	Orb->speedx = Orb->speed * cos(M_TU * Orb->angle) * 5;
	Orb->speedy = Orb->speed * sin(M_TU * Orb->angle) * 5;
	Orb->shellx = cos(M_TU * Orb->angle) * Orb->size;
	Orb->shelly = sin(M_TU * Orb->angle) * Orb->size;
	return Orb;
}
void CollisionDetect(struct Power_Orb* Orb)
{
	if (Orb->next != OrbHead)
	{
		if (DistanceOvercmp(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, 500))
		{
			if ((Orb->next->type == 1 || Orb->next->effect == 1) && Distancecmp(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, 525))
			{
				ReflectOrb(Orb, AnglePosition(Orb->x, Orb->y));
				if (Orb->next->effect == 1) Orb->next->effect = 0;
			}
			else
			{
				OrbRemove(Orb->next, Orb);
				ReactorEffect = 6;
			}
		}
		OrbPosition(Orb->next);
	}
	else return;
}
void OrbCreate(struct Power_Orb* Orb, int Type, double x, double y, double Angle)
{
	if (Orb->next == OrbHead)
	{
		Orb->next = new Power_Orb;
		Orb->next = OrbApply(Orb->next, Type, x, y, Angle);
		Orb->next->next = OrbHead;
		return;
	}
	else OrbCreate(Orb->next, Type, x, y, Angle);
}
void OrbRemove(struct Power_Orb* NextOrb, struct Power_Orb* Orb)
{
	Orb->next = Orb->next->next;
	delete NextOrb;
	return;
}
struct Power_Orb* OrbApply(struct Power_Orb* Orb, int Type, double x, double y, double Angle)
{
	switch (Type)
	{
	case 0:
		Orb->power = 2, Orb->size = 25, Orb->type = 0, Orb->effect = 0;
		break;
	case 1:
		Orb->power = 2, Orb->size = 25, Orb->type = 1, Orb->effect = 0;
		break;
	}
	Orb->x = x, Orb->y = y, Orb->angle = Angle;
	Orb = OrbSpeed(Orb);
	return Orb;
}
//--------------------------------------------------------------------------------------------------------------//
void ReflectDetect(struct Power_Orb* Orb, struct Power_Reflector* Reflector)
{
	if (Orb->next != OrbHead)
	{
		if (Distancecmp(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, Reflector->position) && 
			AngleDetect(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, Reflector->angle) && 
			DistanceDetect(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, AnglePosition(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly) - Reflector->angle, Reflector->position, Reflector->size))
			ReflectReflector(Orb->next, Reflector);
		ReflectDetect(Orb->next, Reflector);
	}
	else return;
}
void ReflectReflector(struct Power_Orb* Orb, struct Power_Reflector* Reflector)
{
	ReflectReflectorOrb(Orb, Reflector);
	int score;
	if (Reflector->age > -100) score = OrbScore(Orb->speed, Mole, PressureCaculate(Mole, Temperture), 1.35, Cherenkov.cherenkov);
	else score = OrbScore(Orb->speed, Mole, PressureCaculate(Mole, Temperture), 1, Cherenkov.cherenkov);
	CreateEffect(EffectHead, Orb->x, Orb->y, score);
	Score += score;
	if (Cherenkov.meter < 1000 && Cherenkov.cherenkov == false)	Cherenkov.counter += 125;
	Reflector->age = (int)(50 / Orb->speed);
	Reflector->position += 15.0 * (int)Orb->speed;
	if (score > 1000) Reflector->effect = 312;
	else if (score > 500) Reflector->effect = 212;
	else if (score > 250) Reflector->effect = 112;
	else Reflector->effect = 12;
}
struct Power_Orb* ReflectReflectorOrb(struct Power_Orb* Orb, struct Power_Reflector* Reflector)
{
	if (Reflector->module[1] == 1) Orb->power += 0.1;
	else if (Reflector->module[1] == 2) Orb->power -= 0.1;
	ReflectOrb(Orb, Reflector->angle);
	OrbPosition(Orb);
	return Orb;
}
struct Power_Orb* ReflectOrb(struct Power_Orb* Orb, double Angle)
{
	if (ObtuseDetect(AngleOverflow(Orb->angle - Angle)))
	{
		if (AngleOverflow(Orb->angle - Angle) < 0.5) Orb->angle = AngleOverflow(Orb->angle + 0.25);
		else Orb->angle = AngleOverflow(Orb->angle - 0.25);
	}
	else Orb->angle = Reflect(Orb->angle, Angle);
	OrbSpeed(Orb);
	return Orb;
}
//--------------------------------------------------------------------------------------------------------------//
void ReflectorPosition(struct Power_Reflector* Reflector, bool Left, bool Right, bool Up, bool Down)
{
	if (Reflector->next != ReflectorHead)
	{
		if (GameStart && Reflector->next->age <= 0) ReflectDetect(OrbHead, Reflector->next);
		if (Reflector->next->effect % 100 == 0) Reflector->next->effect = 0;
		else if ((Time & 1) && Reflector->next->effect > 0) Reflector->next->effect--;
		if (Reflector->next->age > -101) Reflector->next->age--;
		double Break = 1;
		if (Up || Down && Reflector->next->position < 455 && Reflector->next->position > 270)
		{
			switch (Reflector->next->module[3])
			{
			case 0:
				if (Up) Break = 2;
				if (Down) Break = 0.5;
				break;
			case 1:
				if (Up && Reflector->next->speed < 2) Reflector->next->speed += 0.5;
				if (Down && Reflector->next->speed > 0) Reflector->next->speed -= 0.5;
			case 2:
				if (Up && Reflector->next->position > 300)  Reflector->next->position -= 15;
				if (Down && Reflector->next->position < 450) Reflector->next->position += 15;
				break;
			}
		}
		else
		{
			if (Reflector->next->position < 375) Reflector->next->position += 5;
			else if (Reflector->next->position > 500) Reflector->next->position -= (int)((Reflector->next->position * 0.2 - 75) * 0.2) * 5.0;
			else if (Reflector->next->position > 375) Reflector->next->position -= 5;
		}
		if (Right) Reflector->next->angle = AngleOverflow(Reflector->next->angle - 0.006 * Reflector->next->speed / Reflector->next->position * 375 * Break);
		if (Left) Reflector->next->angle = AngleOverflow(Reflector->next->angle + 0.006 * Reflector->next->speed / Reflector->next->position * 375 * Break);
		ReflectorPosition(Reflector->next, Left, Right, Up, Down);
	}
	else return;
}
void ReflectorCreate(struct Power_Reflector* Reflector, int Count)
{
	if (Reflector->next == ReflectorHead)
	{
		Reflector->next = new Power_Reflector;
		Reflector->next = ReflectorApply(Reflector->next, Count);
		Reflector->next->next = ReflectorHead;
		if (Count < Reflector->module[2])  ReflectorCreate(Reflector->next, Count + 1);
		else return;
	}
	else ReflectorCreate(Reflector->next, Count);
}
void ReflectorRemove(struct Power_Reflector* NextReflector, struct Power_Reflector* Reflector)
{
	Reflector->next = Reflector->next->next;
	delete NextReflector;
	return;
}
struct Power_Reflector* ReflectorReset(struct Power_Reflector* Reflector)
{
	Reflector->module[0] = 0, Reflector->module[1] = 1, Reflector->module[2] = 5, Reflector->module[3] = 2, Reflector->module[4] = 0;
	Reflector->position = 375, Reflector->size = 375, Reflector->speed = 1, Reflector->age = -100, Reflector->effect = 0;
	if (Reflector->module[2] & 1) Reflector->angle = 0;
	else Reflector->angle = 0.25;
	return Reflector;
}
struct Power_Reflector* ReflectorApply(struct Power_Reflector* Reflector, int Count)
{
	Reflector->position = ReflectorHead->position;
	Reflector->size = ReflectorHead->size;
	Reflector->speed = ReflectorHead->speed;
	Reflector->age = ReflectorHead->age;
	Reflector->effect = ReflectorHead->effect;
	for (int i = 0; i < 5; i++)	Reflector->module[i] = ReflectorHead->module[i];
	Reflector->angle = ReflectorHead->angle + Count / (Reflector->module[2] + 1.0);
	return Reflector;
}
//--------------------------------------------------------------------------------------------------------------//