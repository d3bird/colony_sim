#pragma once

#include "Angel.h"

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

//******************************************************************
//                                                                  
//  Class: chair
//                                                                  
//  Purpose:  holds the information to draw a chair at a given point 
//            
//  Functions:
//             draw()               //draws the room and its objects
//             update()             //updated the objects
//             misc update funtions // other functions to change other parts of the object
//******************************************************************

class cube {
public:

	cube();

	void init();
	void draw(bool i);
	void drawSelec(color4 i);
	void update();
	void colorcube();
	void MyQuad(int a, int b, int c, int d);
	void updateAngle(float i[3]) { theta[0] = i[0]; theta[1] = i[1]; theta[2] = i[2]; }

	GLuint get_quad_color_size() { return sizeof(quad_color); }
	GLuint get_points_size() { return sizeof(points); }
	GLuint get_loc_size() { return sizeof(loc); }

	void setModelVeiw(GLuint i) { trans = i; }
	void setColorloc(GLuint i) { coloring = i; }
	void setindex(int i) { placeIndex = i; }

	void calTranMat();
	void setLoc(vec3 i);

	int getHeight() { return  length; }

	void setColor(color4 i) { material_diffuse = i; colorcube(); changed = true; }


	bool isselected() { return selected; }
	void setselected(bool i) { selected = i; }

	

	//old code
	void updateVeiwer(vec4 i) { viewer = i; }
	void updateLightPos(point4 i) { light_position = i; }
	bool isvissible() { return vissible; }
	void setvissible(bool i) { vissible = i; }

private:

	bool vissible;

	vec4 product(vec4 a, vec4 b) {
		return vec4(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
	}
	//the arbitrary value that makes the block apear right next to each other
	double gridOfset = 0.27;
	//double gridOfset = 1;
	bool selected;
	const static int NumVertices = 36;// was 36
	point4  vertices[8];// was 8
	point4  vertices2[8];// was 8
	point4  vertices3[8];

	// colors for dynamic lighting
	color4 material_ambient;
	vec4 material_diffuse;
	color4 material_specular;
	float material_shininess;
	point4 light_position;
	color4 light_ambient;
	color4 light_diffuse;
	color4 light_specular;
	bool spec;

	//selected color
	color4 Scolor;

	//rotation angles for lighting
	float theta[3];


	color4 outline;
	color4 hidden;

	point4 points[NumVertices];

	//not functional for now
	color4 quad_color[NumVertices];
	vec4 viewer;
	int amountOfcubes = 3;

	//locations for the values in the shader
	GLuint  trans, coloring;

	//location of the cube in the wolrd
	vec3 loc;


	//the dimentions of the base cube
	int scale;
	int width;
	int length;
	int height;


	int placeIndex;

	//rotation transformation 
	mat4 ctm;

	//transformation for the location in the world 
	mat4 tloc;

	//matrix to passed to the shader
	mat4 model_veiw_base;

	//if the cube needs to recalulate a matrix
	bool changed;

};
