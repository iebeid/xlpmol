
#ifndef GLTest_Bond_h
#define GLTest_Bond_h

#include "QAtom.h"
#include "Point3D.h"

class Bond{
public:
    
    Point3D a, b;
    
	Point3D dir;
	Point3D col1, col2;
     
    float lenght;
	Point3D startp;
    
    const QAtom *atmA, *atmB;
    
    bool hetatomFlag;
	Point3D cola, colb;
    void Draw();
    void DrawHalo();
    void DrawShadowmap();
	Bond(Point3D a, Point3D b, float rada, float radb, Point3D col1, Point3D col2);
    Bond(const QAtom &a,const QAtom &b);
    
    void UpdateColors(); // copies color from atoms
    
    void DrawOnTexture();  // for GPU ao.
    bool AssignNextTextPos(int texsize);
    int tx, ty;

    
};

#endif
