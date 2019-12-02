//******************************************************************
//                                                                  
//  Class: room1
//                                                                  
//  Purpose:  hold the information to generate the first room and 
//            the differnt objects that it contains
//            
//  Functions:
//             draw()               //draws the room and its objects
//             update()             //updated the objects in the room
//             isCollission()       // determin if the player is out of bounds
//             foundSpecial()       // determin if the interaction hit a special object
//******************************************************************
#pragma once

#include "Angel.h"
#include <vector>
#include <iostream>
#include "cube.h"
#include "tree.h"
#include "creature.h"
#include <vector>

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;


class world{
public:
    world();
    ~world();

    void draw();
    void update();

	void init();
	
	void increaseLayer();
	void decreaseLayer();

	void setModelVeiw(GLuint i) { trans = i; trees->setModelVeiw(trans); }
	void setColorloc(GLuint i) { coloring = i; trees->setColorloc(trans); }
	
	void setGridLines(bool i) { gridlines = i; }
	bool drawlingGrids() { return gridlines; }

	void setDrawHidden(bool i) { drawhidden = i; }
	bool drawlinghidden() { return drawhidden; }

	void proccessMouse(int btn, int state, int x, int y);

	void toggleDebug() { debug = !debug; }

	void setWindow(int x, int y) { win_w = x; win_h = y; }

	void setMultiselecct() { multiSelcting = true; }
	void shawdowSelect(int yi, int xi);

	void setDestination() { direction = true; }
	void setMining() { mining = true; }
private:
	
	void processSelection(unsigned char PixelColor[], int btn);
	bool cmpcolor(unsigned char colora[], vec3 colorb);
	bool cmpcolor2(unsigned char colora[], vec3 colorb);
	bool cmpcolor3(unsigned char colora[], vec3 colorb);
	bool cmpcolor4(unsigned char colora[], vec3 colorb);
	
	void multiselect(int yi, int xi);
	void clearArea();
	

	float height;//the total height of the world
	float ywidth;// the total width of the world
	float xwidth;// the total length of the world

	int drawdistance;
	int startLayer;

	int overworld_height;

	bool gridlines;
	bool drawhidden;

	//these are the different bools to determin what the mouse does
	bool multiSelcting;

	

	//these are for the multi select command
	bool firstPoint;
	bool diffpoint;
	bool point2init;
	vec3 pointOne;
	vec3 pointTwo;

	cube*** map;//the map of the whole world
	color4** selection;// the colors for selection
	tree* trees;
	std::vector<tree*>* treeList;// the trees are based on layers on wich they are grown
	std::vector<creature*> Creatures;
	GLuint trans, coloring;

	int win_h;
	int win_w;

	bool debug;

	//everything to do with the command system
	bool direction;
	bool mining;
};
