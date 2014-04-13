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
bool Model::LoadModel( const char *mdPath, int id) {
		

	flagBackRemove=0;
	ID=id;
	if ( (mdPath==NULL) )
		return false;

	FILE *mdFp = fopen(mdPath, "r" );			// open file
	if ( (mdFp==NULL) )
		return false;

	fscanf( mdFp, "data%d%d", &numVerts, &numFaces );	// num of vertices and indices

	// alloc vertex normal, vertex and index buffer
	verts = new Vert[ numVerts ];
	faces = new ModelFace[ numFaces ];

	backRemoveFaces = new ModelFace *[numFaces];

	// read vertices
	for ( int i = 0; i < numVerts; i++ ) {
		fscanf( mdFp, "%f%f%f", &verts[ i ].vert.x, &verts[ i ].vert.y, &verts[ i ].vert.z );
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



void Model::ScanConvertion(vec3 viewPoint, vec3 *light, vec3 *lightPosition, int lightNum, Texture *texture, ModelFace *f) {
	int l=f->numSides;
	int y[2];
	Edge *e=new Edge[l];	//edge table
	Edge *te=new Edge[l];	//active edge
	int yMax=-1, yMin=PIXELY+200;
	int le=0;
	int p0=f->indices[0], p1;
	float xMax=verts[p0].curVert.x, xMin=verts[p0].curVert.x;
	float zMax=verts[p0].curVert.z, zMin=verts[p0].curVert.z;

	//for(int i=0; i<l; i++) {
		//int p=f->indices[i];
	//	fprintf(aa, "%lf %lf %lf\n", verts[p].curVert.x, verts[p].curVert.y, verts[p].curVert.z);
	//}

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
		float t=fabs((float)y[0]-(float)y[1])+1.0;
		e[le].xIncreasement=( verts[p1].curVert.x-verts[p0].curVert.x)/t;
		e[le].zIncreasement=( verts[p1].curVert.z-verts[p0].curVert.z )/t;
		e[le].nIncreasement=(verts[p1].normalVert-verts[p0].normalVert)/t;
		if(verts[p0].curVert.y<verts[p1].curVert.y) {
			e[le].x=verts[p0].curVert.x;
			e[le].z=verts[p0].curVert.z;
			e[le].normal=verts[p0].normalVert;
		}
		else {
			e[le].zIncreasement=-e[le].zIncreasement;
			e[le].xIncreasement=-e[le].xIncreasement;
			e[le].nIncreasement=-e[le].nIncreasement;
			e[le].x=verts[p1].curVert.x;
			e[le].z=verts[p1].curVert.z;
			e[le].normal=verts[p1].normalVert;
		}
		e[le].yminID=Min(y[0], y[1]);
		e[le].ymaxID=Max(y[0], y[1]);
		yMin=Min(e[le].yminID, yMin);
		yMax=Max(e[le].ymaxID, yMax);
		//printf("p0: %d p1: %d\n", p0, p1);
		//printf("%lf %lf %lf\n", e[le].normal.x, e[le].normal.y, e[le].normal.z);
		le++;
	}

	//set the current face's bounding, this is used to figure out which mipmap is used.
	texture->curMaxX=xMax;
	texture->curMinX=xMin;
	texture->curMaxY=yValue[yMax];
	texture->curMinY=yValue[yMin];

	sort(e, e+le, cmp1);

	int a=0;
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

		vec3 nn, vv, ll;

		//draw lines
		if(shadingModel==0) {
			for(int j=0; j<tl; j++) {
				if(j==tl-1)
					DrawLine(te[j].x, te[j].x, te[j].z, te[j].z, i, &light[0]);
				else
					DrawLine(te[j].x, te[j+1].x, te[j].z, te[j+1].z, i, &light[0]);
			}
		}
		else if(shadingModel==1) {		//gouraund shading
			vec3 ca, cb;
			for(int j=0; j<tl; j++) {
				vv=viewPoint-vec3(te[j].x, yValue[i], te[j].z);
				ll=lightPosition[0]-vec3(te[j].x, yValue[i], te[j].z);
				ca=illuminationCompute(&te[j].normal, &ll, &vv, &light[0]);
				if(j==tl-1) {
					DrawLine(te[j].x, te[j].x, te[j].z, te[j].z, ca, ca, i);
				}
				else {
					vv=viewPoint-vec3(te[j+1].x, yValue[i], te[j+1].z);
					ll=lightPosition[0]-vec3(te[j+1].x, yValue[i], te[j+1].z);
					cb=illuminationCompute(&te[j+1].normal, &ll, &vv, &light[0]);
					DrawLine(te[j].x, te[j+1].x, te[j].z, te[j+1].z, ca, cb, i);
				}
			}
		}
		else if(shadingModel==2) {		//phong shading
			for(int j=0; j<tl; j++) {
				if(te[j].normal.x==0&&te[j].normal.y==0&&te[j].normal.z==0)
					continue;
				if(j==tl-1)
					DrawLine(te[j].x, te[j].x, te[j].z, te[j].z, te[j].normal, te[j].normal, i, &light[0], &viewPoint, &lightPosition[0], texture);
				else
					DrawLine(te[j].x, te[j+1].x, te[j].z, te[j+1].z, te[j].normal, te[j+1].normal, i, &light[0], &viewPoint, &lightPosition[0], texture);
			}
		}

		//delete the edges whose yMax is equal to i
		ttl=0;
		for(int j=0; j<tl; j++) {
			if(te[j].ymaxID==i) 
				continue;
			te[j].x+=te[j].xIncreasement;
			te[j].z+=te[j].zIncreasement;
			te[j].normal+=te[j].nIncreasement;
			te[ttl++]=te[j];
		}
		tl=ttl;
	}

	//collect memory
	delete[] e;
	delete[] te;

}

