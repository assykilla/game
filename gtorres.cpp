//George Torres

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "fonts.h"
using namespace std;

int points = 0;
int credit = 0;
int currentBalls = 1;
bool leftFlipper, rightFlipper = 0;
Rect stats[1];
bool level1 = 1;
bool level2 = 0;

void renderStats(Rect r, int x, int y, int pts) {
    r.bot = y-35;
    r.left = x/2;
    r.center = -5;
    ggprint8b(&r, 16, 0x0000c8f5, "Points: %i", pts);
}

float Gt1[] = 
{
    5.0f, 50.0f,
    150.0f, 50.0f,
    5.0f, 100.0f
};

float Gt2[] =
{
    545.0f, 100.0f,
    545.0f, 50.0f,
    400.0f, 50.0f
};

float Gt3[] =
{
    600.0f, 700.0f,
    600.0f, 800.0f,
    500.0f, 800.0f
};

float Gt4[] =
{
    105.0f, 795.0f,
    5.0f, 695.0f,
    5.0f, 795.0f
};

float Gt5[] =
{
    225.0f, 795.0f,
    225.0f, 695.0f,
    200.0f, 795.0f
};

float Gt6[] =
{
    225.0f, 795.0f,
    225.0f, 695.0f,
    250.0f, 795.0f
};

float Gt7[] =
{
    80.0f, 550.0f,
    5.0f, 550.0f,
    5.0f, 600.0f
};

float Gt8[] =
{
    80.0f, 550.0f,
    5.0f, 300.0f,
    5.0f, 550.0f
};

float Gt9[] =
{
    545.0f, 650.0f,
    545.0f, 500.0f,
    420.0f, 500.0f
};

float Gt10[] =
{
    545.0f, 250.0f,
    545.0f, 500.0f,
    420.0f, 500.0f
};

float Gt11[] =
{
    150.0f, 100.0f,
    60.0f, 135.0f,
    60.0f, 275.0f
};

float Gt12[] =
{
    490.0f, 275.0f,
    490.0f, 135.0f,
    400.0f, 100.0f
};

int addScore(int amount)
{
    return amount;
}

bool TriangleCol(float t[5], float ballx, float bally)
{
    float s1 = t[5] - t[1];
	float s2 = t[4] - t[0];
	float s3 = t[3] - t[1];
	float s4 = bally - t[1];

	float w1 = (t[0] * s1 + s4 * s2 - ballx * s1) / (s3 * s2 - (t[2]-t[0]) * s1);
	float w2 = (s4- w1 * s3) / s1;
	return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
}
