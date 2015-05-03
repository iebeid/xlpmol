
#include "gl_globals.h"
#include "shader.h"
#include "camera.h"
#include "Mol.h"
#include <iostream>
#include <cstring>
#include <string>

#define FIELD_OF_VIEW 90.0f

typedef struct
{
	int buttons;
	int x, y;
	int button_x;
	int button_y;
} mouse_state_t;

Mol myMol;
Mol myMol_autre;

unsigned int atomNumbers = 0;
unsigned int atomNumbersFirst = 0;
unsigned int atomNumersSecond = 0;
bool recompile = false;
GLuint vertexBuffer = 0, vertexArray = 0;
GLuint vertexBufferAutre = 0, vertexArrayAutre = 0;
ShaderManager shsphere;
Camera camera;
mouse_state_t g_mouse = { 0, 0, 0, 0, 0 };
int width = 640, height = 480;
float fov = FIELD_OF_VIEW;
std::string firstPdbFile;
std::string secondPdbFile;
std::string vertexShader;
std::string fragmentShader;

using namespace std;

void getInputFiles(int argc, char** argv){
	cout << argv[1] << endl;
	std::vector<std::string> linemap = myMol.readFile(argv[1]);
	firstPdbFile = linemap[0];
	secondPdbFile = linemap[1];
	vertexShader = linemap[2];
	fragmentShader = linemap[3];
	char const * firstPdbFileConv = firstPdbFile.c_str();
	char const * secondPdbFileConv = secondPdbFile.c_str();

	myMol = Mol(firstPdbFileConv);
	atomNumbersFirst = myMol.atom.size();
	myMol_autre = Mol(secondPdbFileConv);
	atomNumersSecond = myMol_autre.atom.size();
	atomNumbers = atomNumbersFirst + atomNumersSecond;
	//std::vector<std::string> linemap = myMol.readFile("data/map.txt");
	//for (std::vector<std::string>::iterator it = linemap.begin(); it != linemap.end(); ++it) {
		//cout << *it << endl;
	//}
	//std::vector<std::string> linefasta = myMol.readFile("data/bsa.fasta");
	//for (std::vector<std::string>::iterator it = linefasta.begin(); it != linefasta.end(); ++it) {
		//cout << *it << endl;
	//}
	//std::vector<std::string> linedata = myMol.readFile("data/data.txt");
	//for (std::vector<std::string>::iterator it = linedata.begin(); it != linedata.end(); ++it) {
		//cout << *it << endl;
	//}
}

int create_shader()
{
	if (shsphere.isLoaded())
		shsphere.unload();

	shsphere.load(vertexShader.c_str(), fragmentShader.c_str());

	int s = shsphere.link();
	if (s)
	{
		printf("Error occurred.\n");
		return 1;
	}
	if (CHECK_GLERROR() != GL_NO_ERROR)
		return 1;

	return 0;
}

