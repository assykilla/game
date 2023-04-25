//modified by Xander Reyes, Andres Botello
//date:
//
//author: Gordon Griesel
//date: Spring 2022
//purpose: get openGL working on your personal computer
//
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "abotello.h"
#include "isamara.h"
#include "xreyes.h"
#include "gtorres.h"
//some structures
Image img("dinosaurs.jpeg");

class Global {
    public:
	int xres, yres;
	int n;
    GLuint texture;
	unsigned int pause;
	unsigned int mainmenu;
	unsigned int issa_feature;
	Global(){
	    xres = 800;
	    yres = 800;
	    n = 0;
	    pause = 0;
	    mainmenu = 0;
	    issa_feature = 0;

	}
} g;
extern int alex_feature;
extern float velocity[2];
extern bool summonball;
class Box {
    public:
	float w;
	float h;
	float pos[2];
	float vel[2];
	unsigned char color[3];
	void set_color ( unsigned char col[3]) {
	    memcpy(color, col, sizeof(unsigned char) *3);

	}
	//float boxr=150.0f/255.0f, boxg=160.0f/255.0f, boxb=220.0f/255.0f;
	Box(){
	    w = 80.0f;
	    h = 15.0f;
	    pos[0] = 90;
	    pos[1] = 290;
	    vel[0] = 0.0f;
	    vel[1] = 0.0f;
	}
	Box(float wid, float hgt, int x, int y, float v0, float v1){
	    w = wid;
	    h = hgt;
	    pos[0] = x;
	    pos[1] = y;
	    vel[0] = v0;
	    vel[1] = v1;
	}
} highbox1(50.0f,335.0f, 310, 10, 0.0f, 0.0f), 
    ball(4.0f,4.0f, 380, 100, velocity[0], velocity[1]),
    highbox2(20.0f,float(g.yres), 0, 10, 0.0f, 0.0f);
class Circle {
    public:
	float r, c[2], x, y, vel[2];
	unsigned char color[3];
	void set_color ( unsigned char col[3]) {
	    memcpy(color, col, sizeof(unsigned char) *3);

	}
	Circle(){
	    r = 70.0f;
	    c[0] = 600.0f;
	    c[1] = 25.0f;
	    vel[0] = 0.0f;
	    vel[1] = 0.0f;
	}
	Circle(float radius, int cx,int cy, float v0, float v1) {
	    r = radius;
	    c[0] = cx;
	    c[1] = cy;
	    vel[0] = v0;
	    vel[1] = v1;
	}

} circle, test_ball(10,g.xres/2,g.yres/2,0,0), 
  halfcir(50.0f, 310 ,335, 0.0f, 0.0f),
  cir1(12.0f, 135 ,235, 0.0f, 0.0f),
  cir2(12.0f, 105 ,210, 0.0f, 0.0f),
  cir3(12.0f, 165 ,210, 0.0f, 0.0f);


class X11_wrapper {
    private:
	Display *dpy;
	Window win;
	GLXContext glc;
    public:
	~X11_wrapper();
	X11_wrapper();
	void set_title();
	bool getXPending();
	XEvent getXNextEvent();
	void swapBuffers();
	void reshape_window(int width, int height);
	void check_resize(XEvent *e);
    void check_mouse(XEvent *e);
	int check_keys(XEvent *e);
} x11;

//FunctiCircle(float wid, float hgt, int x, int y, float v0, float v1){
void init_opengl(void);
void physics(void);
void render(void);



//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main()
{
    init_opengl(); 
    //Main loop
    int done = 0;
    while (!done) {
	//Process external events.
	while (x11.getXPending()) {
	    XEvent e = x11.getXNextEvent();
	    x11.check_resize(&e);
	    x11.check_mouse(&e);
	    done = x11.check_keys(&e);
	    if (g.mainmenu == 4)
		done = 1;
	}
	physics();
	render();
	x11.swapBuffers();
	usleep(2000);
    }
    cleanup_fonts();
    return 0;
}

