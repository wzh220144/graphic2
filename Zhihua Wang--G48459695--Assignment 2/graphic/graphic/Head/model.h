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

public :

	Model();
	~Model();
	void DrawEdges(Matrix *t);
	void Scale(vec3 offset);	// scale model
	void Translate( const vec3 &offset );		//translate model
	void Rotate( float angle, vec3 vector);		//rotate model
	bool LoadModel( const char *mdPath, const char *norPath, const char *tangPath, int id);	// load model from .d file
	void BackFaceRemove(vec3 viewPoint);	//back face remove
	void ScanConvertion(Matrix *t, vec3 *c);		///scan covertion
	void ScanConvertion(Matrix *t, ModelFace *f, vec3 *c);	//used for ScanCovertion
	void DrawLine(float a, float b, float za, float zb, int yID, vec3 *c);
};


#endif // __GWU_MODEL__