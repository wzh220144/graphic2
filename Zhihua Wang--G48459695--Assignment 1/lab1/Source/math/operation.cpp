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