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
};

class Image { 
    public: 
	int width, height; 
	unsigned char *data; 
	~Image();
	Image(const char *fname);
};

#endif

