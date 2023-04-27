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
/*Triangle t1;
Triangle t2;
Triangle t3;
Triangle t4;
Triangle t5;
Triangle flipper1;
Triangle flipper2;*/
class Image { 
    public: 
	int width, height; 
	unsigned char *data; 
	~Image();
	Image(const char *fname);
};

#endif

