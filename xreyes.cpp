//modified by Xander Reyes
////date: Spring 2022
#include <iostream>
#include <string>
#include <math.h>
#include "xreyes.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <cstring>
#include <unistd.h>

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
        if (bally <= boxy + h - 8 &&
                bally >= boxy - h + 8)
            *vx = -*vx;

        if (bally <= boxy + h &&    // top 
                ballx >= boxx - w + 8 &&
                ballx <= boxx + w - 8 &&
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
        if ( bally > cy) {
            if (ballx < cx) {
                if (*vy > 0) {
                    if (r <20)
                        *vx = -dist2 * 0.4;
                    else
                        *vx = -dist2 * 0.075;
                }
                else {
                    *vy =  -*vy * 0.1;
                    if (r <20)
                        *vx = -dist2 * 0.4;
                    else
                        *vx = -dist2 * 0.075;
                }
            } else {

                if (*vy > 0) {
                    if (r <20)
                        *vx = dist2 * 0.4;
                    else
                        *vx = dist2 * 0.075;
                }
                else {
                    *vy =  -*vy * 0.1;
                    if (r <20)
                        *vx = -dist2 * 0.4;
                    else
                        *vx = -dist2 * 0.075;
                }
            }
        }
        else {
            if (ballx < cx) {
                if (*vy > 0) {
                    if (r <20)
                        *vx = -dist2 * 0.4;
                    else
                        *vx = -dist2 * 0.075;

                    *vy =  -*vy * 0.07;
                }
                else {
                    if (r <20)
                        *vx = -dist2 * 0.4;
                    else
                        *vx = -dist2 * 0.075;
                }
            } else {

                if (*vy > 0) {
                    if (r <20)
                        *vx = -dist2 * 0.4;
                    else
                        *vx = -dist2 * 0.075;

                    *vy =  -*vy * 0.075;
                }
                else {
                    if (r <20)
                        *vx = dist2 * 0.4;
                    else
                        *vx = dist2 * 0.075;
                }
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

void draw_circle(float r, float cx, float cy)
{
    float x, y;
    int n = 50;
    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    glColor3ub(255,255,100);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
        x = r*cos(angle);    
        y = r*sin(angle);    
        glVertex2f(x+cx, y+cy);
        angle += inc;
    }
    glEnd();
}

float vector( float x1, float y1, float x2, float y2)
{
    float run = x2 - x1;
    float rise = y2 - y1;

    float vect = sqrt((run*run) + (rise*rise));

    return vect;
}

void triangle_collision( Triangle triangle, float ballx, float bally, 
        float *vx, float *vy)
{
    float u,w,v,x,y;
    float theta, alpha, hyp, opp;
    if ( ((ballx <= triangle.vertex1[0] && ballx >= triangle.vertex2[0]) ||
                (ballx <= triangle.vertex3[0] && ballx >= triangle.vertex1[0])) &&
            bally <= triangle.vertex1[1] && bally >= triangle.vertex3[1]) {

        u = vector(triangle.vertex1[0], triangle.vertex1[1], 
                triangle.vertex2[0], triangle.vertex2[1]);
        v = vector(triangle.vertex1[0], triangle.vertex1[1], 
                triangle.vertex3[0], triangle.vertex3[1]);
        w = vector(triangle.vertex2[0], triangle.vertex2[1], 
                triangle.vertex3[0], triangle.vertex3[1]);

        if ( w > u && w > v) {
            opp = v;
            hyp = w;

            x = vector(triangle.vertex2[0],triangle.vertex2[1], ballx, bally);
            y = (triangle.vertex1[1]-bally);
        }
        else if ( u > w && u > v) {
            opp = v;
            hyp = u;
            x = vector(triangle.vertex2[0],triangle.vertex2[1], ballx, bally);
            y = (triangle.vertex2[1]-bally);
        }
        else {
            opp = u; 
            hyp = v;
            x = vector(triangle.vertex3[0],triangle.vertex3[1], 
                    ballx, bally);
            y = (triangle.vertex2[1]-bally);
        }
        theta = asin(opp/hyp);
        alpha = asin(y/x);
        //float v1,v2;
        if (alpha <= theta) {
            /*if (hyp == v) {
              v1 = x;
              v2 = triangle.vertex2[1] - bally;
              angle = asin(v2/v1); 
              } 
              else if (hyp == w) {
              v1 = x;
              v2 = triangle.vertex1[1] - bally;
              angle = asin(v2/v1);   
              }*/
            if (*vx == 0) 
                *vx = -*vy;
            else if (*vy == 0)
                *vy = -*vx;
            else {
                *vx = -*vx ;
                *vy = -*vy ;
            }


        }

    }
}

void xtriangle_collision( Triangle triangle, float ballx, float bally, 
        float *vx, float *vy)
{
    float opp, hyp,x,y;
    float theta, alpha;

    if ( ballx >= triangle.vertex1[0] && ballx <= triangle.vertex3[0] && 
           bally <= triangle.vertex1[1] && bally >= triangle.vertex2[1]) {

        opp = vector(triangle.vertex1[0], triangle.vertex1[1], 
                triangle.vertex2[0], triangle.vertex2[1]);
        hyp = vector(triangle.vertex2[0], triangle.vertex2[1], 
                triangle.vertex3[0], triangle.vertex3[1]);

        x = vector(triangle.vertex3[0],triangle.vertex3[1], ballx, bally);
        y = (triangle.vertex1[1]-bally);

        theta = asin(opp/hyp);
        alpha = asin(y/x);
         if (alpha <= theta) {
            if (*vx == 0) 
                *vx = -*vy;
            else if (*vy == 0)
                *vy = -*vx;
            else {
                *vx = -*vx ;
                *vy = -*vy ;
            }
        }
    }
}


Image::~Image() { delete [] data; }
Image::Image(const char *fname) {

    if (fname[0] == '\0')
	return;
    int ppmFlag = 0;
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];
    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
	ppmFlag = 1;
    if (ppmFlag) {
	strcpy(ppmname, name);
    } else {
	name[slen-4] = '\0';
	sprintf(ppmname,"%s.ppm", name);
	char ts[100];
	sprintf(ts, "convert %s %s", fname, ppmname);
	system(ts);
    }
    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
	char line[200];
	fgets(line, 200, fpi);
	fgets(line, 200, fpi);
	//skip comments and blank lines
	while (line[0] == '#' || strlen(line) < 2)
	    fgets(line, 200, fpi);
	sscanf(line, "%i %i", &width, &height);
	fgets(line, 200, fpi);
	//get pixel data
	int n = width * height * 3;
	data = new unsigned char[n];
	for (int i=0; i<n; i++)
	    data[i] = fgetc(fpi);
	fclose(fpi);
    } else {
	printf("ERROR opening image: %s\n",ppmname);
	exit(0);
    }
    if (!ppmFlag)
        unlink(ppmname);
}
