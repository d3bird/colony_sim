#include "world.h"



world::world(){
    //intalising data for the floor
    material_ambient = color4(1.0, 0.0, 1.0, 1.0);
	material_diffuse = color4(1.0, 0.8, 0.0, 1.0);
	material_specular = color4(1.0, 0.8, 0.0, 1.0);
	material_shininess = 100.0;
	light_position = point4(0.0, 20.0, -40.0, 0.0);
	light_ambient = color4(0.2, 0.2, 0.2, 1.0);
	light_diffuse = color4(1.0, 1.0, 1.0, 1.0);
	light_specular = color4(1.0, 1.0, 1.0, 1.0);
	Fviewer = vec4(0.0, 0.0, 1.0, 0.0);
	spec = false;
	int tscale = 0;
	int width = 4000;//135
	int length = -5;//
	int height = 4000;//
 
	int Twidth = 40;//
	int Tlength = 200;//
	int Theight = 30;//

   // table stats
	Fvertices[0] = point4(-width, -(height), length, 1.0);
	Fvertices[1] = point4(-width, (height), length, 1.0);
	Fvertices[2] = point4(width, (height), length, 1.0);
	Fvertices[3] = point4(width, -(height), length, 1.0);
	Fvertices[4] = point4(-width, -(height), -length, 1.0);
	Fvertices[5] = point4(-width, (height), -length, 1.0);
	Fvertices[6] = point4(width, (height), -length, 1.0);
	Fvertices[7] = point4(width, -(height), -length, 1.0);


	Floc.x = 0;
	Floc.y = 0;
	Floc.z = height- height/3;//ceiling
	FTloc = Translate(Floc.x, Floc.y, Floc.z);// get the transformation for the floor
    Cloc.x =0;
    Cloc.y =0;
    Cloc.z = -Floc.z;
    CTloc = Translate(Cloc.x, Cloc.y, Cloc.z);// get the transformation for the floor
    florC = color4(1.0,0.0,1.0,1.0);
    ceilC = color4(0.0,0.0,1.0,1.0);

    W1loc.x = 0;
    W1loc.y = 0;
    W1loc.z = Floc.z*1.5;
    W1Tloc = Translate(W1loc.x, W1loc.y, W1loc.z);
    wall1C = color4(1.0,1.0,0.0,1.0);

    W2loc.x = 0;
    W2loc.y = 0;
    W2loc.z = -(Floc.z*1.5);
    W2Tloc = Translate(W2loc.x, W2loc.y, W2loc.z);
    wall2C = color4(0.0,1.0,0.0,1.0);

    W3loc.x = 0;
    W3loc.y = 0;
    W3loc.z = Floc.z*1.5;
    W3Tloc = Translate(W3loc.x, W3loc.y, W3loc.z);
    wall3C = color4(0.0,1.0,1.0,1.0);

    W4loc.x = 0;
    W4loc.y = 0;
    W4loc.z = -(Floc.z*1.5);
    W4Tloc = Translate(W4loc.x, W4loc.y, W4loc.z);
    wall4C = color4(1.0,1.0,0.5,1.0);

    vec3 ilov = vec3(1000,(height- height/3),0); 
    ncc = new cube();
    vec3 chairloc = vec3(ilov.x, ilov.z ,ilov.y - ncc->getHeight());// tempplate for drawing a table
    ncc->setLoc(chairloc);
    ncc ->updateVeiwer(chairloc);
    ncc ->updateLightPos(chairloc);
    ncc->setspecial(true);
    cc.push_back(ncc);

    ilov = vec3(0,(height- height/3),1000); 
    ncc = new cube();
    chairloc = vec3(ilov.x, ilov.z ,ilov.y - ncc->getHeight());// tempplate for drawing a table
    ncc->setLoc(chairloc);
    ncc ->updateVeiwer(chairloc);
    ncc ->updateLightPos(chairloc);
    ncc->setspecial(false);
    cc.push_back(ncc);

    ilov = vec3(-1500,(height- height/3),1000); 
    ncc = new cube();
    chairloc = vec3(ilov.x, ilov.z ,ilov.y - ncc->getHeight());// tempplate for drawing a table
    ncc->setLoc(chairloc);
    ncc ->updateVeiwer(chairloc);
    ncc ->updateLightPos(chairloc);
    ncc->setspecial(false);
    cc.push_back(ncc);

    ilov = vec3(0,(height- height/3),-2000); 
    ncc = new cube();
    chairloc = vec3(ilov.x, ilov.z ,ilov.y - ncc->getHeight());// tempplate for drawing a table
    ncc->setLoc(chairloc);
    ncc ->updateVeiwer(chairloc);
    ncc ->updateLightPos(chairloc);
    ncc->setspecial(false);
    cc.push_back(ncc);

    determinLighting();
}

