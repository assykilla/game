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
#include "xreyes.h"
using namespace std;

extern Triangle flipper1;
extern Triangle flipper2;
extern void triangle_collision( Triangle triangle, float *ballx,
            float *bally, float *vx, float *vy); 

int points = 0;
int credit = 0;
int currentBalls = 1;
bool leftFlipper, rightFlipper = 0;
Rect stats[1];
bool level1 = 1;
bool level2 = 0;
int leftFlipperFrame = 0;
int rightFlipperFrame = 0;
float leftAngle = 0.0f;
float rightAngle = 0.0f;
int flipperFrames = 4;
int * x_og;
int * y_og;
int x_rotated;
int y_rotated;
bool noBalls = 0;
bool debugGeorge = 0;
extern const int MAX_BALLS;

Triangle GflipL = Triangle (150.0f, 150.0f, 265.0f,
                            50.0f, 10.0f, 10.0f);
Triangle GflipR = Triangle (400.0f, 285.0f, 400.0f,
                            50.0f, 10.0f, 10.0f);

bool TriangleCol(Triangle, float, float);

extern bool makeBall(unsigned int map, bool *summoned, float *px, float *py, float *vy)
{
    if (!*summoned) {
        if (map == 1) {
            //Spawn a ball
            *px = 415;
            *py = 100;
            *summoned = 1;
            *vy = 8.0f;
            return 1;
        }
        if (map == 2) {
            //Spawn a ball
            *px = 575;
            *py = 100;
            *summoned = 1;
            *vy = 9.5f;
            return 1;
        }
    }
    return 0;
}

void rotate_point(float angle, float *x, float *y, float cx, float cy)
{
  float s = sin(angle);
  float c = cos(angle);

  // translate point back to origin:
  *x -= cx;
  *y -= cy;

  // rotate point
  float xnew = *x * c - *y * s;
  float ynew = *x * s + *y * c;

  // translate point back:
  *x = xnew + cx;
  *y = ynew + cy;
}

void flipping(unsigned int map, float *ballx, float *bally, float *ballvx, float *ballvy)
{
    //LEVEL 1 LOGIC
    if (map == 1) {
        if (leftFlipper == 1) { //LEFT IS PRESSED
            if (leftFlipperFrame <= flipperFrames) { //MOVING UP
                if (TriangleCol(flipper1, *ballx, *bally)) {
                    *ballvy  = 8.0f;
                    cout << "Collision!" << endl;
                }
            } else if (leftFlipperFrame == flipperFrames + 1) { //FLIPPER PEAK
                if (TriangleCol(flipper1, *ballx, *bally)) {
                    *ballvy = *ballvx * 0.8;
                }
            }
        }
        if (leftFlipper == 0) { //LEFT IS RELEASED
            if (leftFlipperFrame > 0) { //MOVING DOWN
                if (TriangleCol(flipper1, *ballx, *bally)) {
                    *ballvx = *ballvx * 1;
                    *ballvy = *ballvy * 1;
                }
            } else if (leftFlipperFrame == 0) { //FLIPPER DOWN
                triangle_collision(flipper1, ballx, bally,
                ballvx, ballvy);
            }
        }


        if (rightFlipper == 1) { //RIGHT IS PRESSED
            if (rightFlipperFrame <= flipperFrames) { //MOVING UP
                if (TriangleCol(flipper2, *ballx, *bally)) {
                    *ballvy  = 8.0f;
                }
            } else if (rightFlipperFrame == flipperFrames + 1) { //FLIPPER PEAK
                rightAngle = 0;
                if (TriangleCol(flipper2, *ballx, *bally)) {
                    *ballvy = *ballvx * -0.8;
                }
            }
        }
        if (rightFlipper == 0) { //RIGHT IS RELEASED
            if (rightFlipperFrame > 0) { //MOVING DOWN
                if (TriangleCol(flipper2, *ballx, *bally)) {
                    *ballvx = *ballvx * 1;
                    *ballvy = *ballvy * 1;
                }
            } else if (rightFlipperFrame == 0) { //FLIPPER DOWN
                triangle_collision(flipper1, ballx, bally,
                ballvx, ballvy);
            }
        }
    }
    //LEVEL 2 LOGIC
    if (map == 2) {
        if (leftFlipper == 1) { //LEFT IS PRESSED
            if (leftFlipperFrame <= flipperFrames) {
                if (TriangleCol(GflipL, *ballx, *bally)) {
                    *ballvy  = 8.0f;
                }
            } else if (leftFlipperFrame == flipperFrames + 1) {
                if (TriangleCol(GflipL, *ballx, *bally)) {
                    *ballvy = *ballvx * 0.8;
                }
            }
        }
        if (leftFlipper == 0) {
            if (leftFlipperFrame > 0) {
                if (TriangleCol(GflipL, *ballx, *bally)) {
                    *ballvx = *ballvx * 1;
                    *ballvy = *ballvy * 1;
                }
            } else if (leftFlipperFrame == 0) {
                triangle_collision(GflipL, ballx, bally,
                                    ballvx, ballvy);
            }
        }


        if (rightFlipper == 1) { //RIGHT IS PRESSED
            if (rightFlipperFrame <= flipperFrames) { //MOVING UP
                if (TriangleCol(GflipR, *ballx, *bally)) {
                    *ballvy  = 8.0f;
            }
            } else if (rightFlipperFrame == flipperFrames + 1) { //FLIPPER PEAK
                rightAngle = 0;
                if (TriangleCol(GflipR, *ballx, *bally)) {
                    *ballvy = *ballvx * -0.8;
                }
            }
        }
        if (rightFlipper == 0) { //RIGHT IS RELEASED
            if (rightFlipperFrame > 0) { //MOVING DOWN
                if (TriangleCol(GflipR, *ballx, *bally)) {
                    *ballvx = *ballvx * 1;
                    *ballvy = *ballvy * 1;
                    cout << "Collision!" << endl;
                }
            } else if (rightFlipperFrame == 0) { //FLIPPER DOWN
                triangle_collision(GflipR, ballx, bally,
                ballvx, ballvy);
            }
        }
    }
}

