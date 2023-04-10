//modified by Xander Reyes
////date: Spring 2022
#include <string>
#include <math.h>
#include "xreyes.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"

using namespace std;
int alex_feature;
float velocity[2] = {0.0f, 0.0f};
bool summonball = false;
Triangle::Triangle() 
{
    vertex1[0] = 0;
    vertex2[0] = 0;
    vertex3[0] = 0;
    vertex1[1] = 0;
    vertex2[1] = 0;
    vertex3[1] = 0;

}

Triangle::Triangle(float vertx1, float vertx2, float vertx3,
	float verty1, float verty2, float verty3) 
{
    vertex1[0] = vertx1;
    vertex2[0] = vertx2;
    vertex3[0] = vertx3;
    vertex1[1] = verty1;
    vertex2[1] = verty2;
    vertex3[1] = verty3;
}
void box_collision(int ballx, int bally, int boxx, int boxy, 
	int w, int h, float *vx, float *vy)
{
    if (bally <= boxy + h &&     	// top 
	    ballx  >= boxx - w &&  	// left
	    ballx <= boxx + w &&   	// right
	    bally >= boxy - h) {     	// bottom
	if (bally <= boxy + h - 10 &&
		bally >= boxy - h + 10)
	    *vx = -*vx;

	if (bally <= boxy + h &&    // top 
		ballx >= boxx - w + 10 &&
		ballx <= boxx + w - 10 &&
		bally >= boxy - h)
	    *vy = -*vy * 0.7;

    }
}

void circle_collision(int ballx, int bally, float cx, float cy, 
	float r, float *vx, float *vy)
{
	 int dist2,xd2,yd2;
        xd2 = ballx - cx;
        yd2 = bally - cy;
        dist2 = sqrt((xd2*xd2)+(yd2*yd2));
        if (dist2 <= r) {
            if (ballx < cx) {
                if (*vy > 0) {
                *vx = -dist2 * 0.1;
                }
                else {
                *vy =  -*vy * 0.1;
                *vx = -dist2 * 0.1;
                }
            } else {

                if (*vy > 0) {
                *vx = dist2 * 0.1;
                }
                else {
                *vy =  -*vy * 0.1;
                *vx = -dist2 * 0.1;
                }
            }

        }

}

void draw_triangle(Triangle triangle)
{
	glPushMatrix();
    glBegin(GL_TRIANGLES);
    glColor3f(0.5,0,0);
    glVertex2f(triangle.vertex1[0],triangle.vertex1[1]);
    glVertex2f(triangle.vertex2[0],triangle.vertex2[1]);
    glVertex2f(triangle.vertex3[0],triangle.vertex3[1]);
    glEnd();


}
