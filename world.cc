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
	trees = new tree();
	treeList = new std::vector<tree*>[height];


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
					map[f][y][x].setColor(color4(0.522, 0.353, 0.251, 1.0));
				}
				else {
					//map[f][y][x].setColor(color4(0.486, 0.988, 0.0, 1.0));// grass green
					map[f][y][x].setColor(color4(0.196, 0.804, 0.196, 1.0));//dark lime green
				}
				map[f][y][x].setLoc(vec3(x, -f, y));

				if ((x == xwidth - 1 || x == 0) || (y == ywidth - 1 || y == 0) || (f == height - 1 || f == 0)) {
					map[f][y][x].setvissible(true);
				}
			}
		}
	}

	//examples for rendering multiple trees at any location
	vec3 loc = vec3(5, 0, 1);
	trees->setColorloc(coloring);
	trees->setModelVeiw(trans);
	trees->setLoc(loc);
	trees->init();
	int temp = loc.y;
	if (temp < 0) {
		temp = temp * -1;
	}
	treeList[temp].push_back(trees);


	trees = new tree();
	loc = vec3(15, 0, 8);
	trees->setColorloc(coloring);
	trees->setModelVeiw(trans);
	trees->setLoc(loc);
	trees->init();
	 temp = loc.y;
	 if (temp < 0) {
		 temp = temp * -1;
	 }
	treeList[temp].push_back(trees);

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
		if (treeList[f].size() != 0 && f == startLayer) {
			for (int i = 0; i < treeList[f].size(); i++) {
				treeList[f][i]->draw(gridlines);
			}
		}
	}



	//trees->draw(gridlines);
}

void world::update(){

}



