#include "world.h"


world::world() {

	height = 2;
	ywidth = 10;
	xwidth = 10;


	map = new cube ** [height];

	for (int f = 0; f < height; f++) {

		map[f] = new cube * [ywidth]; // generating the rows

		for (int i = 0; i < ywidth; i++) {// generating the collums
			map[f][i] = new cube[xwidth];
		}

	}

}




world::~world(){
	for (int f = 0; f < height; f++) {
		for (int y = 0; y < ywidth; y++) {
			delete[] map[f][y];
		}
		delete[] map[f];
	}
	delete[] map;
}

void world::init() {

	for (int f = 0; f < height; f++) {
		for (int y = 0; y < ywidth; y++) {
			for (int x = 0; x < xwidth; x++) {
				//map[f][y][x]

				map[f][y][x].setModelVeiw(trans);
				map[f][y][x].setColorloc(coloring);
				if (f != 0) {
					map[f][y][x].setColor(color4(1.0, 0.0, 0.0, 1.0));
				}
				else {
					map[f][y][x].setColor(color4(0.0, 1.0, 0.0, 1.0));
				}
				map[f][y][x].setLoc(vec3(x, -f, y));
			}
		}
	}

}


void world::draw(){

	for (int f = 0; f < height; f++) {
		for (int y = 0; y < ywidth; y++) {
			for (int x = 0; x < xwidth; x++) {
				map[f][y][x].draw();

			}
		}
	}

}

void world::update(){

}



