#include "cube.h"


typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

cube::cube() {

	theta[0] = 90.0;
	theta[1] = 0.0;
	theta[2] = 0.0;

	material_ambient = color4(1.0, 0.0, 1.0, 1.0);
	material_diffuse = color4(0.0, 0.8, 1.0, 1.0); // color of the chair
	material_specular = color4(1.0, 0.8, 0.0, 1.0);
	material_shininess = 100.0;
	light_position = point4(0.0, 200.0, 1.0, 0.0);
	light_ambient = color4(0.2, 0.2, 0.2, 1.0);
	light_diffuse = color4(1.0, 1.0, 1.0, 1.0);
	light_specular = color4(1.0, 1.0, 1.0, 1.0);
	viewer = vec4(0.0, 0.0, -1.0, 0.0);
	spec = true;

	scale = 0;
	//for the base of the table
	width = 135;//315
	length = 135;//
	height = 135;//


	loc.x = 0;
	loc.y = 0;
	loc.z = 0;

	special = false;

	// table stats
	vertices[0] = point4(-width, -(height), length, 1.0);
	vertices[1] = point4(-width, (height), length, 1.0);
	vertices[2] = point4(width, (height), length, 1.0);
	vertices[3] = point4(width, -(height), length, 1.0);
	vertices[4] = point4(-width, -(height), -length, 1.0);
	vertices[5] = point4(-width, (height), -length, 1.0);
	vertices[6] = point4(width, (height), -length, 1.0);
	vertices[7] = point4(width, -(height), -length, 1.0);

	

	tloc = Translate(loc.x, loc.y, loc.z);// the location of the table

	placeIndex = 0;
	//Modeltrans = 0;
	//init();
}

void cube::setLoc(vec3 i){
	loc = i;
	tloc = Translate(loc.x, loc.y, loc.z);
}



void cube::MyQuad(int a, int b, int c, int d) {
	static int i = 0;

	// Compute normal vector.
	vec3 n1 = normalize(cross(ctm * vertices[b] - ctm * vertices[a], ctm * vertices[c] - ctm * vertices[b]));
	vec4 n = vec4(n1[0], n1[1], n1[2], 0.0);
	vec4 half = normalize(light_position + viewer);
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
	}

	quad_color[i] = material_diffuse;
	points[i] =  vertices[a];// for the table base
	i++;
	quad_color[i] = material_diffuse;
	points[i] =  vertices[b];// for the table base
	i++;
	quad_color[i] = material_diffuse;
	points[i] =  vertices[c];// for the table base
	i++;
	quad_color[i] = material_diffuse;
	points[i] =  vertices[a];// for the table base
	i++;
	quad_color[i] = material_diffuse;
	points[i] =  vertices[c];// for the table base
	i++;
	quad_color[i] = material_diffuse;
	points[i] =  vertices[d];// for the table base
	i++;
	i %= 36;


}




void cube::colorcube() {
	MyQuad(1, 0, 3, 2);
	MyQuad(2, 3, 7, 6);
	MyQuad(3, 0, 4, 7);
	MyQuad(6, 5, 1, 2);
	MyQuad(4, 5, 6, 7);
	MyQuad(5, 4, 0, 1);
}



void cube::calTranMat() {
	ctm = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);//rotes the cube
	model_veiw_base = tloc * ctm;
	glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
}



void cube::draw() {
	//ctm = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);//rotes the cube
	//calTranMat();
	//colorcube();

	calTranMat();
	//std::cout << temp << std::endl;

	//if (placeIndex == 0) {
	//	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // the top of the table
	//}
	//else {
glDrawArrays(GL_TRIANGLES, NumVertices * placeIndex, NumVertices * (placeIndex + 1)); // the top of the table
	//}

}

void cube::update(){


}

void cube::init() {
	calTranMat();
	colorcube();
	//if (placeIndex == 0) {
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);//old way
	//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(quad_color), quad_color);
	//}
	//else {
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) * placeIndex, sizeof(points), points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) * (placeIndex + 1), sizeof(quad_color), quad_color);
	//}
	glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
}