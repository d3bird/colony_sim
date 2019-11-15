#include "tree.h"

tree::tree(GLuint t, GLuint c) {
	ncubes = 13;
	blocks = new cube[ncubes];
	leaves = color4(0.0, 1.0, 0.0, 1.0);
	bark = color4(0.0, 0.0, 1.0, 1.0);
	x = 0;
	y = 0;
	z = 0;
	rinit = false;
	coloring = c;
	trans = t;
}

tree::tree() {
	ncubes = 13;
	blocks = new cube[ncubes];
	leaves = color4(0.0, 1.0, 0.0, 1.0);
	bark = color4(0.0, 0.0, 1.0, 1.0);
	x = 0;
	y = 0;
	z = 0;
	rinit = false;
}

tree::~tree() {

	delete[] blocks;
}

void tree::draw(bool i) {
	for (int q = 0; q < ncubes; q++) {
		blocks[q].draw(i);
	}

}

void tree::init() {
	if (!rinit) {

		rinit = true;
		for (int i = 0; i < ncubes; i++) {//only for the first five
			blocks[i].setModelVeiw(trans);
			blocks[i].setColorloc(coloring);
			blocks[i].setindex(1);
			blocks[i].setvissible(true);
			if (i >= 4) {
				blocks[i].setColor(color4(0.3216, 0.4196, 0.1765, 1.0));
			}
			else {
				blocks[i].setColor(color4(0.549, 0.3843, 0.3451, 1.0));
			}
			blocks[i].setLoc(vec3(x, y + 1 + i, z));
			blocks[i].init();
		}

		blocks[5].setLoc(vec3(x+1, y + 3, z));
		blocks[6].setLoc(vec3(x-1, y + 3, z));
		blocks[7].setLoc(vec3(x, y + 3, z+1));
		blocks[8].setLoc(vec3(x, y + 3, z-1));
		blocks[9].setLoc(vec3(x+1, y + 4, z));
		blocks[10].setLoc(vec3(x-1, y + 4, z));
		blocks[11].setLoc(vec3(x, y + 4, z+1));
		blocks[12].setLoc(vec3(x, y + 4, z-1));
		//blocks[13].setLoc(vec3(x, y + 4, z - 1));

		//for debugging purposes only to see where the leaves are forming
		/*blocks[5].setColor(color4(0.0, 1.0, 0.0, 1.0));
		blocks[6].setColor(color4(0.0, 1.0, 0.0, 1.0));
		blocks[7].setColor(color4(1.0, 0.0, 0.0, 1.0));
		blocks[8].setColor(color4(1.0, 0.0, 0.0, 1.0));
		blocks[9].setColor(color4(0.0, 0.0, 1.0, 1.0));
		blocks[10].setColor(color4(0.0, 0.0, 1.0, 1.0));
		blocks[11].setColor(color4(1.0, 1.0, 0.0, 1.0));
		blocks[12].setColor(color4(1.0, 1.0, 0.0, 1.0));*/
		//blocks[13].setColor(color4(0.0, 1.0, 0.0, 1.0));
	}

}


