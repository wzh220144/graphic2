#include "head.h"

//calculate the product of a and b
vec3  Xproduct(vec3 a, vec3 b) {
	return vec3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
}

//calculate the distance between vertex a and vertex b
float Distance(vec3 a, vec3 b) {
	return sqrt((a.x -b.x )*(a.x -b.x )+(a.y -b.y )*(a.y -b.y )+(a.z-b.z)*(a.z-b.z));
}

float *MultiMatrix(Matrix a, Matrix b) {
		float *temp;
		temp=new float[16];
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				temp[4*i+j]=0.0;
				for(int k=0; k<4; k++) {
					temp[i*4+j]+=a.mat[4*k+j]*b.mat[4*i+k];
				}
			}
		}
		return temp;
}

float Random() {		//return a float between [0, 1]
	float a, b;
		a=(float)abs(rand()), b=(float)abs(rand());
		if(a>b)
			return b/a;
		else return a/b;
}

void init_buffer() {
	xIncreasement=2.0/((float)PIXELX);
	yIncreasement=2.0/((float)PIXELY);
	zIncreasement=2.0/((float)PIXELZ);

	xValue[0]=-1.0;
	yValue[0]=-1.0;
	zValue[0]=-1.0;
	for(int i=1; i<PIXELX; i++) {
		xValue[i]=xValue[i-1]+xIncreasement;
		yValue[i]=yValue[i-1]+yIncreasement;
		zValue[i]=zValue[i-1]+zIncreasement;
	}
	xValue[PIXELX-1]=1.0;
	yValue[PIXELY-1]=1.0;
	zValue[PIXELZ-1]=1.0;
}

void clean_buffer() {
	for(int i=0; i<PIXELX; i++) {
		for(int j=0; j<PIXELY; j++) {
			Zbuffer[i][j]=-3.402823e38;
			Ibuffer[i][j]=vec3(0.0, 0.0, 0.0);
		}
	}
}

int xfind(float x) {
	int mid, left=0, right=PIXELX-1, res=0;
	while(left<=right) {
		mid=((left+right)>>1);
		if(x<xValue[mid]) {
			right=mid-1;
		}
		else {
			res=right;
			left=mid+1;
		}
	}
	float ts=fabs(xValue[right]-x), temp;
	int t;
	res=right;
	for(int i=0; i<10; i++) {
		t=right-i;
		if(t<0)
			break;
		temp=fabs(xValue[t]-x);
		if(temp<ts) {
			ts=temp;
			res=t;
		}
	}
	for(int i=0; i<10; i++) {
		t=right+i;
		if(t>=PIXELX)
			break;
		temp=fabs(xValue[t]-x);
		if(temp<ts) {
			ts=temp;
			res=t;
		}
	}
	return res;
}

int yfind(float y) {
	int mid, left=0, right=PIXELY-1, res=0;
	while(left<=right) {
		mid=((left+right)>>1);
		if(y<yValue[mid]) {
			right=mid-1;
		}
		else {
			res=mid;
			left=mid+1;
		}
	}
	float ts=fabs(yValue[right]-y), temp;
	int t;
	res=right;
	for(int i=0; i<10; i++) {
		t=right-i;
		if(t<0)
			break;
		temp=fabs(yValue[t]-y);
		if(temp<ts) {
			ts=temp;
			res=t;
		}
	}
	for(int i=0; i<10; i++) {
		t=right+i;
		if(t>=PIXELY)
			break;
		temp=fabs(yValue[t]-y);
		if(temp<ts) {
			ts=temp;
			res=t;
		}
	}
	return res;
}

int zfind(float z) {
	int mid, left=0, right=PIXELZ-1, res=0;
	while(left<=right) {
		mid=((left+right)>>1);
		if(z<=zValue[mid]) {
			right=mid-1;
		}
		else {
			res=mid;
			left=mid+1;
		}
	}

	return res;
}

void DrawImage() {
	glBegin(GL_POINTS);
	for(int i=0; i<PIXELX; i++) {
		for(int j=0; j<PIXELY; j++) {
			glColor3fv(Ibuffer[i][j].ptr());
			glVertex2f(xValue[i], yValue[j]);
		}
	}
	glEnd();
}

bool cmp1(const Edge &a, const Edge &b) {
	return a.yminID<b.yminID;
}

bool cmp2(const Edge &a, const Edge &b) {
	return a.x<b.x;
}

int Max(int a, int b) {
	return a>b?a:b;
}

int Min(int a, int b) {
	return a<b?a:b;
}