void Model::ScanConvertion(vec3 viewPoint, vec3 *light, vec3 *lightPosition, int lightNum, Texture *texture) {
	if(flagBackRemove) {
		for(int i=0; i<backRemoveNum; i++) {
		//	fprintf(aa, "Case %d:\n", i);
			ScanConvertion(viewPoint, light, lightPosition, lightNum, texture, backRemoveFaces[i]);
		}
	}
	else {
		for(int i=0; i<numFaces; i++) {
			ScanConvertion(viewPoint, light, lightPosition, lightNum, texture, &faces[i]);
		}
	}
}

void Model::ComputeNormal() {
	for(int i=0; i<numFaces; i++) {
		int p1=faces[i].indices[1], p0=faces[i].indices[0], p2=faces[i].indices[2];
		faces[i].normal=Xproduct( (verts[p1].curVert-verts[p0].curVert), (verts[p2].curVert-verts[p1].curVert) );
		faces[i].normal=faces[i].normal.normalize();
		int l=faces[i].numSides;
		for(int j=0; j<l; j++) {
			int p=faces[i].indices[j];
			verts[p].normalVert=verts[p].normalVert+faces[i].normal;
		}
	}
	for(int i=0; i<numVerts; i++) {
		verts[i].normalVert=verts[i].normalVert.normalize();
		//fprintf(aa, "Case %d: %lf %lf %lf\n", i, verts[i].normalVert.x, verts[i].normalVert.y, verts[i].normalVert.z);
		//printf("Case %d: %lf %lf %lf\n", i, verts[i].normalVert.x, verts[i].normalVert.y, verts[i].normalVert.z);
	}

}

