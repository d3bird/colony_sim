#pragma once
#include "Angel.h"

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

class camera {
public:

	camera();

	void moveCam();
	void reshape(int width, int height);
	void motion(int xpos, int ypos);

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
	bool mforward = false;
	bool mleft = false;
	bool mright = false;
	bool mbackward = false;
	bool mup = false;
	bool mdown = false;

	//Everything to do with the camera
	vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraDirection = normalize(cameraPos - cameraTarget);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraRight = normalize(cross(up, cameraDirection));
	vec3 cameraUp = cross(cameraDirection, cameraRight);
	mat4 view = LookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
	float cameraSpeed = 0.1f;

	//window vars
	int Wheight = 900;
	int Wwidth = 900;


	//vars for the first person 
//float lastX = 450, lastY = 450;
	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 450.0f;
	float lastY = 450;
	float fov = 45.0f;

	int x = 0;
	int y = 0;
	int z = -1;
	int moveamount = 1;
//	point4 at(0.0, 0.0, 0.0, 1.0);
	// vec4   up(0.0, 1.0, 0.0, 0.0);
	//point4 eye(0, 0.0, -1, 1.0);
	// the information for the camer pois
	GLfloat camera_angle = 45.0; // Camera's angle of view in degrees
	GLfloat zNear;             // Camera's near clipping plane
	GLfloat zFar;              // Camera's far clipping plane
	GLfloat aspect;            // Window's aspect ratio (updated in reshape)
	GLfloat left;
	GLfloat right;
	GLfloat top;
	GLfloat bottom;

	GLuint Projection;
	GLuint Modelview;
};

