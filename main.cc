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
const static int numOfObjects =3;
int axis = 0;
float theta[3] = {0.0, 0.0, 0.0};

GLuint buffers[2];
GLuint loc, loc2;
GLint matrix_loc;

bool rotate = true;
bool basecube = true;
bool testcube = false;

bool game = false;
//pointers for objects to draw
camera* cam;
cube* baseCube;
cube* outline;
cube* outline2;

world* w1;
GLuint program;

int x =0;
int y =0;
int z =-1;
int moveamount = 1;
  point4 at(0.0, 0.0, 0.0, 1.0);
 // vec4   up(0.0, 1.0, 0.0, 0.0);
point4 eye(0, 0.0, -1, 1.0);
// the information for the camer pois
GLfloat camera_angle=45.0; // Camera's angle of view in degrees
GLfloat zNear;             // Camera's near clipping plane
GLfloat zFar;              // Camera's far clipping plane
GLfloat aspect;            // Window's aspect ratio (updated in reshape)
GLfloat left;
GLfloat right;
GLfloat top;
GLfloat bottom;
// Model-view and projection matrices uniform location
GLuint  Modeltrans, Projection, Modelview; 
mat4 model_view; //the transfermations per objects based off the position of the player
// OpenGL initialization



//bools for interaction
bool interact = false;

bool inroom1 = true;

int openwalls =0;




//Everything to do with the camera
vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);  
vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraDirection = normalize(cameraPos - cameraTarget);
vec3 up = vec3(0.0f, 1.0f, 0.0f); 
vec3 cameraRight = normalize(cross(up, cameraDirection));
vec3 cameraUp = cross(cameraDirection, cameraRight);
mat4 view = LookAt(vec3(0.0f, 0.0f, 3.0f),  vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
float cameraSpeed = 0.1f;

//window vars
int Wheight =900;
int Wwidth = 900;

//booleans for movments
bool mforward = false;
bool mleft = false;
bool mright = false;
bool mbackward = false;
bool mup = false;
bool mdown = false;

//vars for the first person 
//float lastX = 450, lastY = 450;
bool firstMouse = true;
float yaw   =  -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  450.0f;
float lastY =  450;
float fov   =  45.0f;

extern "C" void reshape(int width, int height){
  glViewport(0, 0, width, height);

	Wheight = width;
	Wheight = height;

  GLfloat left = -2.0, right = 2.0;
  GLfloat top = 2.0, bottom = -2.0;
 
  // Use following for ortho projection
  //  GLfloat zNear = -20.0, zFar = 20.0;

  // Use following for perspective projection
  zNear = 0.2;
  zFar = 40.0;

  aspect = GLfloat(width)/height;

  if (aspect > 1.0) {
    left *= aspect;
    right *= aspect;
  }
  else {
    top /= aspect;
    bottom /= aspect;
  }

	mat4 projection = Perspective(camera_angle, aspect, zNear, zFar);

  // Can use either perspective or ortho projection.
   // mat4 projection = Ortho(left, right, bottom, top, zNear, zFar);
  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}


void moveCam() {
	if (mforward) {
		cameraPos += cameraSpeed * cameraFront;
		mforward = false;
	}
	if (mbackward) {
		cameraPos -= cameraSpeed * cameraFront;
		mbackward = false;
	}
	if (mleft) {
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		mleft = false;
	}
	if (mright) {
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		mright = false;
	}
	if (mup) {
		cameraPos.y += cameraSpeed;
		mup = false;
	}
	if (mdown) {
		cameraPos.y -= cameraSpeed;
		mdown = false;
	}


	mat4 view = LookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(Modelview, 1, GL_TRUE, (view));
}

void Setmforward() {
	mforward = true;
}
void Setmleft() {
	mleft = true;
}
void Setmright() {
	mright = true;
}
void Setmbackward() {
	mbackward = true;
}
void Setmup() {
	mup = true;
}
void Setmdown() {
	mdown = true;
}


extern "C" void motion(int xpos, int ypos)
{
 
	//std::cout<<"mopuse is moving:"<<xpos<<" "<<ypos<<std::endl;
	//std::cout<<"check"<<std::endl;
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    vec3 front;
    front.x = cos(DegreesToRadians*yaw) * cos(DegreesToRadians*pitch);
    front.y = sin(DegreesToRadians*pitch);
    front.z = sin(DegreesToRadians*yaw) * cos(DegreesToRadians*pitch);
    cameraFront = normalize(front);
		//glutWarpPointer(450,450); 

	    if ( xpos < 50 || xpos > Wwidth - 50 ) {  //you can use values other than 100 for the screen edges if you like, kind of seems to depend on your mouse sensitivity for what ends up working best
        lastX = Wwidth/2;   //centers the last known position, this way there isn't an odd jump with your cam as it resets
        lastY = Wheight/2;   
        glutWarpPointer(Wwidth/2, Wheight/2);  //centers the cursor
    } else if (ypos < 50 || ypos > Wheight - 50) {
        lastX = Wwidth/2;
        lastY = Wheight/2;
        glutWarpPointer(Wwidth/2, Wheight/2);
    } 

  glutPostRedisplay();
}

//the rest of the program


void init() {

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// set up vertex buffer object

	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, (baseCube->get_points_size() + baseCube->get_quad_color_size()) * numOfObjects, NULL, GL_STATIC_DRAW);
	std::cout << baseCube->get_points_size() << std::endl;
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	loc2 = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(baseCube->get_points_size()));

	Modelview = glGetUniformLocation(program, "model_view");
	Modeltrans = glGetUniformLocation(program, "model_trans");
  	Projection = glGetUniformLocation(program, "Projection");

	//uncomment this for the wire frame model
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

