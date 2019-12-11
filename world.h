//******************************************************************
//                                                                  
//  Class: world
//                                                                  
//  Purpose:   
//           
//            
//  Functions:
//             draw()               //draws the room and its objects
//             update()             //updated the objects in the room
//             ()       // 
//             ()       // 
//******************************************************************
#pragma once

#include "Angel.h"
#include <vector>
#include <iostream>
#include "cube.h"
#include "tree.h"
#include "creature.h"
#include "colonist.h"
#include "Pumpkin.h"


typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

struct job {

	int colinistID; //if == to -1 then no one is working on it
	vec3 loc; //were the job is located
	vec3 mloc; //the location to move to do the job
	int jobtype;

};

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

	void addJobToQue(job i);
	void compleatJob(int i);
	void checkJobs();
	void assignJobs();

	void toggleDestination() { direction = !direction; mining = false;}
	void toggleMining() { mining = !mining; direction = false; }
	void toggleHarvesting() { harvest = !harvest; direction = false; mining = false; }

	void printJobs();
	
private:
	
	void processSelection(unsigned char PixelColor[], int btn);
	bool cmpcolor(unsigned char colora[], vec3 colorb);
	bool cmpcolor2(unsigned char colora[], vec3 colorb);
	bool cmpcolor3(unsigned char colora[], vec3 colorb);
	bool cmpcolor4(unsigned char colora[], vec3 colorb);
	
	void multiselect(int yi, int xi);
	void clearArea();
	
	void reveale(int f, int y, int x);

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

	//for the last seleceteed location
	int x;
	int y;
	int z;
	int lplant;


	cube*** map;//the map of the whole world
	color4** selection;// the colors for selection
	tree* trees;

	std::vector<tree*>* treeList;// the trees are based on layers on wich they are grown
	std::vector<colonist*> colonists;
	std::vector<Pumpkin*> pumpkins;

	int MaxPlants;
	color4* plantSelection;

	GLuint trans, coloring;

	int win_h;
	int win_w;

	bool debug;

	//everything to do with the command system
	bool direction;
	bool mining;
	bool harvest;

	std::vector<job> jobs;
};
