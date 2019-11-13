#pragma once
#include "Angel.h"

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

class camera {
public:

	camera();

	void moveCam();

	//the functions that are called when the mouse moves and window size changes
	void reshape(int width, int height);
	void motion(int xpos, int ypos);

	//set the locations for the shader vars
	void setProjection(GLuint i) { Projection = i; }
	void setModelveiw(GLuint i) { Modelview = i; }

	//functions for the movement of the cam
	void Setmforward() { mforward = true; }
	void Setmleft() { mleft = true; }
	void Setmright() { mright = true; }
	void Setmbackward() { mbackward = true; }
	void Setmup() { mup = true; }
	void Setmdown() { mdown = true; }

private:

	//booleans for movments
	bool mforward;
	bool mleft;
	bool mright;
	bool mbackward;
	bool mup;
	bool mdown;

	//Everything to do with the camera
	vec3 cameraPos;
	vec3 cameraTarget;
	vec3 cameraDirection;
	vec3 up;
	vec3 cameraRight;
	vec3 cameraUp;
	mat4 view;
	vec3 cameraFront;
	float cameraSpeed;

	//window vars
	int Wheight;
	int Wwidth;


	//vars for the first person 
	bool firstMouse = true;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;

	int x;
	int y;
	int z;
	int moveamount;

	// the information for the camera pos
	GLfloat camera_angle; // Camera's angle of view in degrees
	GLfloat zNear;             // Camera's near clipping plane
	GLfloat zFar;              // Camera's far clipping plane
	GLfloat aspect;            // Window's aspect ratio (updated in reshape)

	//other vals needed for ortho perspective
	GLfloat left;
	GLfloat right;
	GLfloat top;
	GLfloat bottom;

	GLuint Projection;
	GLuint Modelview;
};