X11_wrapper::~X11_wrapper()
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper()
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w = g.xres, h = g.yres;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
	cout << "\n\tcannot connect to X server\n" << endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
	cout << "\n\tno appropriate visual found\n" << endl;
	exit(EXIT_FAILURE);
    }  
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask =
	ExposureMask | KeyPressMask | KeyReleaseMask |
	ButtonPress | ButtonReleaseMask |
	PointerMotionMask |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
	    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);

    glXMakeCurrent(dpy, win, glc);
}

void X11_wrapper::set_title()
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "3350 Lab2");
}

bool X11_wrapper::getXPending()
{
    //See if there are pending events.
    return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
    //Get a pending event.
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}

void X11_wrapper::swapBuffers()
{
    glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
    //window has been resized.
    g.xres = width;
    g.yres = height;
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
	return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != g.xres || xce.height != g.yres) {
	//Window size did change.
	reshape_window(xce.width, xce.height);
    }
}
//-----------------------------------------------------------------------------

float rnd()
{
    float r = ((float)rand() / (float)RAND_MAX);
    return r;
}

void X11_wrapper::check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;

    //Weed out non-mouse events
    if (e->type != ButtonRelease &&
	    e->type != ButtonPress &&
	    e->type != MotionNotify) {
	//This is not a mouse event that we care about.
	return;
    }
    //
    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    if (g.mainmenu == 0) {
		g.mainmenu = select_option(e->xbutton.x, g.yres - e->xbutton.y);
	    } else if (g.mainmenu == 1) {
		if (g.issa_feature) {
		    push_button(&test_ball.vel[0], g.xres, g.yres);
		}
	    }



	    //Left button was pressed.
	    //int y = g.yres - e->xbutton.y;
	    //make_particle(e->xbutton.x, g.yres - e->xbutton.y);
	    return;
	}
	if (e->xbutton.button==3) {
	    //Right button was pressed.
	    return;
	}
    }
    if (e->type == MotionNotify) {
	//The mouse moved!
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
	    if (!g.pause){
		savex = e->xbutton.x;
		savey = e->xbutton.y;
		//Code placed here will execute whenever the mouse moves.
	    }
	}
    }
}

void triangleCollisionLU()
{
    float temp = ball.vel[0];
    ball.vel[0] = ball.vel[1]-1;
    ball.vel[1] = temp-1;
}
void triangleCollisionLD()
{
    float temp = ball.vel[1];
    ball.vel[1] = ball.vel[0]-1;
    ball.vel[0] = temp-1;
}
void triangleCollisionRU()
{
    float temp = ball.vel[0];
    ball.vel[0] = ball.vel[1]-1;
    ball.vel[1] = temp-1;
}
void triangleCollisionRD()
{
    float temp = ball.vel[0];
    ball.vel[0] = ball.vel[1]-1;
    ball.vel[1] = temp-1;
}
int X11_wrapper::check_keys(XEvent *e)
{
    leftFlipper = 0;
    rightFlipper = 0;
    if (e->type != KeyPress && e->type != KeyRelease)
	return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
	switch (key) {
	    case XK_p:
		g.pause = manage_pstate(g.pause);
		break;
		if (XK_Shift_L)
		    // Key 2 and shift are both pressed down
		{
		    if (alex_feature == 0)
			alex_feature = 1;
		    else 
			alex_feature = 0;
		}
		break;
	    case XK_3:
		if (XK_Shift_L && g.mainmenu == 1) {
		    g.issa_feature = !g.issa_feature;
		}
		break;
	    case XK_space:
		summonball = true;
		velocity[1] = 8.0f;
		ball.vel[1] = velocity[1];
		break;
	    case XK_e:
        case XK_2:
        if (level1 == 1) {
            level2 = 1;
            level1 = 0;
        }
        else {
            level1 = 1;
            level2 = 0;
        }
        break;
		if (XK_Shift_L && g.mainmenu != 0) {
		    g.mainmenu = 0;
		}
		break;
            case XK_Left:
                leftFlipper = 1;
                break;
            case XK_Right:
                rightFlipper = 1;
                break;
	    case XK_Escape:
		//Escape key was pressed
		return 1;
	}
    }
    return 0;
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Set the screen background color
    glClearColor(0.2, 0.5, 0.2, 0.3);
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    glGenTextures(1, &g.texture);
    int w = img.width;
    int h = img.height;
    glBindTexture(GL_TEXTURE_2D, g.texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
        GL_RGB, GL_UNSIGNED_BYTE, img.data);
}
float Gravity = 0.075;
Triangle t1 = Triangle(400.0f,350.0f,400.0f,
	450.0f, 450.0f, 300.0f);
