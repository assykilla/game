// Xander Reyes

#ifndef _XANDER_H_
#define _XANDER_H_
class Triangle 
{
    public:
        float vertex1[2],vertex2[2],vertex3[2];
	float angle;
        Triangle();
        Triangle(float vertx1, float vertx2, float vertx3,
                float verty1, float verty2, float verty3);
	unsigned char color[3];
        void set_color(unsigned char col[3]);
};

extern Triangle t1;
extern Triangle t2;
extern Triangle t3;
extern Triangle t4;
extern Triangle t5;
extern Triangle t6;
extern Triangle t7;
extern Triangle t8;
extern Triangle t9;
extern Triangle t10;
extern Triangle flipper1;
extern Triangle flipper2;
class Image { 
    public: 
	int width, height; 
	unsigned char *data; 
	~Image();
	Image(const char *fname);
};

#endif

