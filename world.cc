#include "world.h"


world::world() {

	height = 14;
	ywidth = 20;
	xwidth = 20;


	drawdistance = 5;
	startLayer = 0;
	gridlines = false;
	drawhidden = false;

	map = new cube * *[height];

	//creating the map of the world
	for (int f = 0; f < height; f++) {

		map[f] = new cube * [ywidth]; // generating the rows

		for (int i = 0; i < ywidth; i++) {// generating the collums
			map[f][i] = new cube[xwidth];
		}
	}

	//creating the selection colors

	selection = new  color4 * [ywidth];

	for (int i = 0; i < ywidth; i++) {
		selection[i] = new color4[xwidth];
	}

	for (int yi = 0; yi < ywidth; yi++) {
		for (int xi = 0; xi < xwidth; xi++) {
			selection[yi][xi].x = xi / xwidth;
			selection[yi][xi].y = yi / ywidth;
			//selection[yi][xi].x = (xi / xwidth) + (yi / ywidth);
			//selection[yi][xi].y = 0;
			selection[yi][xi].z = 0;
			//selection[yi][xi].z = (xi / xwidth) + (yi / ywidth);
			selection[yi][xi].w = 1;
			//selection[yi][xi] = color4(xi / 255, yi / 255, 0.0, 1.0);
			//std::cout << temp * 255 + 0.5 << " " << selection[yi][xi].y * 255 + 0.5 << " " << selection[yi][xi].z * 255 + 0.5 << std::endl;
		}
	}

	//creating the trees
	trees = new tree();
	treeList = new std::vector<tree*>[height];

	win_h = 900;
	win_w = 900;
	debug = false;
	multiSelcting = false;
	firstPoint = false;
	point2init = false;
}


world::~world(){
	for (int f = 0; f < height; f++) {
		for (int y = 0; y < ywidth; y++) {
			delete[] map[f][y];
			
		}
		delete[] map[f];
	}
	delete[] map;

	for (int y = 0; y < ywidth; y++) {
		delete[] selection[y];
	}
	delete[] selection;
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
	trees->setTreeType(0);
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
	trees->setTreeType(1);
	trees->init();
	 temp = loc.y;
	 if (temp < 0) {
		 temp = temp * -1;
	 }
	treeList[temp].push_back(trees);

	colonist*beast;
	beast = new colonist();
	loc = vec3(1, 1, 12);
	beast->setColorloc(coloring);
	beast->setModelVeiw(trans);
	beast->setLoc(loc);
	beast->init();
	//beast->Cinit();
	colonists.push_back(beast);

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
				if ((map[f][y][x].isvissible() || (drawhidden && f == startLayer))&& !map[f][y][x].isAir()) {
					if (!debug) {
						map[f][y][x].draw(gridlines);
					}
					else {
						//draws the back buffer instead of the normal colors
						//needed to debug selection
						color4 temp = selection[y][x];
						map[f][y][x].drawSelec(temp);
					}
					
				}
				else if (map[f][y][x].isAir() && f+1 < height) {
					map[f+1][y][x].draw(gridlines);
				}
			}
		}
		if (treeList[f].size() != 0 && f == startLayer) {
			for (int i = 0; i < treeList[f].size(); i++) {
				treeList[f][i]->draw(gridlines);
			}
		}
	}
	//draw each of the creatures
	for (int i = 0; i < colonists.size(); i++) {
		if (colonists[i]->getLevel() == startLayer) {
			colonists[i]->draw();
		}
	}

	//trees->draw(gridlines);
}

void world::update(){
	for (int i = 0; i < colonists.size(); i++) {
		//std::cout << "asd" << std::endl;
		colonists[i]->update();
	}
}

void world::proccessMouse(int btn, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		// Draw the scene with identifying colors
		// Ensure the clear color isn't the same as any of your objects
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Setting first parameter to true means to use the selection
		// colors, not the object colors

		for (int y = 0; y < ywidth; y++) {
			for (int x = 0; x < xwidth; x++) {
				//draws the back buffer instead of the normal colors
				//needed to debug selection
				color4 temp = selection[y][x];
				map[startLayer][y][x].drawSelec(temp);

			}
		}


		// Flush ensures all commands have drawn
		glFlush();

		// Read the value at the location of the cursor
		// In the back buffer (not the one visible on-screen)
		glReadBuffer(GL_BACK);
		unsigned char PixelColor[3];
		glReadPixels(x, win_h - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &PixelColor);
		//std::cout << int(PixelColor[0]) << " "<< int(PixelColor[1]) << " "	<< int(PixelColor[2]) << std::endl;
		processSelection(PixelColor, btn);

		/*
		glutSwapBuffers();
		std::cout << "Type any character to continue: ";
		char a;
		std::cin >> a;
		*/

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

	}

}

