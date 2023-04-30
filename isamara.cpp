// Issa Samara
//
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
#include "isamara.h"

//void draw_box(float, float, float, float, int, int, unsigned char);
void prompt::set_color ( unsigned char col[3])
{
    memcpy(color, col, sizeof(unsigned char) *3);
}


prompt::prompt()
{
    w = 80.0f;
    h = 15.0f;
    pos[0] = 90;
    pos[1] = 300;
    color[0] = 100;
    color[1] = 100;
    color[2] = 100;
    }

prompt::prompt(float width, float height, float xpos, float ypos)
{
    w = width;
    h = height;
    pos[0] = xpos;
    pos[1] = ypos;
}

void draw_box(float xcoord, float ycoord, float width, float height, int xres, int yres, unsigned char col[3])
{
    glPushMatrix();
    glColor3ubv(col);
    glTranslatef(xcoord, ycoord, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-width, -height);
    glVertex2f(-width,  height);
    glVertex2f( width,  height);
    glVertex2f( width, -height);
    glEnd();
    glPopMatrix();
}

void create_text(Rect t, string text, char* temp, float xcoord, float ycoord)
{
    t.bot = ycoord;
    t.left = xcoord;
    t.center = -5;
    strcpy(temp, text.c_str());
    ggprint8b(&t, 16, 0x00fffff0, temp);
}
void render_menu(prompt p, Rect t, int i, int x, int y)
{
    string text[4] = {"Start", "Controls", "Leaderboard", "Exit"};
    char temp[24];
    float xcoord = p.pos[0];
    float ycoord = p.pos[1] - 60*i;
    //Draw boxes.
    draw_box(p.pos[0], ycoord, p.w, p.h, x, y, p.color);
    create_text(t, text[i], temp, xcoord, ycoord);
}

void render_map_select(int x, int y)
{
    string text[2] = {"Table 1", "Table 2"};
    Rect map[2];
    prompt first_map(x/8, y/8, x/2 - x/6, y/2);
    prompt second_map(x/8, y/8, x/2 + x/6, y/2);
    unsigned char c[3] = {200,200,200};
    first_map.set_color(c);
    second_map.set_color(c);
    char temp[24];

    draw_box(first_map.pos[0], first_map.pos[1], first_map.w, first_map.h, x, y, first_map.color);
    draw_box(second_map.pos[0], second_map.pos[1], second_map.w, second_map.h, x, y, second_map.color);
    create_text(map[0], text[0], temp, first_map.pos[0], first_map.pos[1]);
    create_text(map[1], text[1], temp, second_map.pos[0], second_map.pos[1]);
}


void render_title(Rect t, int x, int y)
{
    char text[17] = "Triassic Pinball";
    t.bot = y/1.1;
    t.left = x/2;
    t.center = -5;
    ggprint8b(&t, 16, 0x00fffff0, text);
}

unsigned int select_option(int x, int y)
{
    prompt size;
    if (y <= size.pos[1]+size.h &&
            x >= size.pos[0]-size.w &&
            x <= size.pos[0]+size.w &&
            y >= size.pos[1]-size.h) {
        return 1;
    } else if (y <= size.pos[1]+size.h - 60 &&
            x >= size.pos[0]-size.w &&
            x <= size.pos[0]+size.w &&
            y >= size.pos[1]-size.h - 60) {
        return 2;
    } else if (y <= size.pos[1]+size.h - 120 &&
            x >= size.pos[0]-size.w &&
            x <= size.pos[0]+size.w &&
            y >= size.pos[1]-size.h - 120) {
        return 3;
    } else if (y <= size.pos[1]+size.h - 180 &&
            x >= size.pos[0]-size.w &&
            x <= size.pos[0]+size.w &&
            y >= size.pos[1]-size.h - 180) {
        return 4;
    } else {return 0;}
}

unsigned int select_map(int x, int y, int xbutton, int ybutton)
{
    prompt first_map(x/8, y/8, x/2 - x/6, y/2);
    prompt second_map(x/8, y/8, x/2 + x/6, y/2);
    if (ybutton <= first_map.pos[1]+first_map.h &&
            xbutton >= first_map.pos[0]-first_map.w &&
            xbutton <= first_map.pos[0]+first_map.w &&
            ybutton >= first_map.pos[1]-first_map.h) {
        return 1;
    } else if (ybutton <= second_map.pos[1]+second_map.h &&
            xbutton >= second_map.pos[0]-second_map.w &&
            xbutton <= second_map.pos[0]+second_map.w &&
            ybutton >= second_map.pos[1]-second_map.h) {
        return 2;
    } else {return 0;}
}

void draw_button(int x, int y)
{
    int n = 20;
    //circle.h will be used for the width
    //drawing button to be pushed
    prompt circle;
    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    //glVertex2f(x, y);
    glColor3ub(100,200, 255);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
        circle.pos[0] = circle.h*cos(angle);
        circle.pos[1] = circle.h*sin(angle);
        glVertex2f(x/1.5+circle.pos[0],y/3 + circle.pos[1]);
        angle += inc;
    }
    glEnd();
}

void push_button(float *velocity, int x, int y)
{
    // tries to launch ball similar to how pinball machines work
    prompt circle;
    if (y <= y/3 &&
            x >= -1*circle.h-x/1.5 &&
            x <= circle.h+x/1.5 &&
            y >= -y/3) {
        *velocity = 20.0f;
    }
    
}

void update_lives(unsigned int lives) 
{

}
