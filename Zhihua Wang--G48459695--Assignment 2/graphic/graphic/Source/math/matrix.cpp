#include "head.h"
//================================
// matrix
//================================

Matrix::Matrix() {
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			if(i==j)
				mat[i+j*4]=1.0;
			else
				mat[i+j*4]=0.0;
		}
	}
}

Matrix::~Matrix() {
}

void Matrix::init() {
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			if(i==j)
				mat[i+j*4]=1.0;
			else
				mat[i+j*4]=0.0;
		}
	}
}

//multiply two Matrixes
void Matrix::MultiMatrix(Matrix a) {
	float temp[16];
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			temp[4*i+j]=0.0;
			for(int k=0; k<4; k++) {
				temp[i*4+j]+=mat[4*k+j]*a.mat[4*i+k];
			}
		}
	}
	for(int i=0; i<16; i++) {
		mat[i]=temp[i];
	}
}

void Matrix::antiMultiMatrix(Matrix a) {
	float temp[16];
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			temp[4*i+j]=0.0;
			for(int k=0; k<4; k++) {
				temp[i*4+j]+=a.mat[4*k+j]*mat[4*i+k];
			}
		}
	}
	for(int i=0; i<16; i++) {
		mat[i]=temp[i];
	}
}

//set the value of a Matrix
void Matrix::SetMatrix(Matrix a) {
	for(int i=0; i<16; i++)
		mat[i]=a.mat[i];
}

//add two matrixes
void Matrix::AddMatrix(Matrix a) {
	for(int i=0; i<16; i++)
		mat[i]+=a.mat[i];
}