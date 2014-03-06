#ifndef __GWU_PARAMETER__
#define __GWU_PARAMETER__

//================================
// global variables
//================================

#define MODEL_NUM 1	//number of models
#define LIGHT_NUM 10	//number of light source


const float PI=cos(-1.0);	//set PI
const float shitf=0.01;	//the shift of every movement of item and camera 

bool curMouseFlag=false;	//sign that if the left mouse has been clicked
int g_frameIndex;	//frame index
int g_mdCtrl = 0;	// the index of the current model controled
int winXaxis=100, winYaxis=100;	//window position
float temp[16];	//temp for saving modelview matrix 
float winWidth=600.0, winHeight=600.0;	//window size
float Theta=20.0f, Dnear=0.01f, Dfar=100.0f;	//perspective parameter theta, aspect, dnear, dfar

vec3 camera_pos=vec3(0,0,0);

Model *g_model;	// model
Camera *g_camera;	//camera of every model
Camera globalCamera;	//global camera
Matrix *globalProjection;	//global projection matrix


//path of model
char g_mdPath[MODEL_NUM][90]={"Data/cow.d"};

//eye, center and up vector of global camera
vec3 globalVector[3]={vec3(0.0, 0.0, 5.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0)};

#endif