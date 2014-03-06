#ifndef __GWU_MATRIX__
#define __GWU_MATRIX__

//================================
// matrix
//================================

class Matrix {

public:
	float mat[16];

public:
	Matrix();
	~Matrix();
	void init();
	void MultiMatrix(Matrix a);
	void antiMultiMatrix(Matrix a);
	void SetMatrix(Matrix a);
	void AddMatrix(Matrix a);
};

#endif //__GWU_MATRIX__