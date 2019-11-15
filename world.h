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

private:

	int height;//the total height of the world
	int ywidth;// the total width of the world
	int xwidth;// the total length of the world

	int drawdistance;
	int startLayer;

	int overworld_height;

	bool gridlines;
	bool drawhidden;
	cube*** map;
	tree* trees;
	std::vector<tree*>* treeList;// the trees are based on layers on wich they are grown
	GLuint trans, coloring;

};
