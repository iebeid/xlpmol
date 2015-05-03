
#include "Bond.h"
#include "QAtom.h"
#include "Point3D.h"

float stick_radius;

static int lx=0,ly=0;
int BSIZE;
int CSIZE; // number of texels for a patch of an aotm

int BSIZEx, BSIZEy; // number of texels for a pach of a bond
int SubPatchX, SubPatchY; 
    // an atom size is subdivided in this many subpatches for bonds



#define TOO_BIG 0
#define TOO_SMALL 1
#define SIZE_OK 2

bool Bond::AssignNextTextPos(int texsize){
	if (texsize<0) { lx = ly = 0; return true; };

	tx = lx;
	ty = ly;

	if ((lx + BSIZE>texsize) || (ly + CSIZE>texsize)) return false;

	lx += BSIZE;
	if (lx + BSIZE>texsize)  {
		ly += CSIZE;
		lx = 0;
	}
	return true;
}

Bond::Bond(const QAtom &_a, const QAtom &_b) :
atmA(&_a), atmB(&_b)
{

	/*
	a = _a.P();
	b = _b.P();

	dir = (a - b).Normalize();

	float rada = _a.covalentr;
	float radb = _b.covalentr;


	lenght = (a - b).Norm();
	startp = dir^Point3D(0, 0, 1);
	if (startp.SquaredNorm()<0.1) startp = dir^Point3D(0, 1, 0);
	startp.Normalize();

	hetatomFlag = _a.hetatomFlag || _b.hetatomFlag;

	UpdateColors();
	*/
}

void Bond::UpdateColors(){
	col1 = Point3D(atmA->cr, atmA->cg, atmA->cb);
	col2 = Point3D(atmB->cr, atmB->cg, atmB->cb);
}