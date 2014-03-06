#ifndef __GWU_MODEL__
#define __GWU_MODEL__

//================================
// Model
//================================
class Model {

public :
	int			numVerts;
	int			numFaces;
	int ID;		//model index
	Vert *verts;
	ModelFace *	faces;
	Matrix md_mat;	//model's transformation matrix
	Matrix pj_mat;	//model's projection transformation matrix:if we need projection model separately, we use this matrix

	int flagBackRemove;

	int backRemoveNum;
	ModelFace **backRemoveFaces;

	int shadingModel;		//0 is constant shading, 1 is gouraud shading, 2 is phong shading

public :

	Model();
	~Model();
	void DrawEdges(Matrix *t);
	void Scale(vec3 offset);	// scale model
	void Translate( const vec3 &offset );		//translate model
	void Rotate( float angle, vec3 vector);		//rotate model
	bool LoadModel( const char *mdPath, int id);	// load model from .d file
	void BackFaceRemove(vec3 viewPoint);	//back face remove
	void ScanConvertion(vec3 viewPoint, vec3 *light, vec3 *lightPosition, int lightNum);		///scan covertion
	void ScanConvertion(vec3 viewPoint, vec3 *light, vec3 *lightPosition, int lightNum, ModelFace *f);	//used for ScanCovertion
	void DrawLine(float a, float b, float za, float zb, vec3 na, vec3 nb, int yID, vec3 *c, vec3 *v, vec3 *l);
	void DrawLine(float a, float b, float za, float zb, vec3 ca, vec3 cb, int yID);
	void DrawLine(float a, float b, float za, float zb, int yID, vec3 *c);
	void ComputeNormal();		//compute normal for every vertex and every polygon
	void ComputeVerts(Matrix *t);	//compute current vertex based on given matrix
	void PhongShading();
	void GouraudShading();
	void ConstantShading();
	vec3 illuminationCompute(vec3 *n, vec3 *l, vec3 *v, vec3 *c);
};


#endif // __GWU_MODEL__