//#include "stdafx.h"
#include "headcpp.h"
#include "parameter/parameter.h"
#pragma comment(lib,"glew32d.lib")

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

	//collect memory
	delete[] g_model;
	g_model=NULL;
	delete[] g_camera;
	g_camera = NULL;
	delete globalProjection;
	globalProjection = NULL;

}

//================================
// init
//================================
void init( void ) {

	//init projection and camera
	g_mdCtrl=0;
	g_model = new Model[MODEL_NUM];
	g_camera = new Camera[MODEL_NUM];
	globalProjection =new Matrix;
	globalProjection->init();
	globalCamera.setLookAt(globalVector[0], globalVector[1], globalVector[2], -1);

	// load model coordinate
	for(int i=0; i<MODEL_NUM;i++) {
		g_model[i].LoadModel( g_mdPath[i], i);
		g_model[i].BackFaceRemove();		//back face remove
		switch(i) {
		case 0:
			g_model[i].Scale(vec3(0.2, 0.2, 0.2));
		//	g_model[i].Translate(vec3(-3.0, 0.0, 0.0));
			break;
		case 1:
			g_model[i].Scale(vec3(1.0, 1.0, 1.0));
			g_model[i].Translate(vec3(0.0, 0.0, 0.0));
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
void render( void ) {

	// clear color and depth buffer
	glClearColor (0.0, 0.0, 0.0, 1.0);

	// draw model
	Matrix temp;
	for(int i=0; i<1; i++) {
		//Recover_Matr(i);
		
		for(int j=0; j<16; j++)
			temp.mat[i]=globalCamera.cm_mat.mat[i];
		temp.MultiMatrix(g_model[i].md_mat);
		g_model[i].DrawEdges(&temp);
	}
	// swap back and front buffers
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
	perspective(Theta, (GLfloat)winWidth/(GLfloat)winHeight, Dnear, Dfar, globalProjection);
	
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
	
	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( (int)winWidth, (int)winHeight ); 
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