
#ifndef _QATOM_H
#define _QATOM_H
#include<string>
#include "gl_globals.h"
#include "Point3D.h"
using namespace std;
#define MAX_COVALENT_RADIUS 1.688f
class QAtom{
public:
    float px,py,pz,r;
    float cr,cg,cb;
    int serial;
    char atomName;
    string fullName;
    int chainIndex;
    unsigned int atomColor;
    float spacefillr, covalentr;
    int tx,ty; // starting places for texture
    bool hetatomFlag;

	QAtom(std::string st);

	int getAtomColor(const char* atomicElement);
	float getAtomRadius(const char* atomicElement);
	float getAtomCovalentRadius(const char* atomicElement);
	int getChainColor(int chianIndex);
	void getCol(const char* st);

	inline Point3D P() const {
		return Point3D(px, py, pz);
	}
};
#endif