#pragma once

#include "Angel.h"
#include <vector>
#include <iostream>
#include "cube.h"


typedef Angel::vec4  point4;
typedef Angel::vec4  color4;


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

class world{
public:
    world(/* args */);
    ~world();

    void draw();
    void update();

    void determinLighting();

    void updateLightpos(point4 i);

    void updatePlayerpos(vec3 i){player = i;}

    bool isCollission();

    bool foundSpecial();
    void openDoor(){open = true;}
    bool isopen(){return open;}

private:
    void MyQuad(int a, int b, int c, int d); 
    bool spec;
    bool open;
	vec4 product(vec4 a, vec4 b) {
		return vec4(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
	}
    // misc values need for the walls
        vec4 Fviewer;
        const static int NumVertices = 36;
        vec3 player;
    //wall

   //floor 

    color4 material_ambient;
	color4 material_diffuse;
	color4 material_specular;
	float material_shininess;
	point4 light_position;
	color4 light_ambient;
	color4 light_diffuse;
	color4 light_specular;

    point4  Fvertices[8];// was 8
	point4 Fpoints[NumVertices];
	color4 Fquad_color[NumVertices];
    color4 florC;
    vec3 Floc;
    mat4 FTloc;
   //ceiling
	point4 Cpoints[NumVertices];
	color4 Cquad_color[NumVertices];
    color4 ceilC;
    vec3 Cloc;
    mat4 CTloc;
   //wall 1
	point4 W1points[NumVertices];
	color4 W1quad_color[NumVertices];
    color4 wall1C;
    vec3 W1loc;
    mat4 W1Tloc;
    //wall 2
	point4 W2points[NumVertices];
	color4 W2quad_color[NumVertices];
    color4 wall2C;
    vec3 W2loc;
    mat4 W2Tloc;
    //wall 3
  	point4 W3points[NumVertices];
	color4 W3quad_color[NumVertices];
    color4 wall3C;
    vec3 W3loc;
    mat4 W3Tloc;  
    //wall 4
	point4 W4points[NumVertices];
	color4 W4quad_color[NumVertices];
    color4 wall4C;
    vec3 W4loc;
    mat4 W4Tloc;


    //objects in the room
    cube* ncc;
    std::vector<cube*> cc; 
};
