//#include "stdafx.h"
#include  "Head/headcpp.h"
#include "Head/parameter/parameter.h"
#pragma comment(lib,"glew32d.lib")
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")

//================================
// routine for recovering modelview matrix for evey model
//================================
void Recover_Matr(int id) {
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf(MultiMatrix(globalCamera.cm_mat, g_model[id].md_mat));
}

void Recover_Global() {
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf(globalCamera.cm_mat.mat);
}

//================================
//collect memory
//================================

void close() {

}

//================================
// init
//================================
void init( void ) {

	//Initialize IL
	ilInit();
	//Initialize ILU
	iluInit();
	//Initialize ILUT with opengl support
	ilutRenderer(ILUT_OPENGL);

	ilEnable(IL_FILE_OVERWRITE);

	glClearColor (0.0, 0.0, 0.0, 1.0);

	init_buffer();
	
	//init projection and camera
	g_mdCtrl=0;
	g_model = new Model[MODEL_NUM];
	g_camera = new Camera[MODEL_NUM];
	g_Texture = new Texture[MODEL_NUM];
	globalProjection =new Matrix;
	globalProjection->init();
	globalCamera.setLookAt(globalVector[0], globalVector[1], globalVector[2], -1);

	// load model coordinate
	for(int i=1; i<MODEL_NUM;i++) {
		g_model[i].LoadModel( g_mdPath[i], i);
		g_Texture[i].LoadFile(g_texPath[i]);
		switch(i) {
		case 0:
			g_model[i].Scale(vec3(1.0, 1.0, 1.0));
			break;
		case 1:
			/*g_Texture[i].doubleX();
			g_Texture[i].doubleY();
			g_Texture[i].doubleX();
			g_Texture[i].doubleY();
			g_Texture[i].doubleX();
			g_Texture[i].doubleY();
			g_Texture[i].doubleX();
			g_Texture[i].doubleY();*/
			g_Texture[i].MipMap();
			/*g_Texture[i].StoreImage("0.jpg", g_Texture[i].mipmap[0], g_Texture[i].texil[0], g_Texture[i].texil[0]);
			g_Texture[i].StoreImage("1.jpg", g_Texture[i].mipmap[1], g_Texture[i].texil[1], g_Texture[i].texil[1]);
			g_Texture[i].StoreImage("2.jpg", g_Texture[i].mipmap[2], g_Texture[i].texil[2], g_Texture[i].texil[2]);
			g_Texture[i].StoreImage("3.jpg", g_Texture[i].mipmap[3], g_Texture[i].texil[3], g_Texture[i].texil[3]);
			g_Texture[i].StoreImage("4.jpg", g_Texture[i].mipmap[4], g_Texture[i].texil[4], g_Texture[i].texil[4]);
			g_Texture[i].StoreImage("5.jpg", g_Texture[i].mipmap[5], g_Texture[i].texil[5], g_Texture[i].texil[5]);
			g_Texture[i].StoreImage("6.jpg", g_Texture[i].mipmap[6], g_Texture[i].texil[6], g_Texture[i].texil[6]);
			g_Texture[i].StoreImage("7.jpg", g_Texture[i].mipmap[7], g_Texture[i].texil[7], g_Texture[i].texil[7]);
			g_Texture[i].StoreImage("8.jpg", g_Texture[i].mipmap[8], g_Texture[i].texil[8], g_Texture[i].texil[8]);
			g_Texture[i].StoreImage("9.jpg", g_Texture[i].mipmap[9], g_Texture[i].texil[9], g_Texture[i].texil[9]);*/
			g_model[i].Scale(vec3(1.0, 1.0, 1.0));
			g_model[i].Translate(vec3(0.1, 0.3, 0.0));
			break;
		case 2:
			g_model[i].Scale(vec3(1.0, 1.0, 1.0));
			g_model[i].Translate(vec3(3.0, 0.0, 0.0));
			break;
		case 3:
			g_model[i].Scale(vec3(5.0, 5.0, 5.0));
		default:
			break;
		}
	}
	
}


//================================
// update
//================================
void update( void ) {
	// do something before rendering...
}

//================================
// render
//================================
void render() {
	// clear color and depth buffer
	clean_buffer();
	// draw model
	Matrix temp;
	for(int i=1; i<MODEL_NUM; i++) {		//draw which item
		//multiply camera, projection and model transformation together
	/*	temp1=MultiMatrix(globalCamera.cm_mat, g_model[i].md_mat);
		for(int j=0; j<16; j++)
			temp.mat[j]=temp1[j];
		temp1=MultiMatrix(*globalProjection, temp);
		for(int j=0; j<16; j++)
			temp.mat[j]=temp1[j];*/
		//only apply model transformation, no projection and viewing transformation, 
		//so no need to calculate the inverse of the matrix which is the multiply projection and camera together
		g_model[i].ComputeVerts(&g_model[i].md_mat);
		g_model[i].ComputeNormal();
		g_model[i].BackFaceRemove(globalCamera.position);		//back face remove
		//g_model[i].DrawEdges(&g_model[i].md_mat);
		g_model[i].PhongShading();		//pong shading
		//g_model[i].GouraudShading();		//gouraud shading
		//g_model[i].ConstantShading();		//constant shading
		g_model[i].ScanConvertion(globalCamera.position, &globalLight[i], &globalLightPosition[i], 1, &g_Texture[i]);		//scan convertion
	}
	DrawImage();		//draw image

	//	 swap back and front buffers
	glutSwapBuffers();
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {

	// screen size
	winWidth  = w;
	winHeight = h;	
	glViewport( 0, 0, (GLsizei)winWidth, (GLsizei)winHeight );
	//perspective(Theta, (GLfloat)winWidth/(GLfloat)winHeight, Dnear, Dfar, globalProjection);
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {
	// increase frame index
	g_frameIndex++;

	update();
	
	// render
	glutPostRedisplay();

	// reset timer
	glutTimerFunc( 16, timer, 0 );
}

//================================
// main
//================================
int main( int argc, char** argv ) {
	
	//aa=fopen("normal.txt", "w");

	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( winWidth, winHeight ); 
	glutInitWindowPosition( winXaxis, winYaxis);
	glutCreateWindow( argv[0] );

	// init
	init();
	
	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	glutTimerFunc( 16, timer, 0 );

	// main loop
	glutMainLoop();

	close();

	return 0;
}