// Issa Samara


#ifndef _ISAMARA_H_
#define _ISAMARA_H_

class prompt {
    public:
        float w, h;
        float pos[2];
        unsigned char color[3];
        void set_color ( unsigned char col[3]);
        prompt();
        prompt(float, float, float, float);
};


extern void draw_box(float, float, float, float, int, int, unsigned char);
extern void create_text(Rect, char*, float, float);
extern void render_menu(prompt,Rect,int,int,int);
extern void render_map_select(int, int);
extern void render_title(Rect t,int,int);
extern unsigned int select_option(int,int);
extern unsigned int select_map(int, int, int, int);
extern void draw_button(int, int);
extern void push_button(float*, int, int);

#endif
