#ifndef __GWU_GRAPHIC__
#define __GWU_GRAPHIC__

//================================
// ModelFace
//================================

class ModelFace {
public :
	int numSides;
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

class Vert {
	public:
		vec3 vert;
		vec3 normalVert;
		vec3 textureVert;
		vec3 tangentVert;
		vec3 curVert;
};


class Edge {
	public:
		int ymaxID, yminID;
		float xIncreasement;
		float zIncreasement;
		float x, z;

		Edge & operator= ( const Edge &v );
};

#endif //__GWU_GRAPHIC__