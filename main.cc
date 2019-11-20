//******************************************************************* 
//                                                                    
//  Program:                                               
//                                                                     
//  Author:      Scott Craford
//  Email:       sc573016@ohio.edu
//                                                                    
//                                                                    
//  Description:                 
//                                                                	
//                                                                    
//*******************************************************************

#include "Angel.h"
#include "cube.h"
#include "world.h"
#include "camera.h"
#include "output.h"
#include "hud.h"
#include <iostream>

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
const int NumOFBlocks = 1;

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

//this is need to import the shader to the grapics card
#pragma warning(disable : 4996)//needed to getting the shader importer to work
namespace Angel {

	// Define a new warp pointer for everyone to use.
	void WarpPointer(int x, int y) {
#ifdef __APPLE__
		CGPoint warpPoint = CGPointMake(x, y);
		CGWarpMouseCursorPosition(warpPoint);
		CGAssociateMouseAndMouseCursorPosition(true);
#else
		glutWarpPointer(x, y);
#endif
	}

	// Create a NULL-terminated string by reading the provided file
	static char* readShaderSource(const char* shaderFile)
	{
		FILE* fp = fopen(shaderFile, "r");

		if (fp == NULL) {
			return NULL;
		}

		fseek(fp, 0L, SEEK_END);
		long size = ftell(fp);

		fseek(fp, 0L, SEEK_SET);
		char* buf = new char[size + 1];
		fread(buf, 1, size, fp);

		buf[size] = '\0';
		fclose(fp);

		return buf;
	}


	// To allow for more shaders in the future.
	static const int NUM_SHADERS = 2;

	// Create a GLSL program object from vertex and fragment shader files
	GLuint InitShader(const char* vShaderFile, const char* fShaderFile)
	{
		struct Shader {
			const char* filename;
			GLenum       type;
			GLchar* source;
		}  shaders[NUM_SHADERS] = { {vShaderFile, GL_VERTEX_SHADER, NULL},
						   {fShaderFile, GL_FRAGMENT_SHADER, NULL}
		};

		GLuint program = glCreateProgram();

		// For each shader do this stuff
		for (int i = 0; i < NUM_SHADERS; ++i) {
			Shader& s = shaders[i];
			s.source = readShaderSource(s.filename);
			if (shaders[i].source == NULL) {
				std::cerr << "Failed to read " << s.filename << std::endl;
				exit(EXIT_FAILURE);
			}

			GLuint shader = glCreateShader(s.type);
			glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
			glCompileShader(shader);

			GLint  compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
				std::cerr << s.filename << " failed to compile:" << std::endl;
				GLint  logSize;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
				char* logMsg = new char[logSize];
				glGetShaderInfoLog(shader, logSize, NULL, logMsg);
				std::cerr << logMsg << std::endl;
				delete[] logMsg;

				exit(EXIT_FAILURE);
			}

			delete[] s.source;

			glAttachShader(program, shader);
		}

		// link  and error check
		glLinkProgram(program);

		GLint  linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked) {
			std::cerr << "Shader program failed to link" << std::endl;
			GLint  logSize;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetProgramInfoLog(program, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete[] logMsg;

			exit(EXIT_FAILURE);
		}

		// use program object
		glUseProgram(program);
		return program;
	}

}  // Close namespace Angel block


// Adjust this value for your taste (to speed up, make bigger, to
// slow down rotation, make smaller
GLfloat incr =0.06;
const static int numOfObjects =50;
int axis = 1;
float theta[3] = {0.0, 0.0, 0.0};

GLuint buffers[2];
GLuint loc, loc2;
GLint matrix_loc;

bool rotate = true;
bool basecube = true;
bool testcube = true;
bool game = true;
bool controler = false;
bool drawHud = false;
//pointers for objects to draw
camera* cam;
cube* baseCube;
hud* info;

world* w1;
GLuint program;


// Model-view and projection matrices uniform location
GLuint  Modeltrans, Projection, Modelview, coloring, simple; 
mat4 model_view; //the transfermations per objects based off the position of the player
// OpenGL initialization

//bools for interaction
bool interact = false;

bool inroom1 = true;


//window vars
int Wheight =900;
int Wwidth = 900;


extern "C" void reshape(int width, int height){
	cam->reshape( width,  height);
	w1->setWindow(width, height);
}

extern "C" void motion(int xpos, int ypos)
{
	cam->motion(xpos, ypos);
	//w1->shawdowSelect(ypos, xpos);
	glutPostRedisplay();
}


