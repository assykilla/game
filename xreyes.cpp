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
int score = 0;
int lives = 3;
double point = 0.00;

Triangle::Triangle() 
{
    vertex1[0] = 0;
    vertex2[0] = 0;
    vertex3[0] = 0;
    vertex1[1] = 0;
    vertex2[1] = 0;
    vertex3[1] = 0;
    angle = 0.0f;

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
    angle = 0.0f;
}
void Triangle::set_color(unsigned char col[3]) 
{
    memcpy(color, col, sizeof(unsigned char) * 3);
}

Triangle t1 = Triangle(450.0f,375.0f,450.0f,
	495.0f, 495.0f, 350.0f);
Triangle t2 = Triangle(20.0f,20.0f,120.0f,
	450.0f, 300.0f, 450.0f);
Triangle flipper1 = Triangle(105.0f, 105.0f, 167.0f,
	30.0f, 8.0f, 8.0f);
Triangle flipper2 = Triangle(235.0f, 173.0f, 235.0f,
	30.0f, 8.0f, 8.0f);
Triangle t3 = Triangle(20.0f, 20.0f, 107.0f,
	95.0f, 30.0f, 30.0f);
Triangle t4 = Triangle(310.0f, 233.0f, 310.0f,
	95.0f, 30.0f, 30.0f);
Triangle t5 = Triangle(20.0f, 20.0f, 120.0f,
	540.0f, 450.0f, 450.0f);
Triangle t6 = Triangle(450.0f, 375.0f, 450.0f,
	600.0f, 495.0f, 495.0f);

void box_collision(float *ballx, float *bally, int ballw,
	int boxx, int boxy, int w, int h, float *vx, float *vy)
{
    if (*bally - ballw <= boxy + h &&     	// top 
	    *ballx  >= boxx - w &&  	// left
	    *ballx <= boxx + w &&   	// right
	    *bally + ballw >= boxy - h) {     	// bottom
	if (*bally <= boxy + h - 8 &&
		*bally >= boxy - h + 8) {
	    if (*ballx > boxx)
		*ballx = boxx + w + 5;
	    else
		*ballx = boxx - w - 5;
	    *vx = -*vx;
	}
	if (*bally - ballw <= boxy + h &&    // top 
		*ballx >= boxx - w + 8 &&
		*ballx <= boxx + w - 8 &&
		*bally + ballw >= boxy - h) {
	    if (*bally > boxy)
		*bally = boxy + h + 5;
	    else
		*bally = boxy - h - 5;
	    *vy = -*vy * 0.7;
	}

    }
}

void circle_collision(float *ballx, float *bally, float cx, float cy, 
	float r, float *vx, float *vy)
{
    int dist2,xd2,yd2;
    xd2 = *ballx - cx;
    yd2 = *bally - cy;
    dist2 = sqrt((xd2*xd2)+(yd2*yd2));
    if (dist2 <= r) {
	score += 100;
	if ( *bally >= cy + 1 && *bally <= cy - 1) {
	    if (r < 20)
		*vx = -dist2 * 0.4;
	    else
		*vx = -dist2 * 0.07;
	}
	else if ( *ballx >= cx + 1 && *ballx <= cx -1)
	    *vy = -*vy;
	else if ( *bally > cy) {
	    if (*ballx < cx) {
		if (*vy > 0) {
		    if (r < 20)
			*vx = -dist2 * 0.4;
		    else
			*vx = -dist2 * 0.07;
		}
		else {
		    *vy =  -*vy * 0.3;
		    if (r < 20)
			*vx = -dist2 * 0.4;
		    else
			*vx = -dist2 * 0.07;
		}
		// *ballx = *ballx - 5;
	    } else { //ballx > cx && bally > cy
		if (*vy > 0) {
		    if (r < 20)
			*vx = dist2 * 0.4;
		    else
			*vx = dist2 * 0.07;
		}
		else {
		    *vy =  -*vy * 0.3;
		    if (r < 20)
			*vx = dist2 * 0.4;
		    else
			*vx = dist2 * 0.07;
		}
		// *ballx = *ballx + 5;
	    }
	    //    *bally = *bally + 5;
	}
	else { //bally < cy
	    if (*ballx < cx) {
		//*ballx = *ballx + 5 ;
		//*bally = *bally - 5 ;
		if (*vy > 0) {
		    *vy =  -*vy * 0.3;
		    if (r < 20)
			*vx = -dist2 * 0.4;
		    else
			*vx = -dist2 * 0.07;

		}
		else {
		    if (r < 20)
			*vx = -dist2 * 0.4;
		    else
			*vx = -dist2 * 0.07;
		}
	    } else {    //ballx > cx && bally < cy
			// *ballx = *ballx + 5 ;
			// *bally = *bally - 5 ;
		if (*vy > 0) {
		    *vy =  -*vy * 0.3;
		    if (r < 20)
			*vx = dist2 * 0.4;
		    else
			*vx = dist2 * 0.07;
		}
		else {
		    if (r < 20)
			*vx = dist2 * 0.4;
		    else
			*vx = dist2 * 0.07;
		}
	    }
	}
    }
}

