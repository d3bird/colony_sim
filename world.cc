#include "world.h"


world::world() {

	height = 14;
	ywidth = 20;
	xwidth = 20;


	 drawdistance = 5;
	 startLayer = 0;
	 gridlines = false;
	 drawhidden = false;

	map = new cube ** [height];

	for (int f = 0; f < height; f++) {

		map[f] = new cube * [ywidth]; // generating the rows

		for (int i = 0; i < ywidth; i++) {// generating the collums
			map[f][i] = new cube[xwidth];
		}

	}
	trees = new tree;
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

				if ((x == xwidth - 1 || x == 0) || (y == ywidth - 1 || y == 0) || (f == height - 1 || f == 0)) {
					map[f][y][x].setvissible(true);
				}
			}
		}
	}
	trees->setColorloc(coloring);
	trees->setModelVeiw(trans);
	trees->init();
}

void world::increaseLayer() {
	startLayer++;
	if (startLayer < 0) {
		startLayer = 0;
	}
}

void world::decreaseLayer() {
	startLayer--;
	if (startLayer < 0) {
		startLayer = 0;
	}
}

void world::draw(){
	//std::cout << "drawling world" << std::endl;

	int drawlength = startLayer + drawdistance;
	if (drawlength > height) {
		drawlength = height;
	}
	for (int f = startLayer; f < drawlength; f++) {
		for (int y = 0; y < ywidth; y++) {
			for (int x = 0; x < xwidth; x++) {
				if (map[f][y][x].isvissible()|| (drawhidden && f == startLayer)) {
					map[f][y][x].draw(gridlines);
				}
			}
		}
	}
	trees->draw(true);
}

void world::update(){

}



