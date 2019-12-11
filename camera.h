#pragma once
#pragma comment(lib, "XInput.lib")
#include "Angel.h"
#include <windows.h>
#include <Xinput.h>
typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

class camera {
public:

	camera();

	void moveCam();

	bool connectControllerConected();

	bool processControllerInput();

	//the functions that are called when the mouse moves and window size changes
	void reshape(int width, int height);
	void motion(int xpos, int ypos);

	//set the locations for the shader vars
	void setProjection(GLuint i) { Projection = i; }
	void setModelveiw(GLuint i) { Modelview = i; }
	void setVAO1(GLuint i) { vao1 = i; }//not used yet

	//functions for the movement of the cam
	void Setmforward() { mforward = true; }
	void Setmleft() { mleft = true; }
	void Setmright() { mright = true; }
	void Setmbackward() { mbackward = true; }
	void Setmup() { mup = true; }
	void Setmdown() { mdown = true; }

	//getters for the controller
	bool getGrids() { return grids; }
	bool getdownlevel();
	bool getuplevel();
	bool getselect();
	bool getRselect();
	//last known mouse loc
	int getLastX() { return lastX; }
	int getLastY() { return lastY; }


	//misc
	mat4 getProjection() { return  projection; }
	void restCursor();
	void setWorldgenShaderProj(GLuint i) { Projection2 = i; }
	void setWorldgenShaderModel(GLuint i) { Modelview2 = i; }
	void setVAO2(GLuint i) { vao2 = i; }//not used yet

private:

	//misc
	bool grids;
	bool downleval;
	bool uplevel;
	bool select;
	bool rightselect;

	//booleans for movments
	bool mforward;
	bool mleft;
	bool mright;
	bool mbackward;
	bool mup;
	bool mdown;


	//buttons clicks for the controller
	bool xdown;
	bool leftbumber;
	bool rightbumbr;
	bool righttrigger;
	bool lefttrigger;


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

	//for the main shader
	GLuint Projection;
	GLuint Modelview;
	GLuint vao1;
	mat4 projection;
	//for the worldgen shader
	GLuint Projection2;
	GLuint Modelview2;
	GLuint vao2;

	bool invert_con_look;

	int controlerSensitivityL;
};

