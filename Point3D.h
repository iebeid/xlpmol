
#ifndef _POINT3D_HEADER
#define _POINT3D_HEADER

class Point3D{
	float x;
	float y;
	float z;
public:
	Point3D::Point3D(float px, float py, float pz){
		x = px;
		y = py;
		z = pz;
	}
	Point3D::Point3D(){
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
};

#endif