void flipperRotate (unsigned int map)
{
    //LEVEL 1 LOGIC
    if (map == 1) {
        if (leftFlipper == 1) {
            if (leftFlipperFrame <= flipperFrames) {
                leftFlipperFrame += 1;
                leftAngle = 0.2f;
            } else if (leftFlipperFrame == flipperFrames + 1) {
                leftAngle = 0;
            }
        }
        if (leftFlipper == 0) {
            if (leftFlipperFrame > 0) {
                leftFlipperFrame -= 1;
                leftAngle = -0.2f;
            } else if (leftFlipperFrame == 0) {
                leftAngle = 0;
            }
        }


        if (rightFlipper == 1) {
            if (rightFlipperFrame <= flipperFrames) {
                rightFlipperFrame += 1;
                rightAngle = -0.2f;
            } else if (rightFlipperFrame == flipperFrames + 1) {
                rightAngle = 0;
            }
        }
        if (rightFlipper == 0) {
            if (rightFlipperFrame > 0) {
                rightFlipperFrame -= 1;
                rightAngle = 0.2f;
            } else if (rightFlipperFrame == 0) {
                rightAngle = 0;
            }
        }
    }
    //LEVEL 2 LOGIC
    if (map == 2) {
        if (leftFlipper == 1) {
            if (leftFlipperFrame <= flipperFrames) {
                leftFlipperFrame += 1;
                leftAngle = 0.2f;
            } else if (leftFlipperFrame == flipperFrames + 1) {
                leftAngle = 0;
            }
        }
        if (leftFlipper == 0) {
            if (leftFlipperFrame > 0) {
                leftFlipperFrame -= 1;
                leftAngle = -0.2f;
            } else if (leftFlipperFrame == 0) {
                leftAngle = 0;
            }
        }


        if (rightFlipper == 1) {
            if (rightFlipperFrame <= flipperFrames) {
                rightFlipperFrame += 1;
                rightAngle = -0.2f;
            } else if (rightFlipperFrame == flipperFrames + 1) {
                rightAngle = 0;
            }
        }
        if (rightFlipper == 0) {
            if (rightFlipperFrame > 0) {
                rightFlipperFrame -= 1;
                rightAngle = 0.2f;
            } else if (rightFlipperFrame == 0) {
                rightAngle = 0;
            }
        }
    }
    
    if (map == 1) {
        rotate_point(leftAngle, &flipper1.vertex1[0], &flipper1.vertex1[1], 
                            flipper1.vertex1[0], flipper1.vertex1[1]);
        rotate_point(leftAngle, &flipper1.vertex3[0], &flipper1.vertex3[1], 
                            flipper1.vertex1[0], flipper1.vertex1[1]);
        rotate_point(leftAngle, &flipper1.vertex2[0], &flipper1.vertex2[1], 
                            flipper1.vertex1[0], flipper1.vertex1[1]);

        rotate_point(rightAngle, &flipper2.vertex1[0], &flipper2.vertex1[1], 
                            flipper2.vertex1[0], flipper2.vertex1[1]);
        rotate_point(rightAngle, &flipper2.vertex2[0], &flipper2.vertex2[1], 
                            flipper2.vertex1[0], flipper2.vertex1[1]);
        rotate_point(rightAngle, &flipper2.vertex3[0], &flipper2.vertex3[1], 
                            flipper2.vertex1[0], flipper2.vertex1[1]);
    }
    if (map == 2) {
        rotate_point(leftAngle, &GflipL.vertex1[0], &GflipL.vertex1[1], 
                    GflipL.vertex1[0], GflipL.vertex1[1]);
        rotate_point(leftAngle, &GflipL.vertex3[0], &GflipL.vertex3[1], 
                    GflipL.vertex1[0], GflipL.vertex1[1]);
        rotate_point(leftAngle, &GflipL.vertex2[0], &GflipL.vertex2[1], 
                    GflipL.vertex1[0], GflipL.vertex1[1]);

        rotate_point(rightAngle, &GflipR.vertex1[0], &GflipR.vertex1[1], 
                    GflipR.vertex1[0], GflipR.vertex1[1]);
        rotate_point(rightAngle, &GflipR.vertex2[0], &GflipR.vertex2[1], 
                    GflipR.vertex1[0], GflipR.vertex1[1]);
        rotate_point(rightAngle, &GflipR.vertex3[0], &GflipR.vertex3[1], 
                    GflipR.vertex1[0], GflipR.vertex1[1]);
    }
}