void world::processSelection(unsigned char PixelColor[], int btn) {

	//float r = PixelColor.x / 256;
	if (PixelColor[1] == 255) {
		std::cout << "not on the map" << std::endl;
		return;
	}
	else {
		int ox =-1;
		int oy = -1;
		vec3 colorb = vec3(selection[0][0].x, selection[0][0].y, selection[0][0].z);
		
		//finds the x matching color then breaks out
		for (int xi = 0; xi < xwidth; xi++) {
			colorb.x = selection[0][xi].x;
			if ((PixelColor[0] == int(colorb.x * 255 - 0.5)) || (PixelColor[0] == int(colorb.x * 255 + 0.5))) {
				ox = xi;
				break;
			}
		}
		//finds the y matiching color then breaks out
		for (int yi = 0; yi < ywidth; yi++) {
			colorb.y = selection[yi][0].y;
			if ((PixelColor[1] == int(colorb.y * 255 - 0.5)) || (PixelColor[1] == int(colorb.y * 255 + 0.5))) {
				oy = yi;
				break;
			}
		}
		
		//test to make sure that the color was found
		if (ox == -1 || oy == -1) {
		std::cout << "could not find value" << std::endl;
		std::cout << (int)PixelColor[0] << " " << (int)PixelColor[1] << " " << (int)PixelColor[2] << std::endl;
		//std::cout<<selection[yi][xi].x<<" "<< selection[yi][xi].y<< " "<< selection[yi][xi].z << std::endl;
		return;
		}

		
		//set destination for action
		if (direction) {
			map[startLayer][oy][ox].setselected(true);
			colonists[0]->addLocToQue(vec3(ox, startLayer, oy));
		}
		else if (mining) {
			//map[startLayer][oy][ox].setselected(true);
			map[startLayer][oy][ox].setAir();
			reveale(startLayer, oy, ox);
			colonists[0]->setTask(1,vec3(ox, startLayer, oy));
		}else
		//preform the action of the mouse click 
		if (multiSelcting) {
			//std::cout << "multisection is enabled" << std::endl;
			multiselect(oy, ox);
		}
		else {
			map[startLayer][oy][ox].setselected(true);
		}

	}
}


void world::multiselect(int yi, int xi) {

	if (!firstPoint) {
		pointOne = vec3(startLayer, yi, xi);
		pointTwo = pointOne;
		firstPoint = true;
		map[startLayer][yi][xi].setselected(true);
	}
	else {
		map[startLayer][yi][xi].setselected(true);
		//map[startLayer][(int)pointOne.y][(int)pointOne.z].setselected(true);

		int diffy = pointOne.y - yi;
		int diffx = pointOne.z - xi;

		pointTwo.y = yi;
		pointTwo.z = xi;

		int wx = pointOne.z;
		int wy = pointOne.y;
		bool running = true;

		//std::cout << "y diff = " << diffy << std::endl;
		//std::cout << "x diff = " << diffx << std::endl;

		//connects the two points with a path
		if (diffy == 0 || diffx == 0) {
			while (running) {
				if (diffy == 0 && diffx == 0) {
					running = false;
				}
				else if (diffy != 0) {
					if (diffy > 0) {//if it is possitive
						wy--;
						diffy--;
					}
					else {// if it is negative
						wy++;
						diffy++;
					}
					map[startLayer][wy][wx].setselected(true);
				}
				else if (diffx != 0) {
					if (diffx > 0) {//if it is possitive
						wx--;
						diffx--;
					}
					else {// if it is negative
						wx++;
						diffx++;
					}
					map[startLayer][wy][wx].setselected(true);
				}

				//running = false;
			}
		}
		else {
			//make sure the the differences are positive
			int t1 = diffy;
			int t2 = diffx;

			bool t1Neg = false;
			bool t2Neg = false;
			if (t1 < 0) {
				t1 *= -1;
				t1Neg = true;
			}
			if (t2 < 0) {
				t2 *= -1;
				t2Neg = true;
			}
			for (int q = 0; q < t1+1; q++) {
				for (int a = 0; a < t2+1; a++) {

					if (t1Neg && t2Neg) {
						map[startLayer][wy + q][wx + a].setselected(true);
					}
					else if (t1Neg) {
						map[startLayer][wy + q][wx - a].setselected(true);
					}
					else if (t2Neg) {
						map[startLayer][wy - q][wx + a].setselected(true);
					}
					else {
						map[startLayer][wy - q][wx - a].setselected(true);
					}
				}
			}

		}

		firstPoint = false;
		multiSelcting = false;
	}
}

