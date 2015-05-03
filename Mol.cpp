
#include "Mol.h"

#define RADIUS_MEAN 0.02f
#define RADIUS_VAR 0.06f


Mol::Mol(){
	natm = nhetatm = 0;
	ready = false;
}
Mol::Mol(const char * _filename)
{
	natm = nhetatm = 0;
	ready = false;
	char filename[1024];

	FILE *f = fopen(_filename, "r");
	cout << _filename << endl;
	if (!f) {
		cout << "Error reading pdb file";
	}

	long size = 0;
	if (fseek(f, 0, SEEK_END) == 0)
		size = ftell(f);

	fseek(f, 0, SEEK_SET);
	char buf[82];
	buf[81] = 0;
	this->atom.clear();
	this->bond.clear();
	this->textureAssigned = false;

	int lineN = 0;
	vector<int> tmpBondVec;

	while (1){
		if (!fgets(buf, 81, f)) break;
		lineN++;

		string st(buf);
		if ((strcmp(st.substr(0, 6).c_str(), "ATOM  ") == 0) ||
			(strcmp(st.substr(0, 6).c_str(), "HETATM") == 0)){
			this->atom.push_back(QAtom(st));
		}
		if (strcmp(st.substr(0, 6).c_str(), "CONECT") == 0){
			int startAtom = atoi(st.substr(6, 5).c_str());
			int i = 1;
			while (st.size() > 6 + i * 5 + 4 && isdigit(st[6 + i * 5 + 4]))
			{
				int secondAtom = atoi(st.substr(6 + i * 5, 5).c_str());
				++i;
				tmpBondVec.push_back(startAtom - 1);
				tmpBondVec.push_back(secondAtom - 1);
				this->AddBond(startAtom - 1, secondAtom - 1);
			}
		}
		if (strcmp(st.substr(0, 6).c_str(), "SHEET ") == 0){
		}
		if (strcmp(st.substr(0, 6).c_str(), "SEQRES") == 0){
		}
		if (strcmp(st.substr(0, 6).c_str(), "HELIX ") == 0){
		}
		if (strcmp(st.substr(0, 6).c_str(), "CRYST1") == 0){
		}
		if (strcmp(st.substr(0, 6).c_str(), "REMARK") == 0){
		}
		if (strcmp(st.substr(0, 6).c_str(), "HEADER") == 0){
		}
		if (strcmp(st.substr(0, 6).c_str(), "TITLE ") == 0){
		}
		if (strcmp(st.substr(0, 6).c_str(), "COMPND") == 0){
		}
	}

	this->nhetatm = 0;  this->natm = 0;
	for (int i = 0; i < this->atom.size(); i++){
		if (this->atom[i].hetatomFlag)  this->nhetatm++; else this->natm++;
	}

	this->ComputeSize();

	for (int i = 0; i < tmpBondVec.size(); i += 2) {
		this->AddBond(tmpBondVec[i], tmpBondVec[i + 1]);
	}
	this->ready = true;
}



RGB Mol::colorConverter(int hexValue)
{
	RGB rgbColor;
	rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
	rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
	rgbColor.b = ((hexValue)& 0xFF) / 255.0;        // Extract the BB byte
	return rgbColor;
}

std::vector<std::string> Mol::readFile(const char * _filename){
	std::vector<std::string> data;
	std::ifstream file(_filename);
	std::string str;
	while (std::getline(file, str))
	{
		data.push_back(str);
	}
	return data;
}

void Mol::AddBond(int i, int j)
{
	bond.push_back(Bond(atom[i], atom[j]));
}

float Mol::ComputeSize(){
	float x0, y0, z0, x1, y1, z1;
	if (atom.size()>0)
		x0 = x1 = atom[0].px;
	y0 = y1 = atom[0].py;
	z0 = z1 = atom[0].pz;
	for (int i = 1; i<atom.size(); i++) {
		if (x1>atom[i].px) x1 = atom[i].px;
		if (y1 > atom[i].py) y1 = atom[i].py;
		if (z1 > atom[i].pz) z1 = atom[i].pz;
		if (x0 < atom[i].px) x0 = atom[i].px;
		if (y0 < atom[i].py) y0 = atom[i].py;
		if (z0 < atom[i].pz) z0 = atom[i].pz;
	}
	px = (x0 + x1) / 2;
	py = (y0 + y1) / 2;
	pz = (z0 + z1) / 2;
	int k = -4;
	r = sqrt((x1 - x0 + k)*(x1 - x0 + k) + (y1 - y0 + k)*(y1 - y0 + k) + (z1 - z0 + k)*(z1 - z0 + k));
	r *= 0.5;
	return r;
}

GLuint Mol::Draw(){

	GLuint vertexBuffer = 0, vertexArray = 0;
	unsigned int atomNumbersArraySize = this->atom.size() * 12;
	GLfloat *h_data = new GLfloat[atomNumbersArraySize];
	unsigned int j = 0;
	for (unsigned int i = 0; i < atomNumbersArraySize; i = i + 12)
	{
		h_data[i] = (GLfloat)this->atom[j].px / 20; // vertex.x
		h_data[i + 1] = (GLfloat)this->atom[j].py / 20; // vertex.y
		h_data[i + 2] = (GLfloat)this->atom[j].pz / 20; // vertex.z
		h_data[i + 3] = 0.5f; // vertex.w
		RGB rgbColor = colorConverter(this->atom[j].atomColor);
		h_data[i + 4] = rgbColor.r; // Red
		h_data[i + 5] = rgbColor.g; // Green
		h_data[i + 6] = rgbColor.b; // Blue
		h_data[i + 7] = 1.0f; // Alpha
		h_data[i + 8] = RADIUS_VAR * this->ComputeSize() * RADIUS_MEAN;
		//h_data[i + 9] = 0.0f;
		//h_data[i + 10] = 0.0f;
		//h_data[i + 11] = 0.0f;
    	j++;
	}

	if (!vertexBuffer){
		glGenBuffers(1, &vertexBuffer);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, atomNumbersArraySize * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	void* d_data = (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	if (d_data == NULL)
	{
		fprintf(stderr, "Could not map gpu buffer.\n");
		exit(1);
	}
	memcpy(d_data, h_data, atomNumbersArraySize * sizeof(GLfloat));
	if (!glUnmapBuffer(GL_ARRAY_BUFFER))
	{
		fprintf(stderr, "Unmap buffer failed.\n");
		exit(1);
	}
	d_data = NULL;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	delete[] h_data;
	if (!vertexArray){
		glGenVertexArrays(1, &vertexArray);
	}
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // color
	glEnableVertexAttribArray(2); // radius
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12 * 4, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * 4, (GLvoid*)16);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 12 * 4, (GLvoid*)32);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vertexArray;
}