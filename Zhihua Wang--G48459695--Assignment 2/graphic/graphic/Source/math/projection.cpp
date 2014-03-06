#include "head.h"

void perspective (float theta, float aspect, float dnear, float dfar, Matrix *a) {

	Matrix temp;
	temp.init();
	temp.mat[0]=2.0/tan(theta/2.0)/aspect;
	temp.mat[5]=2.0/tan(theta/2.0);
	temp.mat[10]=(dnear+dfar)/(dnear-dfar);
	temp.mat[11]=-1.0;
	temp.mat[14]=2.0*dnear*dfar/(dfar-dnear);
	a->MultiMatrix(temp);
}



void frustum(float xwmin, float xwmax, float ywmin, float ywmax, float dnear, float dfar, Matrix *a) {

	//dnear=-dnear;
	//dfar=-dfar;
	Matrix temp;
	temp.init();
	temp.mat[0]=2.0*dnear/(xwmax-xwmin);
	temp.mat[5]=-2.0*dnear/(ywmax-ywmin);
	temp.mat[8]=(xwmax+xwmin)/(xwmax-xwmin);
	temp.mat[9]=(ywmax+ywmin)/(ywmax-ywmin);
	temp.mat[10]=(dnear+dfar)/(dnear-dfar);
	temp.mat[11]=-1.0;
	temp.mat[14]=2.0*dnear*dfar/(dfar-dnear);
	a->MultiMatrix(temp);

}
