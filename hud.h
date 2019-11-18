#pragma once
#include "Angel.h"

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;


class hud {
public:

	hud();

	void draw();
	void update();
	void init();
	void setindex(int i) { index = i; }
	void setColorloc(GLuint i) { coloring = i; }
	GLuint get_points_size() { return sizeof(points); }

	

private:
	const static int NumVertices = 36;
	int index;
	point4 points[NumVertices];
	color4 background;
	GLuint coloring;

	

	int width;//315
	int length;//
	int height;//
};