void renderStats(Rect r, int x, int y, int pts)
{
    r.bot = y-35;
    r.left = x/2;
    r.center = -5;
    ggprint8b(&r, 16, 0x0000c8f5, "Points: %i", pts);
}


Triangle Gt1 = Triangle (5.0f, 5.0f, 150.0f, 
                        100.0f ,50.0f ,50.0f);
Triangle Gt2 = Triangle (545.0f,400.0f, 545.0f, 
                        100.0f ,50.0f ,50.0f);
Triangle Gt3 = Triangle (600.0f,500.0f, 600.0f, 
                        700.0f ,700.0f ,600.0f);
Triangle Gt4 = Triangle (5.0f,5.0f, 105.0f, 
                        695.0f ,595.0f ,695.0f);
Triangle Gt5 = Triangle (225.0f,200.0f, 225.0f, 
                        695.0f ,695.0f ,595.0f);
Triangle Gt6 = Triangle (225.0f,225.0f, 250.0f,
                        695.0f ,595.0f ,695.0f);
Triangle Gt7 = Triangle (5.0f,5.0f, 80.0f, 
                        500.0f ,450.0f ,450.0f);
Triangle Gt8 = Triangle (5.0f, 5.0f, 80.0f, 
                        450.0f ,200.0f ,450.0f);
Triangle Gt9 = Triangle (545.0f,420.0f, 545.0f, 
                        550.0f ,400.0f ,400.0f);
Triangle Gt10 = Triangle (545.0f,420.0f, 545.0f, 
                        400.0f ,400.0f ,150.0f);
Triangle Gt11 = Triangle (150.0f,60.0f, 60.0f, 
                        100.0f ,135.0f ,275.0f);
Triangle Gt12 = Triangle (490.0f,490.0f, 400.0f, 
                        275.0f ,135.0f ,100.0f);


void addScore(int amount)
{
    points += amount;
    credit += amount;
}

bool TriangleCol(Triangle t, float ballx, float bally)
{
    float s1 = t.vertex3[1] - t.vertex1[1];
	float s2 = t.vertex3[0] - t.vertex1[0];
	float s3 = t.vertex2[1] - t.vertex1[1];
	float s4 = bally - t.vertex1[1];

	float w1 = (t.vertex1[0] * s1 + s4 * s2 - ballx * s1) / (s3 * s2 - (t.vertex2[0]-t.vertex1[0]) * s1);
	float w2 = (s4- w1 * s3) / s1;
	return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
}
