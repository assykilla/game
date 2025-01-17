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
#include <fstream>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "isamara.h"
#include "xreyes.h"

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

void draw_texture(GLuint image, int x, int y)
{
		glBindTexture(GL_TEXTURE_2D, image);
		int w = x;
		int h = y;
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0,  h);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( w,  h);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( w, 0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

}

void create_text(Rect t, string text, char* temp, float xcoord, float ycoord)
{
		t.bot = ycoord;
		t.left = xcoord;
		t.center = -5;
		strcpy(temp, text.c_str());
		ggprint8b(&t, 16, 0x00fffff0, temp);
}
void render_menu(Rect t, int i, int x, int y)
{
		prompt p(x/10, y/40, x/8, 3*(y/4));
		string text[4] = {"Start", "Controls", "Leaderboard", "Exit"};
		char temp[24];
		float xcoord = p.pos[0];
		float ycoord = p.pos[1] - (i*100);
		//Draw boxes.
		unsigned char c[3] = {200,200,200};
		p.set_color(c);
		draw_box(xcoord, ycoord, p.w, p.h, x, y, p.color);
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

unsigned int select_option(int x, int y, int xbutton, int ybutton)
{
		prompt size(x/10, y/40, x/8, 3*(y/4));
		if (ybutton <= size.pos[1]+size.h &&
						xbutton >= size.pos[0]-size.w &&
						xbutton <= size.pos[0]+size.w &&
						ybutton >= size.pos[1]-size.h) {
				return 1;
		} else if (ybutton <= size.pos[1]+size.h - 100 &&
						xbutton >= size.pos[0]-size.w &&
						xbutton <= size.pos[0]+size.w &&
						ybutton >= size.pos[1]-size.h - 100) {
				return 2;
		} else if (ybutton <= size.pos[1]+size.h - 200 &&
						xbutton >= size.pos[0]-size.w &&
						xbutton <= size.pos[0]+size.w &&
						ybutton >= size.pos[1]-size.h - 200) {
				return 3;
		} else if (ybutton <= size.pos[1]+size.h - 300 &&
						xbutton >= size.pos[0]-size.w &&
						xbutton <= size.pos[0]+size.w &&
						ybutton >= size.pos[1]-size.h - 300) {
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

void read_leaderboard(string file, string* name, string* score)
{
		string line;
		int counter = 0;
		ifstream fin(file);
		if (fin.fail()) {
				cout << "Bad file input, please make sure the file path is correct"
						<< endl;
		}
		fin >> line;
		while (!fin.eof()) {
				usleep(1000);
				char placeholder[10];
				char scoreholder[100];
				strncpy(placeholder, line.c_str(), 3);
				name[counter] = placeholder;
				line.erase(0,3);
				strcpy(scoreholder, line.c_str());
				score[counter] = scoreholder;
				fin >> line;
				counter++;
		}

		fin.close();
}

void print_leaderboard_boxes(int x, int y)
{
		prompt n(x/30, y/4, x/8, (y/2));
		prompt s(x/10, y/4, x/2, (y/2));
		prompt l(x/5, y/20, x/2, y-30);
		Rect t;
		string name = "name";
		string score = "score";
		string leaderboard = "leaderboard";
		char temp[24];
		float xcoord1 = n.pos[0];
		float xcoord2 = s.pos[0];
		float xcoord3 = l.pos[0];
		float ycoord = 7*(y/10);
		float ycoord2 = l.pos[1]-10;
		unsigned char c[3] = {200,200,200};
		n.set_color(c);
		s.set_color(c);
		l.set_color(c);
		//draw_box(xcoord, ycoord, p.w, p.h, x, y, p.color);
		draw_box(xcoord1, n.pos[1], n.w, n.h, x, y, n.color);
		draw_box(xcoord2, s.pos[1], s.w, s.h, x, y, s.color);
		draw_box(xcoord3, l.pos[1], l.w, l.h, x, y, l.color);
		create_text(t, name, temp, xcoord1, ycoord);
		create_text(t, score, temp, xcoord2, ycoord);
		create_text(t, leaderboard, temp, xcoord3, ycoord2);
}

void print_end_screen(int x, int y, int print)
{
		prompt l(x/5, y/30, x/2, y-75);
		Rect t;
		string msg1 = "Enter 3 characters to name new score";
		string msg2 = "Then enter e to return to main menu";
		string msg3 = "Enter e to return to mainmenu";
		string msg4 = "New High Score!";
		string msg5 = "You have failed";
		char temp[50];
		float xcoord = l.pos[0];
		float ycoord = l.pos[1];
		unsigned char c[3] = {200,200,200};
		l.set_color(c);
		draw_box(xcoord, l.pos[1], l.w, l.h, x, y, l.color);
		if (print == -1) {
				create_text(t, msg5, temp, xcoord, ycoord+15);
				create_text(t, msg3, temp, xcoord, ycoord);
		} else {
				create_text(t, msg4, temp, xcoord, ycoord+15);
				create_text(t, msg1, temp, xcoord, ycoord);
				create_text(t, msg2, temp, xcoord, ycoord-15);
		}

}

void print_leaderboard(string* name, string* score, int x, int y)
{
		Rect t;
		prompt n(x/30, y/4, x/8, 2*(y/3));
		prompt s(x/10, y/4, x/2, 2*(y/3));
		char temp[100];
		float xcoord1 = n.pos[0];
		float xcoord2 = s.pos[0];
		//create_text(t, text[i], temp, xcoord, ycoord);
		for (int i=0; i<10; i++) {
				string leader = name[i];
				string highscore = score[i];
				float ycoord = n.pos[1] - (i*30);
				create_text(t, leader, temp, xcoord1, ycoord);
				create_text(t, highscore, temp, xcoord2, ycoord);
		}
}

int update_leaderboard(string* name, string* score, int x, int y, int final_score)
{
		int position = -1;
		string temp_score2;
		string temp_name;
		for (int i=0;i<9;i++) {
				int temp_score = atoi(score[i].c_str());
				if (final_score > temp_score && position == -1) {
						position = i;
						temp_score2 = score[i];
						temp_name = name[i];
						score[position] = to_string(final_score);
						name[position] = "";
				} 
				if (position != -1) {
						string temp_score3 = score[i+1];
						string temp_name2 = name[i+1];
						score[i+1] = temp_score2;
						name[i+1] = temp_name;
						temp_score2 = temp_score3;
						temp_name = temp_name2; 
				}
		}
		return position;
}






void save_leaderboard(string file, string* name, string* score)
{
		ofstream fin(file);
		if (fin.fail()) {
				cout << "Bad file input, please make sure the file path is correct"
						<< endl;
		}
		for (int i=0; i<10; i++) {
				string line = name[i] + score[i] + "\n";
				fin << line;
				usleep(1000);
		}
		fin.close();
}

