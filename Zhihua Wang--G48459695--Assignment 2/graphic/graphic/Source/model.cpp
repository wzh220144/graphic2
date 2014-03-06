#include "head.h"

void Model::DrawEdges(Matrix *t) {
	vec3 a, b;
	ModelFace **Faces;
	int num;
	if(flagBackRemove) {
		num=backRemoveNum;
		Faces=backRemoveFaces;
		for ( int i = 0; i < num; i++ ) {
			glBegin(GL_POLYGON);
			for ( int k = 0; k < Faces[ i ]->numSides; k++ ) {
				int p0=Faces[i]->indices[k];
				int p1=Faces[i]->indices[(k+1)%Faces[i]->numSides];
				a=verts[p0].vert*(*t);
				glVertex3fv( a.ptr() );
			}
			glEnd();
		}
	}
	else { 
		for ( int i = 0; i < numFaces; i++ ) {
			glBegin(GL_POLYGON);
			for ( int k = 0; k < faces[ i ].numSides; k++ ) {
				int p0=faces[i].indices[k];
				int p1=faces[i].indices[(k+1)%faces[i].numSides];
				a=verts[p0].vert*(*t);
				b=verts[p1].vert*(*t);
				//glBegin( GL_LINES );
					glVertex3fv( a.ptr() );
					glVertex3fv( b.ptr() );
				//glEnd();
			}
			glEnd();
		}
	}

	flagBackRemove=0;
}

Model::Model() {
	numVerts = 0;
	numFaces = 0;
	faces = NULL;
	verts=NULL;

	md_mat.init();
	pj_mat.init();

	flagBackRemove=0;
	backRemoveNum=0;
	backRemoveFaces=NULL;
}

