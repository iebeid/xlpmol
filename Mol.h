#include "QAtom.h"
#include "Bond.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <thread>
#include <fstream>
#include <string>
#include <iterator>
#include <array>
#include <queue>
#include <memory>

using namespace std;

struct RGB{
	float r;
	float g;
	float b;
};

class Mol{

public:

    vector<QAtom> atom;
    vector<Bond> bond;
    float px,py,pz,r;
	int natm, nhetatm;
	bool ready;
    bool textureAssigned;

	Mol();
	Mol(const char * _filename);
	std::vector<std::string> readFile(const char * _filename);
	RGB colorConverter(int hexValue);
	float ComputeSize();
	GLuint Draw();
	GLuint createShaders();
    void UpdateColors();
    void AddBond(int i, int j);
};
