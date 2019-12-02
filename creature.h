#pragma once
#include "Angel.h"
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <queue>

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

class creature{
public:

	creature();

	void draw();
	void draw(bool i);
	void update();

	void init();

	void setModelVeiw(GLuint i) { trans = i; }
	void setColorloc(GLuint i) { coloring = i; }
	void setindex(int i) { placeIndex = i; }

	void calTranMat();
	void setLoc(vec3 i);

	void setGoal(vec3 i);

	void addLocToQue(vec3 i);

	void colorcube();
	void MyQuad(int a, int b, int c, int d);

	void updateAngle(float i[3]) { theta[0] = i[0]; theta[1] = i[1]; theta[2] = i[2]; }


	// Update the last updated time to now.
	void set_last_time();

	// Reset the simulation time for this object to 0.
	void reset_time();

	// Returns the time since the last update
	GLint compute_time();

private:

	void pathFiding();

	//const values
	double gridOfset = 0.27;
	const static int NumVertices = 36;

	double movespeed;

	color4 defColor;

	//locations for the values in the shader
	GLuint  trans, coloring;

	int placeIndex;

	//rotation angles for lighting
	float theta[3];

	int height;
	int width;
	int length;

	point4  vertices[8];
	point4 points[NumVertices];

	vec3 loc;// current location

	vec3 goal;

	//rotation transformation 
	mat4 ctm;

	//transformation for the location in the world 
	mat4 tloc;

	//matrix to passed to the shader
	mat4 model_veiw_base;

	//if the cube needs to recalulate a matrix
	bool changed;

	bool selected;

	//timing
	GLint last_time;
	GLint current_time;

	//everything to do with path finding
	bool newGoal;
	double destOffset;
	std::queue<vec3> travelingPoints;

};