extern "C" void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear the window
	if (!game){
		if (basecube) {
			baseCube->draw();
			
		}
		if (testcube) {
			outline->draw();
			outline2->draw();
		}

	}else{
		w1->updatePlayerpos(cameraPos);
		w1->draw();
	}
	glutSwapBuffers();
}

//mouse vars


extern "C" void mouse(int btn, int state, int xpos, int ypos) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void idle() {

	static GLint time = glutGet(GLUT_ELAPSED_TIME);
	GLint deltatime = (glutGet(GLUT_ELAPSED_TIME) - time);


	theta[axis] += incr * (deltatime);

	baseCube->updateAngle(theta);
	outline->updateAngle(theta);
	outline2->updateAngle(theta);

	if (theta[axis] > 360.0) theta[axis] -= 360.0;



	time = glutGet(GLUT_ELAPSED_TIME);



	  // deal with movement

	moveCam();

	// check for collision


	if (interact) {



	}



	glutPostRedisplay();
}

extern "C" void mykey(unsigned char key, int mousex, int mousey) {
	//float cameraSpeed = 2.5f;
	switch (key)
	{
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'r':
		rotate = !rotate;
		break;

	case 'w':
		Setmforward();
		//cameraPos += cameraSpeed * cameraFront;
		break;
	case 's':
		Setmbackward();
		//cameraPos -= cameraSpeed * cameraFront;
		break;
	case 'a':
		Setmleft();
		//cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case 'd':
		Setmright();
		//cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case ' ':
		Setmup();
		break;

	case 'e':
		Setmdown();
		break;

	case 'f':
		interact = true;
		break;

	case '1':
		basecube = !basecube;
		break;

	case '2':
		testcube = !testcube;
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
	baseCube = new cube();
	baseCube->setindex(0);
	baseCube->setModelVeiw(Modeltrans);
	baseCube->init();

	outline = new cube();
	outline->setModelVeiw(Modeltrans);
	outline->setindex(1);
	outline->setColor(color4(1.0, 0.0, 0.0, 1.0));
	outline->init();
	outline->setLoc(vec3(0, 1, 0));

	outline2 = new cube();
	outline2->setModelVeiw(Modeltrans);
	outline2->setindex(2);
	outline2->setColor(color4(0.0, 0.0, 1.0, 1.0));
	outline2->init();
	outline2->setLoc(vec3(1, 1, 0));

	w1 = new world();
}


int main(int argc, char** argv){

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
	 glutPassiveMotionFunc (motion);
	setupMenu();
	glutMenuStatusFunc(menustatus);

	glewInit();
	glutWarpPointer(450,450); 
	init();
	myinit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return(EXIT_SUCCESS);
}