void Model::ComputeVerts(Matrix *t) {

	float maxx, maxy, maxz, minx, miny, minz;
	verts[0].curVert=verts[0].vert*(*t);
	maxx=minx=verts[0].curVert.x;
	maxy=miny=verts[0].curVert.y;
	maxz=minz=verts[0].curVert.z;
	for(int i=1; i<numVerts; i++) {
		verts[i].curVert=verts[i].vert*(*t);
		verts[i].normalVert.x=0.0;
		verts[i].normalVert.y=0.0;
		verts[i].normalVert.z=0.0;
		maxx=max(maxx, verts[i].curVert.x);
		maxy=max(maxy, verts[i].curVert.y);
		maxz=max(maxz, verts[i].curVert.z);
		minx=min(minx, verts[i].curVert.x);
		miny=min(miny, verts[i].curVert.y);
		minz=min(minz, verts[i].curVert.z);
	}
	for(int i=0; i<numVerts; i++) {
		verts[i].curVert.x=(2.0*verts[i].curVert.x-maxx-minx)/(maxx-minx);
	}
}

void Model::BackFaceRemove(vec3 viewPoint) {
	flagBackRemove=1;
	backRemoveNum=0;
	for(int i=0; i<numFaces; i++) {
		int p1=faces[i].indices[1], p0=faces[i].indices[0], p2=faces[i].indices[2];
		vec3 pointInFace=( verts[p0].curVert+verts[p1].curVert )/2.0;
		vec3 n=viewPoint-pointInFace;		//globalVector is the eye coordinate
		n=n.normalize();
		double temp=n*faces[i].normal;
		if(temp>0) {
			backRemoveFaces[backRemoveNum]=&faces[i];
			backRemoveNum++;
		}
	}
}

void Model::ConstantShading() {
	shadingModel=0;
}

void Model::GouraudShading() {
	shadingModel=1;
}

void Model::PhongShading() {
	shadingModel=2;
}

void Model::DrawLine(float a, float b, float za, float zb, vec3 na, vec3 nb, int yID, vec3 *c, vec3 *v, vec3 *l, Texture *texture) {		//phong shading
	int aID=xfind(a);
	int bID=xfind(b);
	float z=za;
	float zIncreasement;
	vec3 ll, vv;
	vec3 n=na;
	vec3 nIncreasement=(nb-na)/((float)(b-a+1));
//	fprintf(aa, "%d\n", yID);
	zIncreasement=(zb-za)/((float)(b-a+1));
	for(int i=aID; i<=bID; i++) {
		if( z>Zbuffer[i][yID] ) {
			Zbuffer[i][yID]=z;
			vv=(*v)-vec3(xValue[i], yValue[yID], z);
			ll=(*l)-vec3(xValue[i], yValue[yID], z);
			//Ibuffer[i][yID]=illuminationCompute(&n, &ll, &vv, c);
			Ibuffer[i][yID]=textureCompute(&n, &vec3(xValue[i], yValue[yID], z), texture);
		//	fprintf(aa, "%d %lf %lf %lf\n", i, Ibuffer[i][yID].x, Ibuffer[i][yID].y, Ibuffer[i][yID].z);
		}
		else if(z==Zbuffer[i][yID]) {
		}
		z+=zIncreasement;
		n+=nIncreasement;
	}
}

