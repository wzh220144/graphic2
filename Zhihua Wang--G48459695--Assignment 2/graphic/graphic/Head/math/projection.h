#ifndef __GWU_PROJECTION__
#define __GWU_PROJECTION__


void perspective (float theta, float aspect, float dnear, float dfar, Matrix *a);
void frustum(float xwmin, float xwmax, float ywmin, float ywmax, float dnear, float dfar, Matrix *a);


#endif //__GWU_PROJECTION__