#include <iostream>
#include "Angel.h"


using namespace std;
#pragma warning(disable : 4996)




//this is need to import the shader to the grapics card
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





// How many points are needed to specify the triangle
// Why 6?
const int NumPoints = 6;

// Data storage for our geometry for the triangles
vec3 points[NumPoints];

// This function initializes the particular things for this program
void myinit()
{
  // First points defined:
  points[0]=vec3(  0.0,   0.0, 0.0);
  points[1]=vec3(500.0,   0.0, 0.0);
  points[2]=vec3(  0.0, 500.0, 0.0);
  // Now colors defined:
  points[3]=vec3(1, 0, 0); // red
  points[4]=vec3(1, 1, 0); // yellow
  points[5]=vec3(0, 0, 1); // blue

  // attributes
  glClearColor(0.1, 0.1, 0.1, 0.5); // dark background */
  //glClearColor(0.8, 0.8, 0.8, 0.5); // light grey background
}

// This function initializes the buffers and shaders
void init()
{
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
    
  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshaderTriangle.glsl", "fshaderTriangle.glsl");
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  // Initialize the colors for the vertices for the fragment shader.
  GLuint col=  glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(col);

  // 3 points, each is 3 floating point coordinates after which is the
  // color data.
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(3*3*sizeof(float)));

  // Or you could use the following (since the color data is halfway through the vector):
  /*
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)/2));
  */
}  

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT);          // clear the window
  glDrawArrays(GL_TRIANGLES, 0, 3);      // draw the triangles
  glFlush();                             // clear buffers
}

int main(int argc, char** argv)
{
  // Standard GLUT initialization
  glutInit(&argc,argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA); // default, not needed
  glutInitWindowSize(600,600);                   // 600 x 600 pixel window
  glutInitWindowPosition(0,0);                   // place window top left on display
  glutCreateWindow("Triangle Display");         // window title

  // Add callbacks here, only required is displayFunc
  glutDisplayFunc(display);                      // display callback
						 // invoked when
						 // window opened 

  // Initialize the "magic" that glues all the code together.
  glewInit();

  myinit();             // set attributes
  init();               // set up shaders and display environment
  glutMainLoop();       // enter event loop

  return(EXIT_SUCCESS); // return successful exit code
}