vec3 Model::textureCompute(vec3 *n, vec3 *p, Texture *texture) {

	/**********************************use ball as intermediat(one texture for all polygons)********************************************/
	/*float r=2.0;		//support intermediat is ball, and the radium of the ball is 2.0
	float a=n->x*n->x+n->y*n->y+n->z*n->z;
	float b=(n->x*p->x+n->y*p->y+n->z*p->z)*2.0;
	float c=p->x*p->x+p->y*p->y+p->z*p->z-r*r;
	float t;
	float w=texture->sizeX;
	float h=texture->sizeY;
	float PI=acos(-1.0)*2.0;
	unsigned char *data=texture->data;
	int x, y;
	vec3 tex;		//the coordinate of a point in texture domain corressponding to the point in world domain
	vec3 inter;		//
	t=(-b+sqrt(b*b-4.0*a*c))/2.0/a;
	inter.x=p->x+n->x*t;
	inter.y=p->y+n->y*t;
	inter.z=p->z+n->z*t;
	tex.y=acos(inter.z/r)*h/PI;
	tex.x= min(max((inter.y/sin(acos(inter.z/r))/r), -1.0), 1.0);
	tex.x = acos(tex.x)*w/2.0/PI;
	x=tex.x;
	y=tex.y;
	a=data[texture->sizeX*3*y+x*3];
	b=data[texture->sizeX*3*y+x*3+1];
	c=data[texture->sizeX*3*y+x*3+2];
	return vec3(a/255.0, b/255.0, c/255.0);*/

	/***********************************use plane as intermediat(one texture for all polygons)**********************************************/
	/*float w = texture->sizeX;
	float h = texture->sizeY;
	float tx = (p->x+1.0)/2.0;
	float ty = (p->y+1.0)/2.0;
	int len = texture->sizeX;
	int x=tx*w;
	int y=ty*h;
	int a=texture->data[len*3*y+x*3];
	int b=texture->data[len*3*y+x*3+1];
	int c=texture->data[len*3*y+x*3+2];
	return vec3(a/255.0, b/255.0, c/255.0);
	*/

	/***********************************use plane as intermediat(one texture for each polygon)**********************************************/
	float xMax = xfind(texture->curMaxX);
	float xMin = xfind(texture->curMinX);
	float yMax = yfind(texture->curMaxY);
	float yMin = yfind(texture->curMinY);
	float xLen = xMax-xMin+1.0;
	float yLen = yMax-yMin+1.0;
	float tx=(xfind(p->x)-xMin+1.0)/xLen;
	float ty=(yfind(p->y)-yMin+1.0)/yLen;
	int t = max(xLen, yLen);
	int i;
	//find the right mipmap
	for(i=0; i<texture->count; i++) {
		if(t<=texture->texil[i])
			break;
	}
	if(i==texture->count)
		i--;
	int len=texture->texil[i];		//this the number of texil that the right mipmap has
	int h=len, w=len;
	int x=tx*w;
	int y=ty*h;
	int a=texture->mipmap[i][len*3*y+x*3];
	int b=texture->mipmap[i][len*3*y+x*3+1];
	int c=texture->mipmap[i][len*3*y+x*3+2];
	return vec3(a/255.0, b/255.0, c/255.0);
}

void Model::DrawLine(float a, float b, float za, float zb, vec3 ca, vec3 cb, int yID) {		//gouraud shading
	int aID=xfind(a);
	int bID=xfind(b);
	float z=za;
	float zIncreasement;
	vec3 c=ca;
	vec3 cIncreasement=(cb-ca)/((float)(b-a+1));
	zIncreasement=(zb-za)/((float)(b-a+1));
	for(int i=aID; i<=bID; i++) {
		if( z>Zbuffer[i][yID] ) {
			Zbuffer[i][yID]=z;
			Ibuffer[i][yID]=c;
		}
		else if(z==Zbuffer[i][yID]) {
		}
		z+=zIncreasement;
		c+=cIncreasement;
	}
}

void Model::DrawLine(float a, float b, float za, float zb, int yID, vec3 *c) {		//constant shading
	int aID=xfind(a);
	int bID=xfind(b);
	float z=za;
	float zIncreasement;
	zIncreasement=(zb-za)/((float)(b-a+1));
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

vec3 Model::illuminationCompute(vec3 *n, vec3 *l, vec3 *v, vec3 *c) {
	vec3 a;
	(*l)=l->normalize();
	(*v)=v->normalize();
	vec3 r=2.0* (*n) * ( (*n)*(*l) ) - (*l);
	r=r.normalize();
	float t=max(0.0, (*n)*(*l));
	float tt=max(0.0, (*v)*r);
	a.x=t*(c->x)*Kd+pow( tt, sn)*Ks*(c->x)+ambientColor[0].x+ globalColor[ID].x;
	a.y=t*(c->y)*Kd+pow( tt, sn)*Ks*(c->y)+ambientColor[0].y+ globalColor[ID].y;
	a.z=t*(c->z)*Kd+pow( tt, sn)*Ks*(c->z)+ambientColor[0].z+ globalColor[ID].z;
	return a;
}