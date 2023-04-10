//modified by Xander Reyes
////date: Spring 2022
#include <string>
#include "xreyes.h"
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