Triangle t2 = Triangle(20.0f,20.0f,80.0f,
	450.0f, 300.0f, 450.0f);
Triangle flipper1 = Triangle(80.0f, 80.0f, 130.0f,
	25.0f, 8.0f, 8.0f);
Triangle flipper2 = Triangle(200.0f, 150.0f, 200.0f,
	25.0f, 8.0f, 8.0f);


void physics()
{
    if(!g.pause){
	if (summonball) {
	    if (ball.vel[0] <= 9.0f || ball.vel[1] <= 9.0f) {
		ball.pos[0] += ball.vel[0];
		ball.pos[1] += ball.vel[1];
		ball.vel[1] -= Gravity;
	    }
	}
    }
    if(g.issa_feature) {
	if (test_ball.vel[0] < 0) {
	    test_ball.vel[0] = 0;
	}
	if (test_ball.vel[0] > 0) {
	    test_ball.vel[0] -= Gravity;
	}
    }

    /*for (int i = 0; i < g.n; i++) {
      particle[i].pos[0] += particle[i].vel[0];
      particle[i].pos[1] += particle[i].vel[1];
      particle[i].vel[1] -= Gravity;
    //check if particle went off screen
    if (particle[i].pos[1] < 0.0 || particle[i].pos[0] > g.xres) {
    // optimizing this code below
    // particle[i] = particle [g.n-1];
    // g.n= g.n-1;
    // optimized
    particle[i] = particle [--g.n];
    }
    }
    */


    //check for collision between particles and boxes
    /*for (int j = 0; j < 5; j++) {
      if (particle[i].pos[1] < box[j].pos[1]+box[j].h && 
      particle[i].pos[0] > box[j].pos[0]-box[j].w &&
      particle[i].pos[0] < box[j].pos[0]+box[j].w &&
      particle[i].pos[1] > box[j].pos[1]-box[j].h)
      {
      particle[i].vel[1] =  -particle[i].vel[1] * 0.3; 
      particle[i].vel[0] += 0.01; 
      }

      }
      */
    /*if (ball.pos[1] < flipper.pos[1]+ flipper.h && 
      ball.pos[0] > flipper.pos[0]-flipper.w &&
      ball.pos[0] < flipper.pos[0]+flipper.w &&
      ball.pos[1] > flipper.pos[1]-flipper.h)
      {
      ball.vel[1] =  -ball.vel[1] * 0.7; 
      ball.vel[0] += 0.03; 
      }
      */

     if (level1 == 1) {
    extern void box_collision(int ballx, int bally, int boxx, int boxy,
	    int w, int h, float *vx, float *vy);
    
    box_collision(ball.pos[0], ball.pos[1], highbox1.pos[0], highbox1.pos[1],
	    highbox1.w, highbox1.h, &ball.vel[0], &ball.vel[1]);
    box_collision(ball.pos[0], ball.pos[1], highbox2.pos[0], highbox2.pos[1],
	    highbox2.w, highbox2.h, &ball.vel[0], &ball.vel[1]);

    extern void circle_collision(int ballx, int bally, float cx, float cy,
	    float r, float *vx, float *vy);

    circle_collision(ball.pos[0], ball.pos[1],circle.c[0], circle.c[1],
	    circle.r, &ball.vel[0], &ball.vel[1]);

    circle_collision(ball.pos[0], ball.pos[1],halfcir.c[0], halfcir.c[1],
	    halfcir.r, &ball.vel[0], &ball.vel[1]);

    circle_collision(ball.pos[0], ball.pos[1],cir1.c[0], cir1.c[1],
	    cir1.r, &ball.vel[0], &ball.vel[1]);
    circle_collision(ball.pos[0], ball.pos[1],cir2.c[0], cir2.c[1],
	    cir2.r, &ball.vel[0], &ball.vel[1]);
    circle_collision(ball.pos[0], ball.pos[1],cir3.c[0], cir3.c[1],
	    cir3.r, &ball.vel[0], &ball.vel[1]);

    /* For hypotenus facing left downwards and facing upwards */
    extern void triangle_collision( Triangle triangle, float ballx,
            float bally, float *vx, float *vy);
    triangle_collision( t1, ball.pos[0], ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( flipper1, ball.pos[0], ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    triangle_collision( flipper2, ball.pos[0], ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    /* For hypotenus facing right downwards */
    extern void xtriangle_collision( Triangle triangle, float ballx,
            float bally, float *vx, float *vy);
    xtriangle_collision( t2, ball.pos[0], ball.pos[1],
            &ball.vel[0], &ball.vel[1]);
    // Checking for wall collision
    if (ball.pos[0] - ball.w < 0)
	ball.vel[0] = -ball.vel[0];

    else if (ball.pos[0] + ball.w > g.xres)
	ball.vel[0] = -ball.vel[0];

    if (ball.pos[1] + ball.h > g.yres)
	ball.vel[1] = -ball.vel[1];

     }

     if (level2 == 1) {
                if (ball.pos[0] - ball.w < 0) {
            ball.vel[0] = -ball.vel[0];
            points += 5;
        }

        else if (ball.pos[0] + ball.w > g.xres)
            ball.vel[0] = -ball.vel[0];

        if (ball.pos[1] + ball.w > g.yres)
            ball.vel[1] = -ball.vel[1];

        if (TriangleCol(Gt1, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionRU();
           points += 5;
        }

        if (TriangleCol(Gt2, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionLU();
           points += 5;
           }

        if (TriangleCol(Gt3, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionLD();
           points += 5;
           }

        if (TriangleCol(Gt4, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionRD();
           points += 5;
           }
        
        if (TriangleCol(Gt5, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionLD();
           points += 5;
           }

        if (TriangleCol(Gt6, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionRD();
           points += 5;
           }

        if (TriangleCol(Gt7, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionRU();
           points += 5;
           }

        if (TriangleCol(Gt8, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionRD();
           points += 5;
           }

        if (TriangleCol(Gt9, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionLU();
           points += 5;
           }

        if (TriangleCol(Gt10, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionRU();
           points += 5;
           }

        if (TriangleCol(Gt11, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionRU();
           points += 5;
           }

        if (TriangleCol(Gt12, ball.pos[0], ball.pos[1]) == 1) {
           triangleCollisionLU();
           points += 5;
           }
     }


}
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (g.mainmenu == 0) {
	prompt titlescreen[4];
	Rect titleprompt[5];
	for (int i=0; i<4; i++) {
	    render_menu(titlescreen[i], titleprompt[i], i, g.xres, g.yres);
	}
	render_title(titleprompt[5],g.xres,g.yres);
	return;
    } else if (g.mainmenu == 4) {
	return;
    }


    if (g.issa_feature) {
	int n = 20;
	double angle = 0.0;
	double inc = (2.0*3.14)/n;
	glColor3ub(255,255, 100);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < n; i++) {
	    test_ball.x = test_ball.r*cos(angle);
	    test_ball.y = test_ball.r*sin(angle);
	    glVertex2f(test_ball.x+test_ball.c[0],test_ball.y + test_ball.c[1]);
	    angle += inc;
	}
	glEnd();
	draw_button(g.xres,g.yres);
    }
    //glClear(GL_COLOR_BUFFER_BIT);
    // Draw Box	
    //highbox = Box(10.0f,325.0f, 350, 10, 0.0f, 0.0f);

    if (level1 == 1) {
    glPushMatrix();
    glColor3ub(255,100,255);
    glTranslatef(highbox1.pos[0], highbox1.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-highbox1.w, -highbox1.h);
    glVertex2f(-highbox1.w,  highbox1.h);
    glVertex2f( highbox1.w,  highbox1.h);
    glVertex2f( highbox1.w, -highbox1.h);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(255,100,255);
    glTranslatef(highbox2.pos[0], highbox2.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-highbox2.w, -highbox2.h);
    glVertex2f(-highbox2.w,  highbox2.h);
    glVertex2f( highbox2.w,  highbox2.h);
    glVertex2f( highbox2.w, -highbox2.h);
    glEnd();
    glPopMatrix();

    extern void draw_triangle(Triangle triangle);
    /* create triangle*/
    draw_triangle(t1);
    draw_triangle(t2);
    /* create right flipper*/
    draw_triangle(flipper1);
    /* create left flipper*/
    draw_triangle(flipper2);

    /*//Draw Circle
    int n = 20; 
    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    //glVertex2f(x, y);
    glColor3ub(255,255, 100);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
	circle.x = circle.r*cos(angle);
	circle.y = circle.r*sin(angle);
	glVertex2f(circle.x+circle.c[0],circle.y + circle.c[1]);
	angle += inc;
    }*/

    glEnd();
    //Draw Half Circle
    int n = 40; 

    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    //glVertex2f(x, y);
    glColor3ub(255,100, 255);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n/2+1; i++) {
	halfcir.x = halfcir.r*cos(angle);
	halfcir.y = halfcir.r*sin(angle);
	glVertex2f(halfcir.x+halfcir.c[0],halfcir.y + halfcir.c[1]);
	angle += inc;
    }
    glEnd();

    extern void draw_circle(float r, float cx, float cy);
    draw_circle(cir1.r,cir1.c[0],cir1.c[1]);
    draw_circle(cir2.r,cir2.c[0],cir2.c[1]);
    draw_circle(cir3.r,cir3.c[0],cir3.c[1]);

    glBindTexture(GL_TEXTURE_2D, g.texture);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 1.0f); glVertex2i(400, 0);
    glTexCoord2f( 0.0f, 0.0f); glVertex2i(400,  g.yres);
    glTexCoord2f( 1.0f, 0.0f); glVertex2i( g.xres,  g.yres);
    glTexCoord2f( 1.0f, 1.0f); glVertex2i( g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);


    }

    if (level2 == 1) {
    
    glClear(GL_COLOR_BUFFER_BIT);

    Box start = Box(5.0f, 350.0f, 550.0f, 300.0f, 0.0f, 0.0f);
    glPushMatrix();
    glColor3ub(115, 80, 50);
    glTranslatef(start.pos[0], start.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-start.w, -start.h);
    glVertex2f(-start.w,  start.h);
    glVertex2f( start.w,  start.h);
    glVertex2f( start.w, -start.h);
    glEnd();
    glPopMatrix();

    Box start2 = Box(5.0f, 800.0f, 595.0f, 800.0f, 0.0f, 0.0f);
    glPushMatrix();
    glColor3ub(115, 80, 50);
    glTranslatef(start2.pos[0], start2.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-start2.w, -start2.h);
    glVertex2f(-start2.w,  start2.h);
    glVertex2f( start2.w,  start2.h);
    glVertex2f( start2.w, -start2.h);
    glEnd();
    glPopMatrix();

    Box start3 = Box(5.0f, 150.0f, 550.0f, 800.0f, 0.0f, 0.0f);
    glPushMatrix();
    glColor3ub(135, 145, 145);
    glTranslatef(start3.pos[0], start3.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-start3.w, -start3.h);
    glVertex2f(-start3.w,  start3.h);
    glVertex2f( start3.w,  start3.h);
    glVertex2f( start3.w, -start3.h);
    glEnd();
    glPopMatrix();

    Box out1 = Box(300.0f, 5.0f, 300.0f, 795.0f, 0.0f, 0.0f);
    glPushMatrix();
    glColor3ub(115, 80, 50);
    glTranslatef(out1.pos[0], out1.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-out1.w, -out1.h);
    glVertex2f(-out1.w,  out1.h);
    glVertex2f( out1.w,  out1.h);
    glVertex2f( out1.w, -out1.h);
    glEnd();
    glPopMatrix();

    Box out2 = Box(5.0f, 800.0f, 5.0f, 800.0f, 0.0f, 0.0f);
    glPushMatrix();
    glColor3ub(115, 80, 50);
    glTranslatef(out2.pos[0], out2.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-out2.w, -out2.h);
    glVertex2f(-out2.w,  out2.h);
    glVertex2f( out2.w,  out2.h);
    glVertex2f( out2.w, -out2.h);
    glEnd();
    glPopMatrix();

    Box bot1 = Box(75.0f, 50.0f, 75.0f, 0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glColor3ub(115, 80, 50);
    glTranslatef(bot1.pos[0], bot1.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-bot1.w, -bot1.h);
    glVertex2f(-bot1.w,  bot1.h);
    glVertex2f( bot1.w,  bot1.h);
    glVertex2f( bot1.w, -bot1.h);
    glEnd();
    glPopMatrix();

    Box bot2 = Box(75.0f, 50.0f, 475.0f, 0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glColor3ub(115, 80, 50);
    glTranslatef(bot2.pos[0], bot2.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-bot2.w, -bot2.h);
    glVertex2f(-bot2.w,  bot2.h);
    glVertex2f( bot2.w,  bot2.h);
    glVertex2f( bot2.w, -bot2.h);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt1[0], Gt1[1]);
    glVertex2f(Gt1[2], Gt1[3]);
    glVertex2f(Gt1[4], Gt1[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt2[0], Gt2[1]);
    glVertex2f(Gt2[2], Gt2[3]);
    glVertex2f(Gt2[4], Gt2[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt3[0], Gt3[1]);
    glVertex2f(Gt3[2], Gt3[3]);
    glVertex2f(Gt3[4], Gt3[5]);

    glEnd();

    glFlush();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt4[0], Gt4[1]);
    glVertex2f(Gt4[2], Gt4[3]);
    glVertex2f(Gt4[4], Gt4[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt5[0], Gt5[1]);
    glVertex2f(Gt5[2], Gt5[3]);
    glVertex2f(Gt5[4], Gt5[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt6[0], Gt6[1]);
    glVertex2f(Gt6[2], Gt6[3]);
    glVertex2f(Gt6[4], Gt6[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt7[0], Gt7[1]);
    glVertex2f(Gt7[2], Gt7[3]);
    glVertex2f(Gt7[4], Gt7[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt8[0], Gt8[1]);
    glVertex2f(Gt8[2], Gt8[3]);
    glVertex2f(Gt8[4], Gt8[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt9[0], Gt9[1]);
    glVertex2f(Gt9[2], Gt9[3]);
    glVertex2f(Gt9[4], Gt9[5]);

    glEnd();

        glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt10[0], Gt10[1]);
    glVertex2f(Gt10[2], Gt10[3]);
    glVertex2f(Gt10[4], Gt10[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt11[0], Gt11[1]);
    glVertex2f(Gt11[2], Gt11[3]);
    glVertex2f(Gt11[4], Gt11[5]);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(115, 80, 50);
    glVertex2f(Gt12[0], Gt12[1]);
    glVertex2f(Gt12[2], Gt12[3]);
    glVertex2f(Gt12[4], Gt12[5]);

    glEnd();

    int n = 15; 
    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    Circle circle1 = Circle(40.0f, 125, 650, 0.0f, 0.0f);
    //glVertex2f(x, y);
    glColor3ub(150, 150, 150);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
        circle1.x = circle1.r*cos(angle);
        circle1.y = circle1.r*sin(angle);
        glVertex2f(circle1.x+circle1.c[0],circle1.y + circle1.c[1]);
        angle += inc;
    }
    glEnd();

    Circle circle2 = Circle(25.0f, 225, 550, 0.0f, 0.0f);
    glColor3ub(150, 150, 150);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
        circle2.x = circle2.r*cos(angle);
        circle2.y = circle2.r*sin(angle);
        glVertex2f(circle2.x+circle2.c[0],circle2.y + circle2.c[1]);
        angle += inc;
    }
    glEnd();

    Circle circle3 = Circle(25.0f, 350, 575, 0.0f, 0.0f);
    glColor3ub(150, 150, 150);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
        circle3.x = circle3.r*cos(angle);
        circle3.y = circle3.r*sin(angle);
        glVertex2f(circle3.x+circle3.c[0],circle3.y + circle3.c[1]);
        angle += inc;
    }
    glEnd();

    Circle circle4 = Circle(25.0f, 300, 450, 0.0f, 0.0f);
    glColor3ub(150, 150, 150);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
        circle4.x = circle4.r*cos(angle);
        circle4.y = circle4.r*sin(angle);
        glVertex2f(circle4.x+circle4.c[0],circle4.y + circle4.c[1]);
        angle += inc;
    }
    glEnd();

    if (leftFlipper) {
        glColor3ub(150, 150, 150);
        glBegin(GL_LINES);
        glVertex2f(150.0f, 50.0f);
        glVertex2f(225.0f, 100.0f);
        glFlush();
        glEnd();
    }
    else {
        glColor3ub(150, 150, 150);
        glBegin(GL_LINES);
        glVertex2f(150.0f, 50.0f);
        glVertex2f(225.0f, 25.0f);
        glFlush();
        glEnd();
    }


    if (rightFlipper) {
        glColor3ub(150, 150, 150);
        glBegin(GL_LINES);
        glVertex2f(400.0f, 50.0f);
        glVertex2f(325.0f, 100.0f);
        glFlush();
        glEnd();
    }
    else {
        glColor3ub(150, 150, 150);
        glBegin(GL_LINES);
        glVertex2f(400.0f, 50.0f);
        glVertex2f(325.0f, 25.0f);
        glFlush();
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, g.texture);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 1.0f); glVertex2i(600, 0);
    glTexCoord2f( 0.0f, 0.0f); glVertex2i(600,  g.yres);
    glTexCoord2f( 1.0f, 0.0f); glVertex2i( g.xres,  g.yres);
    glTexCoord2f( 1.0f, 1.0f); glVertex2i( g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);


    }

    // Summon Ball	
    //if (summonball) {
    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef(ball.pos[0], ball.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-ball.w, -ball.h);
    glVertex2f(-ball.w,  ball.h);
    glVertex2f( ball.w,  ball.h);
    glVertex2f( ball.w, -ball.h);
    glEnd();
    glPopMatrix();

    Rect r[2];
    if (g.pause){
        r[0].bot = g.yres/1.5;
        r[0].left = g.xres/2;
        r[0].center = 0;
        ggprint8b(&r[6], 20, 0x00ff0000, "Pause Feature");
    }
    r[1].bot = g.yres-35;
    r[1].left = g.xres/2;
    r[1].center = -5;
    ggprint8b(&r[7], 20, 0x00ffff00, "Alex's Feature -  Shift + 2");

    renderStats(stats[1], g.xres, g.yres, points);
}






