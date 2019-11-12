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
	void draw();

	void update();
	void colorcube();
	void MyQuad(int a, int b, int c, int d);
	void updateAngle(float i[3]) { theta[0] = i[0]; theta[1] = i[1]; theta[2] = i[2]; }

	GLuint get_quad_color_size() { return sizeof(quad_color); }
	GLuint get_points_size() { return sizeof(points); }
	GLuint get_loc_size() { return sizeof(loc); }

	void setModelVeiw(GLuint i) { trans = i; }

	void changeScale(int i);
	void increaseScale();
	void decreaseScale();

	void calTranMat();
	void setLoc(vec3 i);

	int getHeight(){return  length;}


	//old code
	void updateVeiwer(vec4 i){ viewer =i;}
	void updateLightPos(point4 i){light_position = i;}
	bool isspecial(){return special;}
	void setspecial(bool i){special =i;}

	void setColor(color4 i){material_diffuse =i;}
private:

	vec4 product(vec4 a, vec4 b) {
		return vec4(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
	}

	bool special;
	const static int NumVertices = 36;// was 36
	point4  vertices[8];// was 8
	point4  vertices2[8];// was 8
	point4  vertices3[8];
	color4 material_ambient;
	color4 material_diffuse;
	color4 material_specular;
	float material_shininess;
	point4 light_position;
	color4 light_ambient;
	color4 light_diffuse;
	color4 light_specular;

	float theta[3];

	point4 points[NumVertices];
	color4 quad_color[NumVertices];


	GLuint  trans;

	vec3 loc;

	vec4 viewer;

	int scale;
	int width;//315
	int length;//
	int height;//



	//rotation transformation 
	mat4 ctm;

	//transformation for the location in the world 
	mat4 tloc;

	mat4 model_veiw_base;

	// NOTE: SPECULAR LIGHTING to start!!
	bool spec;
};
