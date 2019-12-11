#pragma once

#include "Angel.h"

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

//******************************************************************
//                                                                  
//  Class: Pumpkin
//                                                                  
//  Purpose:  holds the information to draw a chair at a given point 
//            
//  Functions:
//             draw()               //draws the room and its objects
//             update()             //updated the objects
//             misc update funtions // other functions to change other parts of the object
//******************************************************************

class Pumpkin {
public:

	Pumpkin();

	void init();
	void draw(bool i);
	void drawSelec(color4 i);
	void update();
	void updateAngle(float i[3]) { theta[0] = i[0]; theta[1] = i[1]; theta[2] = i[2]; }

	GLuint get_loc_size() { return sizeof(loc); }

	void setModelVeiw(GLuint i) { trans = i; }
	void setColorloc(GLuint i) { coloring = i; }
	void setindex(int i) { placeIndex = i; }
	int getIndex() { return placeIndex; }

	void calTranMat();
	void setLoc(vec3 i);
	mat4 getModelVeiw() { return model_veiw_base; }


	int getHeight() { return  length; }

	void setColor(color4 i) { material_diffuse = i; currentc = i;  changed = true; }

	bool isselected() { return selected; }
	void setselected(bool i) { selected = i; }

	void setAir() { air = true; }
	bool isAir() { return air; }

	bool isvissible() { return vissible; }
	void setvissible(bool i) { vissible = i; }


	//functions used to finetune the models
	void raisStem();
	void LowerStem();

	void enlargeFruit();
	void decreaseFruit();

	void printMesurments();

	void harvest() { harvested = true; }
private:

	GLint compute_time();
	void reset_time();
	void set_last_time();

	void calcOutlinesmats();

	bool vissible;
	bool air;
	vec4 product(vec4 a, vec4 b) {
		return vec4(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
	}

	//the arbitrary value that makes the block apear right next to each other
	double gridOfset = 0.27;
	//double gridOfset = 1;
	bool selected;


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

	//colors
	color4 Scolor;
	color4 currentc;
	color4 stemColor;
	color4 fruitColor;
	color4 outline;
	color4 hidden;
	//rotation angles for lighting
	float theta[3];

	//locations for the values in the shader
	GLuint  trans, coloring;

	//location of the cube in the wolrd
	vec3 loc;
	//the dimentions of the base cube
	int scale;
	int width;
	int length;
	int height;

	int tilt;
	float fruit_scale;
	int placeIndex;

	//rotation transformation 
	mat4 ctm;

	//transformation for the location in the world 
	mat4 tloc;

	//matrix to passed to the shader
	mat4 model_veiw_base;
	mat4 stemBase;
	mat4 stemuppre;
	mat4 stem3;
	mat4 fruit;

	
	mat4 out_stemBase;
	mat4 out_stemuppre;
	mat4 out_fruit;

	//if the cube needs to recalulate a matrix
	bool changed;


	//information to deal with the size of the fruit
	bool hasFruit;

	//timing
	GLint last_time;
	GLint current_time;

	double updateSpeed;
	double updateSpeed_Scale;

	int	stage;

	int ytilt_current;
	int ytilt_Y;
	int ytilt_M;
	int ytilt_O;

	float fscale_current;
	float fscale_stage;
	float fscale_Y;
	float fscale_M;
	float fscale_O;

	bool harvested;
};
