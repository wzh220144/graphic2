#ifndef __GWU_MODEL__
#define __GWU_MODEL__

//================================
// ModelFace
//================================
class ModelFace {
public :
	int		numSides;
	int *	indices;

public :
	ModelFace() {
		numSides = 0;
		indices = NULL;
	}

	~ModelFace() {
		if ( indices ) {
			delete[] indices;                                                                                                                                                                                                                                                                                                                                                                                                  
			indices = NULL;
		}
		numSides = 0;
	}
};

//================================
// Model
//================================
class Model {

public :
	int			numVerts;
	int			numFaces;
	int ID;		//model index
	vec3 *		verts;
	vec3 *		normalVerts;
	//vec3 *textureVerts;
	vec3 *		tangentVerts;
	ModelFace *	faces;
	Matrix md_mat;	//model's transformation matrix
	Matrix pj_mat;	//model's projection transformation matrix:if we need projection model separately, we use this matrix
	

public :

	Model();
	~Model();
	void DrawEdges(Matrix *t);
	void Scale(vec3 offset);	// scale model
	void Translate( const vec3 &offset );		//translate model
	void Rotate( float angle, vec3 vector);		//rotate model
	bool LoadModel( const char *mdPath, int id);	// load model from .d file
	void BackFaceRemove();	//back face remove
	
};


#endif // __GWU_MODEL__