void draw_triangle(Triangle triangle, unsigned char color[])
{
    glPushMatrix(); 
    triangle.set_color(color);
    glColor3ubv(triangle.color);
    glRotatef(triangle.angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0);
    glVertex2f(triangle.vertex1[0],triangle.vertex1[1]);
    glTexCoord2f(1, 0);
    glVertex2f(triangle.vertex2[0],triangle.vertex2[1]);
    glTexCoord2f(0, 1);
    glVertex2f(triangle.vertex3[0],triangle.vertex3[1]);
    glEnd();
    glPopMatrix();
}

void draw_circle(float r, float cx, float cy, unsigned char color[])
{
    float x, y;
    int n = 50;
    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    glColor3ubv(color);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
	x = r*cos(angle);    
	y = r*sin(angle);    
	glTexCoord2f(1, 0);
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

void triangle_collision( Triangle triangle, float *ballx, float *bally, 
	float *vx, float *vy)
{
    float u,w,v,x,y;
    float theta, alpha, hyp, opp;
    if ( ((*ballx <= triangle.vertex1[0] && *ballx >= triangle.vertex2[0]) ||
		(*ballx <= triangle.vertex3[0] && *ballx >= triangle.vertex1[0])) &&
	    *bally <= triangle.vertex1[1] && *bally >= triangle.vertex3[1]) {

	u = vector(triangle.vertex1[0], triangle.vertex1[1], 
		triangle.vertex2[0], triangle.vertex2[1]);
	v = vector(triangle.vertex1[0], triangle.vertex1[1], 
		triangle.vertex3[0], triangle.vertex3[1]);
	w = vector(triangle.vertex2[0], triangle.vertex2[1], 
		triangle.vertex3[0], triangle.vertex3[1]);

	if ( w > u && w > v) {
	    opp = v;
	    hyp = w;
	    x = vector(triangle.vertex2[0],triangle.vertex2[1], *ballx, *bally);
	    y = (triangle.vertex1[1]-*bally);
	}
	else if ( u > w && u > v) { 	//flipper2
	    opp = v;
	    hyp = u;
	    x = vector(triangle.vertex2[0],triangle.vertex2[1], *ballx, *bally);
	    //y = (triangle.vertex2[1]-*bally);
	    y = (*bally - triangle.vertex3[1] - 4);
	}
	else { 				//flipper1
	    opp = u; 
	    hyp = v;
	    x = vector(triangle.vertex3[0],triangle.vertex3[1], *ballx, *bally);
	    //y = (triangle.vertex3[1]-*bally);
	    y = (*bally - triangle.vertex3[1] - 4);
	}
	theta = asin(opp/hyp);
	alpha = asin(y/x);
	if (alpha <= theta ) {
	    if (hyp == w && *vx == 0) 
		*vx = -*vy;
	    else if (*vy == 0 && hyp == w)
		*vy = -*vx;
	    else if (hyp == w && *vy < 0) {
		*vx = -*vx * 0.9;
	    }
	    else if (hyp == u) {     //flipper2
		if (*vx > 0 && *vy <= -5.0f) {
		    *vx = -*vx * 0.9;
		    *vy = -*vy * 0.8;
		    *bally = *bally + 2;
		}
		else if ( *vx < 0 && *vy <= -5.0f) { 
		    *vy = -*vy; 
		    *bally = *bally + 2;
		}
		else if ( *vy > -5.0f && *vy < 0){
		    if (*vx > 0)
			*vx = -*vx * 0.7;
		    *vx = *vx - 0.05;
		    *vy = *vy + 0.125;
		    *bally = *bally + 2;

		}
		else if (*vx == 0) {
		    *bally = *bally + 2; 
		    *vx = *vy;
		}
		else if (*vy == 0)
		    *vy = -*vx;
		else 
		    *vy = -*vy;
	    }
	    else if ( hyp == v) {    //flipper1
		if (*vx > 0 && *vy <= -5.0f) {
		    *vy = -*vy * 0.8;
		    *bally = *bally + 2;
		}
		else if ( *vx < 0 && *vy <= -5.0f) {
		    *vx = -*vx * 0.9;
		    *vy = -*vy * 0.8;
		    *bally = *bally + 2; 
		}
		else if ( *vy > -5.0f && *vy < 0) {
		    if (*vx < 0)
			*vx = -*vx * 0.7;
		    *vx = *vx + 0.05;
		    *vy = *vy + 0.125;
		    *bally = *bally + 2; 
		}
		else 
		    *vy = -*vy * 0.8;
	    }
	    else { 		// t1

		if (*vx == 0) 
		    *vx = -*vy;
		else if (*vy == 0)
		    *vy = -*vx;
		else if (*vy < 0)
		    *vx = -*vx;
		else if (*vx < 0 && *vx > -1.5f)
		    *vx = -*vx;
		else {
		    *vx = -*vx;
		    *vy = -*vy;	
		}
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
	    else if (*vy < 0)
		*vx = -*vx;
	    else if (*vx < 0 && *vx > -1.5f)
		*vx = -*vx;
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
void show_stats(int score, int lives) 
{
    Rect scoreboard;
    scoreboard.bot = 345;
    scoreboard.left = 500;
    scoreboard.center = 0;
    ggprint8b(&scoreboard, 20, 0x00ffff00, "Score: %i", score);

    Rect life;
    life.bot = 295;
    life.left = 500;
    life.center = 0;
    ggprint8b(&life, 20, 0x00ffff00, "Lives: %i", lives);
}
