#pragma once
#include "Angel.h"
#include "cube.h"

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

private:
	bool rinit;
	int x;
	int y;
	int z;

	GLuint trans;
	GLuint coloring;

	int ncubes;

	color4 leaves;
	color4 bark;

	cube* blocks;
};