void init() {

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// set up vertex buffer object

	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, baseCube->get_points_size()+ info->get_points_size(), NULL, GL_STATIC_DRAW);
	std::cout << baseCube->get_points_size() << std::endl;
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//loc2 = glGetAttribLocation(program, "vColor");
	//glEnableVertexAttribArray(loc2);
	//glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(baseCube->get_points_size()));

	Modelview = glGetUniformLocation(program, "model_view");
	Modeltrans = glGetUniformLocation(program, "model_trans");
  	Projection = glGetUniformLocation(program, "Projection");
	coloring = glGetUniformLocation(program, "objColor");
	simple = glGetUniformLocation(program, "simple");


	cam->setModelveiw(Modelview);
	cam->setProjection(Projection);

	//uncomment this for the wire frame model
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

extern "C" void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear the window
	if (!game){
		if (basecube) {
			baseCube->draw(true);
			
		}


	}else{
		//w1->updatePlayerpos(cameraPos);
		w1->draw();
		if (drawHud) {
			glUniform1i(simple, true);

			info->draw();
			//glDrawArrays(GL_TRIANGLE_STRIP, baseCube->get_points_size(), baseCube->get_points_size() + info->get_points_size());
			glUniform1i(simple, false);
		}
	}
	glutSwapBuffers();
}



extern "C" void mouse(int btn, int state, int xpos, int ypos) {
	if (game) {
		w1->proccessMouse(btn, state, xpos, ypos);
	}else{

		if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
		if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
		if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
	}
	glutPostRedisplay();
}


void idle() {

	static int delaytime = 2;
	static GLint time = glutGet(GLUT_ELAPSED_TIME);
	GLint deltatime = (glutGet(GLUT_ELAPSED_TIME) - time);

	if (rotate) {
		theta[axis] += incr * (deltatime);

		baseCube->updateAngle(theta);
	}
	if (theta[axis] > 360.0) theta[axis] -= 360.0;



	//std::cout << time << std::endl;
	// deal with movement
	
	if (controler) {
		controler = cam->processControllerInput();
		w1->setGridLines(cam->getGrids());

	}
	else {
		controler = cam->connectControllerConected();
		if (controler) {
			std::cout << "controller was connected" << std::endl;
		}
	}

	cam->moveCam();


	glutPostRedisplay();
}

extern "C" void mykey(unsigned char key, int mousex, int mousey) {
	//float cameraSpeed = 2.5f;
	switch (key)
	{
	case 27://hitting the escape key
		drawHud = !drawHud;

		break;
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'r':
		rotate = !rotate;
		break;

	case 'w':
		cam->Setmforward();
		//cameraPos += cameraSpeed * cameraFront;
		break;
	case 's':
		cam->Setmbackward();
		//cameraPos -= cameraSpeed * cameraFront;
		break;
	case 'a':
		cam->Setmleft();
		//cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case 'd':
		cam->Setmright();
		//cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case ' ':
		cam->Setmup();
		break;

	case 'e':
		cam->Setmdown();
		break;

	case 'f':
		interact = true;
		break;


	case '1':
		//basecube = !basecube;
		w1->toggleDebug();
		break;

	case '2':
		w1->setMultiselecct();
		break;

	case 'H':
	case 'h':
		w1->setDrawHidden(!w1->drawlinghidden());
		break;
	case 'G':
	case 'g':
		w1->setGridLines(!w1->drawlingGrids());
		break;
	case '<':
		w1->decreaseLayer();
		break;

	case '>':
		w1->increaseLayer();
		break;
	default:
		// glutSetWindowTitle(key);
		break;
	}

}

extern "C" void menustatus(int status, int x, int y) {
	axis = 2;
	glutPostRedisplay();
}

extern "C" void myMenu(int value){
	switch (value) {
	case 0:

		break;
	case 1:

		break;
	case 3:

		break;
	case 4:

		break;
	case 5:

		break;
	default:
		break;
	}
	glutPostRedisplay();
}

// Create menu items
void setupMenu() {
	glutCreateMenu(myMenu);
	glutAddMenuEntry("draw table", 0);
	glutAddMenuEntry("draw chair", 1);
	glutAddMenuEntry("draw small table", 3);
	glutAddMenuEntry("game", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void myinit(){
	std::cout << "screating objects" << std::endl;
	baseCube = new cube();
	baseCube->setindex(0);
	baseCube->setModelVeiw(Modeltrans);
	baseCube->setColorloc(coloring);
	baseCube->setColor(color4(1.0, 0.0, 0.0, 1.0));
	baseCube->init();

	info = new hud();
	info->init();
	info->setColorloc(coloring);
	
	w1 = new world();
	w1->setModelVeiw(Modeltrans);
	w1->setColorloc(coloring);
	w1->init();
	std::cout << "done creating objects" << std::endl;
}


int main(int argc, char** argv) {
	cam = new camera();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutCreateWindow("object viewer");
	//SetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(mykey);
	glutPassiveMotionFunc(motion);
	glutMotionFunc(NULL);
	//setupMenu();
	//glutMenuStatusFunc(menustatus);

	glewInit();
	glutWarpPointer(450, 450);
	init();
	myinit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return(EXIT_SUCCESS);
}