world::~world(){


}

 bool world::isCollission(){

     if(player.x* 1000 >= 3600 || player.x* 1000 <= -3600 ){
         return true;
     }else if (player.y* 1000 >= 2000 || player.y* 1000 <= -2000 ){
         return true;
     }else if (player.z* 1000 >= 3200 || player.z* 1000 <= -3200 ){
            return true;
     }else{
         return false;
     }

}

bool world::foundSpecial(){
     if ((player.x* 1000 >= 0 && player.x* 1000 <= 1650 ) &&
        (player.y* 1000 >= -2000 && player.y* 1000 <= -1250 ) &&
        (player.z* 1000 >= -115.328 && player.z* 1000 <= 800 )
        )
     {
        return true;
     }

    return false;
}

void world::MyQuad(int a, int b, int c, int d) {
	static int i = 0;
    float Stheta[3] = {0,90,0}; //side walls
    float theta[3] = {90,0,0}; //floor walls
    mat4 Fctm = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);
    mat4 Sctm = RotateX(Stheta[0]) * RotateY(Stheta[1]) * RotateZ(Stheta[2]);
	// Compute normal vector.
	/*vec3 n1 = normalize(cross(FTloc * Fvertices[b] - FTloc * Fvertices[a], FTloc * Fvertices[c] - FTloc * Fvertices[b]));
	vec4 n = vec4(n1[0], n1[1], n1[2], 0.0);
	vec4 half = normalize(light_position + Fviewer);
	half.w = 0.0;
	color4 ambient_color, diffuse_color, specular_color;

	ambient_color = product(material_ambient, light_ambient);

	float dd = dot(light_position, n);
	if (dd > 0.0) {
		diffuse_color = dd * product(light_diffuse, material_diffuse);
	}
	else {
		diffuse_color = color4(0.0, 0.0, 0.0, 1.0);
	}

	dd = dot(half, n);
	if (dd > 0.0) {
		specular_color = exp(material_shininess * log(dd)) * product(light_specular, material_specular);
	}
	else {
		specular_color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	// If we don't want specular color added
	if (!spec) {
		specular_color = vec4(0.0, 0.0, 0.0, 1.0);
	}*/

	Fquad_color[i] = florC;
	Fpoints[i] = Fctm * FTloc * Fvertices[a];
    
	Cquad_color[i] = ceilC;
	Cpoints[i] = Fctm * CTloc * Fvertices[a];

    W1quad_color[i] = wall1C;
	W1points[i] = Sctm * W1Tloc * Fvertices[a];
        W2quad_color[i] = wall2C;
	W2points[i] = Sctm * W2Tloc * Fvertices[a];
        W3quad_color[i] = wall3C;
	W3points[i] = W3Tloc * Fvertices[a];     
                W4quad_color[i] = wall4C;
	W4points[i] = W4Tloc * Fvertices[a];        
	i++;
	Fquad_color[i] = florC;
	Fpoints[i] = Fctm * FTloc * Fvertices[b];

	Cquad_color[i] = ceilC;
	Cpoints[i] = Fctm * CTloc * Fvertices[b];

        W1quad_color[i] = wall1C;
	W1points[i] = Sctm * W1Tloc * Fvertices[b];
        W2quad_color[i] = wall2C;
	W2points[i] = Sctm * W2Tloc * Fvertices[b];
        W3quad_color[i] = wall3C;
	W3points[i] = W3Tloc * Fvertices[b];
                W4quad_color[i] = wall4C;
	W4points[i] = W4Tloc * Fvertices[b];               
	i++;
	Fquad_color[i] = florC;
	Fpoints[i] = Fctm * FTloc * Fvertices[c];

 	Cquad_color[i] = ceilC;
	Cpoints[i] = Fctm * CTloc * Fvertices[c];

        W1quad_color[i] = wall1C;
	W1points[i] = Sctm * W1Tloc * Fvertices[c];
        W2quad_color[i] = wall2C;
	W2points[i] = Sctm * W2Tloc * Fvertices[c];
        W3quad_color[i] = wall3C;
	W3points[i] = W3Tloc * Fvertices[c]; 
                W4quad_color[i] = wall4C;
	W4points[i] = W4Tloc * Fvertices[c];             
	i++;
	Fquad_color[i] = florC;
	Fpoints[i] = Fctm * FTloc * Fvertices[a];

	Cquad_color[i] = ceilC;
	Cpoints[i] = Fctm * CTloc * Fvertices[a];

        W1quad_color[i] = wall1C;
	W1points[i] = Sctm * W1Tloc * Fvertices[a];
        W2quad_color[i] = wall2C;
	W2points[i] = Sctm * W2Tloc * Fvertices[a];  
        W3quad_color[i] = wall3C;
	W3points[i] = W3Tloc * Fvertices[a];    
                W4quad_color[i] = wall4C;
	W4points[i] = W4Tloc * Fvertices[a];       
	i++;
	Fquad_color[i] = florC;
	Fpoints[i] = Fctm * FTloc * Fvertices[c];

	Cquad_color[i] = ceilC;
	Cpoints[i] = Fctm * CTloc * Fvertices[c];

        W1quad_color[i] = wall1C;
	W1points[i] = Sctm * W1Tloc * Fvertices[c]; 
                W2quad_color[i] = wall2C;
	W2points[i] = Sctm * W2Tloc * Fvertices[c]; 
        W3quad_color[i] = wall3C;
	W3points[i] = W3Tloc * Fvertices[c];   
                W4quad_color[i] = wall4C;
	W4points[i] = W4Tloc * Fvertices[c];       
	i++;
	Fquad_color[i] = florC;
	Fpoints[i] = Fctm * FTloc * Fvertices[d];

	Cquad_color[i] = ceilC;
	Cpoints[i] = Fctm * CTloc * Fvertices[d]; 

        W1quad_color[i] = wall1C;
	W1points[i] = Sctm * W1Tloc * Fvertices[d];
            W2quad_color[i] = wall2C;
	W2points[i] = Sctm * W2Tloc * Fvertices[d];
        W3quad_color[i] = wall3C;
	W3points[i] = W3Tloc * Fvertices[d];
            W4quad_color[i] = wall4C;
	W4points[i] = W4Tloc * Fvertices[d];                      
	i++;
	i %= 36;


}


