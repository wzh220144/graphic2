#ifndef __GWU_CAMERA__
#define __GWU_CAMERA__

class Camera {

public:
	Matrix cm_mat;
	int ID;
	vec3 position;

public:
	Camera();
	~Camera();
	void setLookAt(vec3 eye, vec3 center, vec3 up, int id);
	void Translate( const vec3 &offset );
	void antiRotate( float angle, vec3 vector);
	void Rotate( float angle, vec3 vector);
};

#endif  //__GWU_CAMERA