void display()
{
	if (recompile)
	{
		printf("Recompile...\n");
		if (create_shader() != 0)
			exit(1);

		recompile = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glTranslatef(0.0f, 0.0f, -5.0f);
	//glm::vec4 lightPos = camera.modelview_glm() * glm::vec4(1.5, 2.5, 1.5, 0.0);
	glm::vec4 lightPos = camera.modelview_glm() * glm::vec4(1.0, 1.0, 1.0, 0.0);
	//glm::vec4 camPos = camera.modelview_glm() * camera.position_glm();



	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
	glEnable(GL_POINT_SPRITE_ARB);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	glDepthMask(GL_TRUE);

	shsphere.bind();
	shsphere.setUniformVar("lightPos", &lightPos.x);
	shsphere.setUniformMat4("MVMatrix", (float*)camera.modelview());
	shsphere.setUniformMat4("PMatrix", (float*)camera.projection());
	shsphere.setUniformVar("screenWidth", width);

	glBindVertexArray(vertexArray);
	glDrawArrays(GL_POINTS, 0, atomNumbersFirst);
	glBindVertexArray(0);
	
	glBindVertexArray(vertexArrayAutre);
	glDrawArrays(GL_POINTS, 0, atomNumersSecond);
	glBindVertexArray(0);
	
	shsphere.unbind();
	glDisable(GL_POINT_SPRITE_ARB);
	if (CHECK_GLERROR() != GL_NO_ERROR)
		exit(1);

	glutPostRedisplay();
	glutSwapBuffers();

	if (CHECK_GLERROR() != GL_NO_ERROR)
		exit(1);
}

void cleanup()
{
	glDeleteVertexArrays(1, &vertexArray);
	vertexArray = 0;

	glDeleteBuffers(1, &vertexBuffer);
	vertexBuffer = 0;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
	case '+':
		fov -= 1.f;
		camera.applyProjection(fov, width, height);
		break;
	case '-':
		fov += 1.f;
		camera.applyProjection(fov, width, height);
		break;
	case 'a':
		camera.strafeCamera(0.01f);
		break;
	case 'd':
		camera.strafeCamera(-0.01f);
		break;
	case 'w':
		camera.distCamera(0.02f);
		break;
	case 's':
		camera.distCamera(-0.02f);
		break;
	case 'q':
		camera.advanceTargetZ(0.05f);
		break;
	case 'e':
		camera.advanceTargetZ(-0.05f);
		break;
	case 'r':
		recompile = true;
		break;
	}
	camera.apply();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		g_mouse.buttons |= 1 << button;
	}
	else if (state == GLUT_UP)
	{
		g_mouse.buttons = 0;
	}

	g_mouse.button_x = x;
	g_mouse.button_y = y;
}

void motion(int x, int y)
{
	float dx, dy;
	dx = (float)(x - g_mouse.button_x);
	dy = (float)(y - g_mouse.button_y);

	if (g_mouse.buttons & 1)
	{
		camera.rotatePosition(0.005f * dx);
		camera.advancePositionZ(-0.02f * dy);
	}
	else if (g_mouse.buttons & 4)
	{
		camera.distCamera(0.002f * dy);
		camera.rotate(0.0005f * dx);
	}

	g_mouse.button_x = x;
	g_mouse.button_y = y;
	camera.apply();
}

void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	camera.applyProjection(fov, width, height);
}

int initGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("XLPM Protein Interaction Viewer");
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glewInit();
	if (!glewIsSupported("GL_VERSION_3_3 "))
	{
		fprintf(stderr, "ERROR: Support for necessary OpenGL extensions missing.");
		fflush(stderr);
		return 1;
	}
	camera.setupCamera(4.0f, 3.0f, 1.0f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.0);
	//camera.setupCamera(14.0f, 14.0f, 14.0f, 0.0f, 0.0f, 0.0f, 0.0f, 4.0f, 4.0f, 4.0f);
	camera.applyProjection(fov, width, height);
//	camera.apply();
//	camera.strafeCamera(-3.0f);
//	camera.apply();
	camera.distCamera(-3.0f);
	camera.rotatePosition(0.5f);
//	camera.rotate(0.5f);
	camera.moveCamera(0.05f);
	camera.apply();
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.662f, 0.662f, 0.662f, 1.0f);

	if (CHECK_GLERROR() != GL_NO_ERROR)
		return 1;

	return 0;
}

int drawProcedure(int argc, char** argv){
	if (initGL(argc, argv) != 0)
	{
		fprintf(stderr, "Unable to init OpenGL.");
		return EXIT_FAILURE;
	}
	getInputFiles(argc, argv);
	vertexArray = myMol.Draw();
	vertexArrayAutre = myMol_autre.Draw();
	if (create_shader() != 0)
	{
		fprintf(stderr, "Unable to create shaders.");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{



	int result = drawProcedure(argc,argv);
	glutMainLoop();
	return result;
}