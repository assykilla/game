// Issa Samara


#ifndef _ISAMARA_H_
#define _ISAMARA_H_

class prompt {
    public:
        float w, h;
        float pos[2];
        unsigned char color[3];
        void set_color (unsigned char col[3]);
        prompt();
        prompt(float,float,float,float);
};


extern void draw_box(float,float,float,float,int,int,unsigned char);
extern void draw_texture(GLuint,int,int);
extern void create_text(Rect,char*,float,float);
extern void render_menu(Rect,int,int,int);
extern void render_map_select(int, int);
extern void render_title(Rect t,int,int);
extern unsigned int select_option(int,int,int,int);
extern unsigned int select_map(int,int,int,int);
extern void draw_button(int,int);
extern void push_button(float*,int,int);
extern void check_score(int,int,int*,int*);
extern bool reset_death(int*,int*,bool);
extern void read_leaderboard(string,string*,string*);
extern void print_leaderboard_boxes(int,int);
extern void print_leaderboard(string*,string*,int,int);
extern int update_leaderboard(string*,string*,int,int,int);
extern void save_leaderboard(string,string*,string*);

#endif
