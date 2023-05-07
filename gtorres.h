#ifndef _GEORGE_T_
#define _GEORGE_T_


extern bool TriangleCol(Triangle, float, float);
extern bool leftFlipper;
extern bool rightFlipper;
extern Rect stats[1];
extern int points;
extern int credit;
extern int currentBalls;
extern int level1;
extern int level2;
extern void addScore(int amount);
extern int leftFlipperFrame;
extern int rightFlipperFrame;
extern float leftAngle;
extern float rightAngle;
extern int flipperFrames;
extern int x_rotated;
extern int y_rotated;
extern int noBalls;
const int MAX_BALLS = 3;
extern bool debugGeorge;
extern bool saviorActive;
//extern void rotateFlippersL(bool leftFlipper, int leftFlipperAngle);
//extern void rotateFlippersR(bool rightFlipper, int rightFlipperAngle);

extern void debugMode(unsigned int mapNum, bool *summoned, float *px, float *py, float *vx, float *vy);
extern bool makeBall(unsigned int map, bool *summoned, float *px, float *py, float *vy);
extern void flipperRotate(unsigned int map);
extern void renderStats(Rect r, int x, int y, int pts);
extern void rotate_point(float angle, float *x, float *y, float cx, float cy);
extern void flipping(unsigned int map, float *ballx, float *bally, float *ballvx, float *ballvy);
extern void make_particle(int, int);
extern Triangle GflipL;
extern Triangle GflipR;
extern Triangle Gt1;
extern Triangle Gt2;
extern Triangle Gt3;
extern Triangle Gt4;
extern Triangle Gt5;
extern Triangle Gt6;
extern Triangle Gt7;
extern Triangle Gt8;
extern Triangle Gt9;
extern Triangle Gt10;
extern Triangle Gt11;
extern Triangle Gt12;


#endif