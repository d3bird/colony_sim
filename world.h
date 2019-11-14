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


typedef Angel::vec4  point4;
typedef Angel::vec4  color4;


class world{
public:
    world();
    ~world();

    void draw();
    void update();

	void init();

	void setModelVeiw(GLuint i) { trans = i; }
	void setColorloc(GLuint i) { coloring = i; }

private:

	int height;
	int ywidth;
	int xwidth;

	int overworld_height;

	cube*** map;

	GLuint trans, coloring;

};
