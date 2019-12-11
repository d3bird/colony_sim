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
#include "InitShader.h"
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
GLuint program, program2;
GLuint vao;

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
	info->setProjection(cam->getProjection());
}

extern "C" void motion(int xpos, int ypos)
{
	cam->motion(xpos, ypos);
	//w1->shawdowSelect(ypos, xpos);
	glutPostRedisplay();
}


void init() {

	// Create a vertex array object

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// set up vertex buffer object

	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, (baseCube->get_points_size())+ info->get_points_size()+ baseCube->get_points_size()+ baseCube->get_points_size() + baseCube->get_points_size(), NULL, GL_STATIC_DRAW);
	std::cout << baseCube->get_points_size() << std::endl;
	program = InitShader("vshader.glsl", "fshader.glsl");
	//program2 = InitShader("vTextshader.glsl", "fTextshader.glsl");
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

		//	glUseProgram(program);
		//	glBindVertexArray(vao);
		w1->draw();
		//baseCube->draw(true);

		if (drawHud) {
			glUniform1i(simple, true);
		//	glEnable(GL_CULL_FACE);
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//info->RenderText("This is sample text", 25.0f, 25.0f, 1.0f, vec3(0.5, 0.8f, 0.2f));
			info->draw();
			//glUseProgram(program);
			//glBindVertexArray(vao);
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
	w1->update();
	cam->moveCam();


	glutPostRedisplay();
}

extern "C" void mykey(unsigned char key, int mousex, int mousey) {
	//float cameraSpeed = 2.5f;
	switch (key)
	{
	case 27://hitting the escape key
		drawHud = !drawHud;
		//std::cout << drawHud << std::endl;
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
		//tesplant->harvest();
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
	case '3':
		w1->toggleDestination();
		break;
	case '4':
		w1->toggleMining();
		break;
	case '5':
		w1->toggleHarvesting();
		break;
	case '6':

		break;
	case '7':

		break;
	case '8':

		break;
	case '9':

		break;
	case '0':

		break;

	case 'i':

		break;
	case 'k':

		break;

	case 'o':
		//tesplant->enlargeFruit();
		break;
	case 'l':
		//tesplant->decreaseFruit();
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
	std::cout << "creating objects" << std::endl;
	baseCube = new cube();
	baseCube->setindex(0);
	baseCube->setModelVeiw(Modeltrans);
	baseCube->setColorloc(coloring);
	baseCube->setColor(color4(1.0, 0.0, 0.0, 1.0));
	baseCube->init();

	info = new hud();
	info->init();
	info->setColorloc(coloring);
	
	glBindVertexArray(vao);
	glUseProgram(program);

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
	glutCreateWindow("0.7");
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
	std::cout << "openGL version " << glGetString(GL_VERSION) << std::endl;
	myinit();
	glEnable(GL_DEPTH_TEST);
	std::cout << "openGL version " <<glGetString(GL_VERSION) << std::endl;
	std::cout <<"glut version "<< glutGet(GLUT_VERSION) << std::endl;
	glutMainLoop();
	return(EXIT_SUCCESS);
}
