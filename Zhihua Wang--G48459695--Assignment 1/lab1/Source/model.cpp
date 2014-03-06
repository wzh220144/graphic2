#include "head.h"

void Model::DrawEdges(Matrix *t) {
	vec3 a, b;
	for ( int i = 0; i < numFaces; i++ ) {
		for ( int k = 0; k < faces[ i ].numSides; k++ ) {
			int p0=faces[i].indices[k];
			int p1=faces[i].indices[(k+1)%faces[i].numSides];
			a=verts[p0]*(*t);
			b=verts[p1]*(*t);
			glBegin( GL_LINES );
				glVertex3fv( a.ptr() );
				glVertex3fv( b.ptr() );
			glEnd();
		}
	}
}

Model::Model() {
	numVerts = 0;
	numFaces = 0;
	verts = NULL;
	normalVerts=NULL;
	tangentVerts=NULL;
	//textureVerts=NULL;
	faces = NULL;

	md_mat.init();
	pj_mat.init();
}

Model::~Model() {
	if ( verts ) {
		delete[] verts;
		verts = NULL;
	}
	if(normalVerts) {
		delete[] normalVerts;
		normalVerts=NULL;
	}
	if(tangentVerts) {
		delete[] tangentVerts;
		tangentVerts=NULL;
	}
	if ( faces ) {
		delete[] faces;
		faces = NULL;
	}

	numVerts = 0;
	numFaces = 0;
}

	// scale model
void Model::Scale(vec3 offset) {
	Matrix temp;
	temp.init();
	temp.mat[0]=offset.x;
	temp.mat[5]=offset.y;
	temp.mat[10]=offset.z;
	md_mat.MultiMatrix(temp);
}

void Model::Translate( const vec3 &offset ) {
	Matrix temp;
	temp.init();
	temp.mat[12]=offset.x;
	temp.mat[13]=offset.y;
	temp.mat[14]=offset.z;
	md_mat.MultiMatrix(temp);
}

void Model::Rotate( float angle, vec3 vector) {
	vector.normalize();
	float d=sqrt(vector.y*vector.y+vector.z*vector.z);
	Matrix m1, m2, m3, m4;
	m1.init(), m2.init(), m3.init(), m4.init();

	if(d>=1e-10) {
		m4.mat[5]=vector.z/d;
		m4.mat[6]=vector.y/d;
		m4.mat[9]=-vector.y/d;
		m4.mat[10]=vector.z/d;
		m3.mat[0]=d;
		m3.mat[2]=vector.x;
		m3.mat[8]=-vector.x;
		m3.mat[10]=d;
		m1.mat[5]=m4.mat[5];
		m1.mat[6]=-m4.mat[6];
		m1.mat[9]=-m4.mat[9];
		m1.mat[10]=m4.mat[10];
		m2.mat[0]=m3.mat[0];
		m2.mat[2]=-m3.mat[2];
		m2.mat[8]=-m3.mat[8];
		m2.mat[10]=m3.mat[10];
		m3.MultiMatrix(m4);
		m1.MultiMatrix(m2);
		m2.init();
		m2.mat[0]=cos(angle);
		m2.mat[4]=-sin(angle);
		m2.mat[1]=sin(angle);
		m2.mat[5]=cos(angle);
		m1.MultiMatrix(m2);
		m1.MultiMatrix(m3);
		md_mat.MultiMatrix(m1);
	}

	else {
		m3.mat[0]=d;
		m3.mat[2]=vector.x;
		m3.mat[8]=-vector.x;
		m3.mat[10]=d;

		m1.mat[0]=m3.mat[0];
		m1.mat[2]=-m3.mat[2];
		m1.mat[8]=-m3.mat[8];
		m1.mat[10]=m3.mat[10];

		m2.mat[0]=cos(angle);
		m2.mat[4]=-sin(angle);
		m2.mat[1]=sin(angle);
		m2.mat[5]=cos(angle);

		m1.MultiMatrix(m2);
		m1.MultiMatrix(m3);
		md_mat.MultiMatrix(m1);
	}
}

	// load model from .d file
bool Model::LoadModel( const char *mdPath, int id) {
		
	ID=id;
	if ( (mdPath==NULL) )
		return false;

	FILE *mdFp = fopen(mdPath, "r" );			// open file
	if ( (mdFp==NULL) )
		return false;

	fscanf( mdFp, "data%d%d", &numVerts, &numFaces );	// num of vertices and indices

	// alloc vertex normal, vertex and index buffer
	verts = new vec3[ numVerts ];
	faces = new ModelFace[ numFaces ];
		

	// read vertices
	for ( int i = 0; i < numVerts; i++ ) {
		fscanf( mdFp, "%f%f%f", &verts[ i ].x, &verts[ i ].y, &verts[ i ].z );
	}

	// read indices
	for ( int i = 0; i < numFaces; i++ ) {
		ModelFace *face = &faces[ i ];
		fscanf( mdFp, "%i", &face->numSides );
		faces[ i ].indices = new int[ face->numSides ];
		for( int k = 0; k < face->numSides; k++ )
			fscanf( mdFp, "%i", &face->indices[ k ] );
	}

	// close file
	fclose( mdFp );

	return true;
}

void Model::BackFaceRemove() {
	int tfaceNum=0;
	for(int i=0; i<numFaces; i++) {
		vec3 normal=Xproduct( (verts[faces[i].indices[1]]-verts[faces[i].indices[0]]), (verts[faces[i].indices[2]]-verts[faces[i].indices[1]]));
		vec3 pointInFace=(verts[faces[i].indices[0]]+verts[faces[i].indices[2]])/2.0;
		vec3 n=pointInFace-globalVector[0];		//globalVector is the eye coordinate
		double temp=n*normal;
		if(temp>0) {
			faces[tfaceNum]=faces[i];
			tfaceNum++;
		}
	}
	//printf("%d\n", numFaces);
	numFaces=tfaceNum;
	printf("%d\n", numFaces);
}