void world::determinLighting(){

  MyQuad(1,0,3,2);
  MyQuad(2,3,7,6);
  MyQuad(3,0,4,7);
  MyQuad(6,5,1,2);
  MyQuad(4,5,6,7);
  MyQuad(5,4,0,1);
}

void world::updateLightpos(point4 i){


}



void world::draw(){

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Fpoints), Fpoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Fpoints), sizeof(Fquad_color), Fquad_color);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // the floor

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Fpoints), Cpoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Fpoints), sizeof(Fquad_color), Cquad_color);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // the cieling
    if(!open){
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Fpoints), W1points);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Fpoints), sizeof(Fquad_color), W1quad_color);
        glDrawArrays(GL_TRIANGLES, 0, NumVertices); //  wall 1
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Fpoints), W2points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Fpoints), sizeof(Fquad_color), W2quad_color);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); //  wall 2

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Fpoints), W3points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Fpoints), sizeof(Fquad_color), W3quad_color);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); //  wall 3

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Fpoints), W4points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Fpoints), sizeof(Fquad_color), W4quad_color);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); //  wall 4
    //draw the chairs
    for (int i =0 ; i < cc.size(); i++){
        if(cc[i]->isspecial()){
           // std::cout<<"special"<<std::endl;
           // cc[i] ->updateLightPos(player);
           // cc[i] ->updateVeiwer(player);
            cc[i] ->draw();
        }else{
            cc[i] ->updateLightPos(player);
            //cc[i] ->updateVeiwer(player);
            cc[i] ->draw();
        }
    }

}
void world::update(){

}



