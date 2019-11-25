#pragma once
#include "Angel.h"
#include "cube.h"
#include <stdlib.h>
#include <random>

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

class tree{
public:
	tree();
	tree(GLuint t, GLuint c);
	~tree();
	void draw(bool i);
	void init();

	vec3 getLoc() { return vec3(x, y, z); }
	void setLoc(vec3 i) { x = i.x; y = i.y; z = i.z; }

	void setModelVeiw(GLuint i) { trans = i; }
	void setColorloc(GLuint i) { coloring = i; }

	int getCubenumber() { return ncubes; }

	void setTreeType(int i) { type = i; }
private:
	bool rinit;
	int x;
	int y;
	int z;
	int type;
	GLuint trans;
	GLuint coloring;

	int ncubes;

	//deffult
	color4 leaves;
	color4 bark;

	//leave varation colors
	color4 leaves1;
	color4 leaves2;
	color4 leaves3;
	color4 leaves4;

	cube* blocks;
};

