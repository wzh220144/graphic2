#include "head.h"

GLvoid Texture::LoadFile(char *path) {

    //does not define what to do when cannot read file
	ilGenImages(1, &name);
	ilBindImage(name);
	ilLoadImage(path);

	sizeX = ilGetInteger(IL_IMAGE_WIDTH);
	sizeY = ilGetInteger(IL_IMAGE_HEIGHT);
	int format = ilGetInteger(IL_IMAGE_FORMAT);
	if(format!=IL_RGB)
		ilConvertImage(IL_RGB, ilGetInteger(IL_IMAGE_TYPE));
	data = ilGetData();
	unsigned char *temp=(unsigned char *)malloc(sizeX*sizeY*3);
	for(int i=0; i<sizeX; i++) {
		for(int j=0; j<sizeX; j++) {
			temp[j*sizeX*3+i*3] = data[j*sizeX*3+i*3];
			temp[j*sizeX*3+i*3+1] = data[j*sizeX*3+i*3+1];
			temp[j*sizeX*3+i*3+2] = data[j*sizeX*3+i*3+2];
		}
	}
	data=temp;
    return ;
}

GLvoid Texture::MipMap() {		//assert sizeX==sizeY
	count=0;
	int temp=sizeX;
	while(temp) {
		count++;
		temp/=2;
	}
	texil=(int *)malloc(sizeof(int)*count);
	mipmap = (unsigned char **)malloc(sizeof(unsigned char *)*count);
	texil[0]=1;
	for(int i=1; i<count; i++) {
		texil[i]=texil[i-1]*2;
	}
	mipmap[count-1]=(unsigned char *)malloc(sizeof(unsigned char)*texil[count-1]*texil[count-1]*3);
	int l=texil[count-1];
	for(int i=0; i<l; i++) {
		for(int j=0; j<l; j++) {
			mipmap[count-1][l*3*j+i*3]=data[sizeX*3*j+i*3];
			mipmap[count-1][l*3*j+i*3+1]=data[sizeX*3*j+i*3+1];
			mipmap[count-1][l*3*j+i*3+2]=data[sizeX*3*j+i*3+2];
		}
	}
	for(int i=count-2; i>=0; i--) {
		mipmap[i]=(unsigned char *)malloc(sizeof(unsigned char)*texil[i]*texil[i]*3);
		int l1=texil[i+1], l2=texil[i];
		for(int j=0; j<l2; j++) {
			for(int k=0; k<l2; k++) {
				int k1=k*2, k2=k*2+1;
				int j1=j*2, j2=j*2+1;
				mipmap[i][l2*3*k+j*3]=(mipmap[i+1][l1*3*k1+j1*3]+mipmap[i+1][l1*3*k1+j2*3]+mipmap[i+1][l1*3*k2+j1*3]+mipmap[i+1][l1*3*k2+j2*3])/4;
				mipmap[i][l2*3*k+j*3+1]=(mipmap[i+1][l1*3*k1+j1*3+1]+mipmap[i+1][l1*3*k1+j2*3+1]+mipmap[i+1][l1*3*k2+j1*3+1]+mipmap[i+1][l1*3*k2+j2*3+1])/4;
				mipmap[i][l2*3*k+j*3+2]=(mipmap[i+1][l1*3*k1+j1*3+2]+mipmap[i+1][l1*3*k1+j2*3+2]+mipmap[i+1][l1*3*k2+j1*3+2]+mipmap[i+1][l1*3*k2+j2*3+2])/4;
			}
		}
	}

}

GLvoid Texture::doubleX() {
	unsigned char *temp=(unsigned char *)malloc(sizeX*sizeY*2*3);
	for(int i=0; i<sizeX; i++) {
		for(int j=0; j<sizeY; j++) {
			temp[j*sizeX*2*3+sizeX*3+i*3] = data[j*sizeX*3+i*3];
			temp[j*sizeX*2*3+i*3] = data[j*sizeX*3+i*3];
			temp[j*sizeX*2*3+sizeX*3+i*3+1] = data[j*sizeX*3+i*3+1];
			temp[j*sizeX*2*3+i*3+1] = data[j*sizeX*3+i*3+1];
			temp[j*sizeX*2*3+sizeX*3+i*3+2] = data[j*sizeX*3+i*3+2];
			temp[j*sizeX*2*3+i*3+2] = data[j*sizeX*3+i*3+2];
		}
	}
	free(data);
	data=temp;
	sizeX=sizeX*2;
}

GLvoid Texture::doubleY() {
	unsigned char *temp=(unsigned char *)malloc(sizeX*sizeY*2*3);
	for(int i=0; i<sizeX; i++) {
		for(int j=0; j<sizeY; j++) {
			temp[sizeX*sizeY*3+j*sizeX*3+i*3] = data[j*sizeX*3+i*3];
			temp[j*sizeX*3+i*3] = data[j*sizeX*3+i*3];
			temp[sizeX*sizeY*3+j*sizeX*3+i*3+1] = data[j*sizeX*3+i*3+1];
			temp[j*sizeX*3+i*3+1] = data[j*sizeX*3+i*3+1];
			temp[sizeX*sizeY*3+j*sizeX*3+i*3+2] = data[j*sizeX*3+i*3+2];
			temp[j*sizeX*3+i*3+2] = data[j*sizeX*3+i*3+2];
		}
	}
	free(data);
	data=temp;
	sizeY=sizeY*2;
}

GLvoid Texture::StoreImage(char *path, unsigned char *temp, int w, int h) {
	ilTexImage(w, h, 0, 3, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), temp);
	ilSaveImage(path);
}