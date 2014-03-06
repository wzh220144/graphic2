#ifndef __GWU_OPERATION__
#define __GWU_OPERATION__

#include "head.h"

vec3  Xproduct(vec3 a, vec3 b);
float Distance(vec3 a, vec3 b);
float *MultiMatrix(Matrix a, Matrix b);
float Random();
int xfind(float x);
int yfind(float y);
int zfind(float z);
void init_buffer();
void clean_buffer();
bool cmp1(const Edge &a, const Edge &b);
bool cmp2(const Edge &a, const Edge &b);
int Max(int a, int b);
int Min(int a, int b);
void DrawImage();

#endif  //__GWU_OPERATION_