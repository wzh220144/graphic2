#ifndef __GWU_TEXTURE__
#define __GWU_TEXTURE__

class Texture {

public:
	unsigned char *data;
	unsigned char **mipmap;
	GLint *texil;
	GLint count;
	GLint sizeX, sizeY;
	ILuint name;
	float curMaxX, curMinX, curMaxY, curMinY;

public:
	GLvoid LoadFile(char *path);
	GLvoid doubleX();
	GLvoid doubleY();
	GLvoid StoreImage(char *path, unsigned char *temp, int w, int h);
	GLvoid MipMap();

};

#endif