//clears an area from the first point to the point given in the prameters
void world::clearArea() {

	if (pointOne == pointTwo) {
		return;
	}

	int diffy = pointOne.y - pointTwo.y;
	int diffx = pointOne.z - pointTwo.z;

	int wx = pointOne.z;
	int wy = pointOne.y;

	//make sure the the differences are positive
	int t1 = diffy;
	int t2 = diffx;

	bool t1Neg = false;
	bool t2Neg = false;
	if (t1 < 0) {
		t1 *= -1;
		t1Neg = true;
	}
	if (t2 < 0) {
		t2 *= -1;
		t2Neg = true;
	}
	for (int q = 0; q < t1 + 1; q++) {
		for (int a = 0; a < t2 + 1; a++) {

			if (t1Neg && t2Neg) {
				map[startLayer][wy + q][wx + a].setselected(false);
			}
			else if (t1Neg) {
				map[startLayer][wy + q][wx - a].setselected(false);
			}
			else if (t2Neg) {
				map[startLayer][wy - q][wx + a].setselected(false);
			}
			else {
				map[startLayer][wy - q][wx - a].setselected(false);
			}
		}
	}

}



void world::shawdowSelect(int yi, int xi) {
	if (multiSelcting && firstPoint) {
		
		//pointOne.y != yi || pointOne.z != xi
		if (!point2init) {
			pointTwo.x = startLayer;
			pointTwo.y = yi;
			pointTwo.z = xi;
			point2init = true;
		}
		else {
				//clearArea();
				proccessMouse(0, GLUT_DOWN, xi, yi);
				firstPoint = true;
				multiSelcting = true;
				//map[startLayer][(int)pointTwo.y][(int)pointTwo.z].setselected(false);
				//pointTwo.y = yi;
				//pointTwo.z = xi;
				//map[startLayer][yi][xi].setselected(true);
			
		}
	}
}

bool world::cmpcolor(unsigned char colora[], vec3 colorb)
{
	return((colora[0] == int(colorb.x * 255 + 0.5)) &&
		(colora[1] == int(colorb.y * 255 + 0.5)) &&
		(colora[2] == int(colorb.z * 255 + 0.5)));
}

bool world::cmpcolor2(unsigned char colora[], vec3 colorb)
{
	return((colora[0] == int(colorb.x * 255 - 0.5)) &&
		(colora[1] == int(colorb.y * 255 - 0.5)) &&
		(colora[2] == int(colorb.z * 255 - 0.5)));
}


bool world::cmpcolor3(unsigned char colora[], vec3 colorb)
{
	return((colora[0] == int(colorb.x * 255 )) &&
		(colora[1] == int(colorb.y * 255 )) &&
		(colora[2] == int(colorb.z * 255 )));
}


bool world::cmpcolor4(unsigned char colora[], vec3 colorb)
{
		return (  ((colora[0] == int(colorb.x * 255 - 0.5)) || (colora[0] == int(colorb.x * 255 + 0.5) )) &&
			      ((colora[1] == int(colorb.y * 255 - 0.5)) || (colora[1] == int(colorb.y * 255 + 0.5))) &&
		          (colora[2] == int(colorb.z * 255 - 0.5) || colora[2] == int(colorb.z * 255 + 0.5)));
}

void world::reveale(int f, int y, int x) {

	if (f == 0) {
		map[f + 1][y][x].setvissible(true);
	}
	else if (f == height) {
		map[f - 1][y][x].setvissible(true);
	}
	else {
		map[f - 1][y][x].setvissible(true);
		map[f + 1][y][x].setvissible(true);
	}

	if (x == 0) {
		map[f][y][x + 1].setvissible(true);
	}
	else if (x == xwidth) {
		map[f][y][x - 1].setvissible(true);
	}
	else {
		map[f][y][x - 1].setvissible(true);
		map[f][y][x + 1].setvissible(true);
	}

	if (y == 0) {
		map[f][y + 1][x].setvissible(true);
	}
	else if (y == ywidth) {
		map[f][y - 1][x].setvissible(true);
	}
	else {
		map[f][y - 1][x].setvissible(true);
		map[f][y + 1][x].setvissible(true);
	}

}