Model::~Model() {
	if ( verts ) {
		delete[] verts;
		verts = NULL;
	}
	if ( faces ) {
		delete[] faces;
		faces = NULL;
	}

	if(backRemoveFaces) {
		delete[] backRemoveFaces;
		faces = NULL;
	}

	numVerts = 0;
	numFaces = 0;
	backRemoveNum=0;
	flagBackRemove=0;
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
bool Model::LoadModel( const char *mdPath, const char *norPath, const char *tangPath, int id) {
		

	flagBackRemove=0;
	ID=id;
	if ( (mdPath==NULL) )
		return false;

	FILE *mdFp = fopen(mdPath, "r" );			// open file
	if ( (mdFp==NULL) )
		return false;
	FILE *norFp = fopen(norPath, "r" );			// open file
	if ( (norFp==NULL) )
		return false;
	FILE *tangFp = fopen(tangPath, "r" );			// open file
	if ( (tangFp==NULL) )
		return false;

	fscanf( mdFp, "data%d%d", &numVerts, &numFaces );	// num of vertices and indices

	// alloc vertex normal, vertex and index buffer
	verts = new Vert[ numVerts ];
	faces = new ModelFace[ numFaces ];

	backRemoveFaces = new ModelFace *[numFaces];

	// read vertices
	for ( int i = 0; i < numVerts; i++ ) {
		fscanf( mdFp, "%f%f%f", &verts[ i ].vert.x, &verts[ i ].vert.y, &verts[ i ].vert.z );
		fscanf( norFp, "%f%f%f", &verts[ i ].normalVert.x, &verts[ i ].normalVert.y, &verts[ i ].normalVert.z );
		fscanf( tangFp, "%f%f%f", &verts[ i ].tangentVert.x, &verts[ i ].tangentVert.y, &verts[ i ].tangentVert.z );
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

void Model::BackFaceRemove(vec3 viewPoint) {
	flagBackRemove=1;
	backRemoveNum=0;
	for(int i=0; i<numFaces; i++) {
		int p1=faces[i].indices[1], p0=faces[i].indices[0], p2=faces[i].indices[2];
		vec3 normal=Xproduct( (verts[p1].vert-verts[p0].vert), (verts[p2].vert-verts[p1].vert) );
		vec3 pointInFace=( verts[p0].vert+verts[p1].vert )/2.0;
		vec3 n=pointInFace-viewPoint;		//globalVector is the eye coordinate
		double temp=n*normal;
		if(temp>0) {
			backRemoveFaces[backRemoveNum]=&faces[i];
			backRemoveNum++;
		}
	}
}

void Model::DrawLine(float a, float b, float za, float zb, int yID, vec3 *c) {
	int aID=xfind(a);
	int bID=xfind(b);
	float z=za;
	float zIncreasement;
	zIncreasement=(zb-za)/((float)(a-b+1));
	for(int i=aID; i<=bID; i++) {
		if( z>Zbuffer[i][yID] ) {
			Zbuffer[i][yID]=z;
			Ibuffer[i][yID]=*c;
		}
		else if(z==Zbuffer[i][yID]) {
		}
		z+=zIncreasement;
	}
}



void Model::ScanConvertion(Matrix *t, ModelFace *f, vec3 *c) {

	int l=f->numSides;
	int y[2];
	Edge *e=new Edge[l];	//edge table
	Edge *te=new Edge[l];	//active edge
	int yMax=-1, yMin=PIXELY+200;
	int le=0;
	int p0=f->indices[0], p1;
	float xMax=verts[p0].curVert.x, xMin=verts[p0].curVert.x;
	float zMax=verts[p0].curVert.z, zMin=verts[p0].curVert.z;

	//create edge table
	for(int i=0; i<l; i++) {

		p0=f->indices[i];
		p1=f->indices[(i+1)%l];
		y[0]=yfind(verts[p0].curVert.y);
		y[1]=yfind(verts[p1].curVert.y);
		if(xMax<verts[p0].curVert.x) {
			xMax=verts[p0].curVert.x;
			zMax=verts[p0].curVert.z;
		}
		if(xMin>verts[p0].curVert.x) {
			xMin=verts[p0].curVert.x;
			zMin=verts[p0].curVert.z;
		}
		if(y[0]==y[1]) {	//if edge is horizon
			if((i==l-1)&&(le==0))	 {
				e[le].x=xMin;
				e[le].z=zMin;
				e[le].yminID=y[0];
				e[le].ymaxID=y[0];
				le++;
				e[le].x=xMax;
				e[le].z=zMax;
				e[le].yminID=y[0];
				e[le].ymaxID=y[0];
				yMin=Min(e[le].yminID, yMin);
				yMax=Max(e[le].ymaxID, yMax);
				le++;
			}
			continue;
		}
		float t=fabs((float)y[0]-(float)y[1]);
		e[le].xIncreasement=( verts[p1].curVert.x-verts[p0].curVert.x)/t;
		e[le].zIncreasement=( verts[p1].curVert.z-verts[p0].curVert.z )/t;
		if(verts[p0].curVert.y<verts[p1].curVert.y) {
			e[le].x=verts[p0].curVert.x;
			e[le].z=verts[p0].curVert.z;
		//	y[1]--;
		}
		else {
			e[le].zIncreasement=-e[le].zIncreasement;
			e[le].xIncreasement=-e[le].xIncreasement;
			e[le].x=verts[p1].curVert.x;
			e[le].z=verts[p1].curVert.z;
		//	e[le].x+=e[le].xIncreasement;
			//e[le].z+=e[le].zIncreasement;
		//	y[1]++;
		}
		e[le].yminID=Min(y[0], y[1]);
		e[le].ymaxID=Max(y[0], y[1]);
		yMin=Min(e[le].yminID, yMin);
		yMax=Max(e[le].ymaxID, yMax);
		le++;
	}

	sort(e, e+le, cmp1);

	int a=0;
	int flag;
	int tl=0, ttl;

	//use active edge to draw picture
	for(int i=yMin; i<=yMax; i++) {

		//add edges whose yMin equal to i
		while(a<le) {
			if(e[a].yminID==i) {
				te[tl++]=e[a++];
			}
			else break;
		}
		sort(te, te+tl, cmp2);

		//draw lines
		flag=0;
		for(int j=0; j<tl; j++) {
	//		if(flag) {
				if(j==tl-1) {
					DrawLine(te[j].x, te[j].x, te[j].z, te[j].z, i, c);
				}
				else
					DrawLine(te[j].x, te[j+1].x, te[j].z, te[j+1].z, i, c);
	//		}

		}

		//delete the edges whose yMax is equal to i
		ttl=0;
		for(int j=0; j<tl; j++) {
			if(te[j].ymaxID==i) 
				continue;
			te[j].x+=te[j].xIncreasement;
			te[j].z+=te[j].zIncreasement;
			te[ttl++]=te[j];
		}
		tl=ttl;
	}

	//collect memory
	delete[] e;
	delete[] te;

}

void Model::ScanConvertion(Matrix *t, vec3 *c) {
	for(int i=0; i<numVerts; i++)
		verts[i].curVert=verts[i].vert*(*t);
	if(flagBackRemove) {
		for(int i=0; i<backRemoveNum; i++) {
			ScanConvertion(t, backRemoveFaces[i], c);
		}
	}
	else {
		for(int i=0; i<numFaces; i++) {
			ScanConvertion(t, &faces[i], c);
		}
	}
}