#include "head.h"

Edge &Edge::operator=( const Edge &e ) {
	x=e.x;
	xIncreasement=e.xIncreasement;
	ymaxID=e.ymaxID;
	yminID=e.yminID;
	z=e.z;
	zIncreasement=e.zIncreasement;
	nIncreasement=e.nIncreasement;
	normal=e.normal;
	return *this;
}