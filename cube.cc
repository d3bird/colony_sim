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

	outline = color4(0.0, 0.0, 0.0, 1.0);
	hidden = color4(0.67, 0.70, 0.75, 1.0);
	scale = 0;
	//for the base of the table
	width = 135;//315
	length = 135;//
	height = 135;//


	loc.x = 0;
	loc.y = 0;
	loc.z = 0;

	//special = false;
	selected = false;
	// table stats
	vertices[0] = point4(-width, -(height), length, 1.0);
	vertices[1] = point4(-width, (height), length, 1.0);
	vertices[2] = point4(width, (height), length, 1.0);
	vertices[3] = point4(width, -(height), length, 1.0);
	vertices[4] = point4(-width, -(height), -length, 1.0);
	vertices[5] = point4(-width, (height), -length, 1.0);
	vertices[6] = point4(width, (height), -length, 1.0);
	vertices[7] = point4(width, -(height), -length, 1.0);

	vissible = false;

	tloc = Translate(loc.x, loc.y, loc.z);// the location of the table

	placeIndex = 0;
	//Modeltrans = 0;
	//init();
	changed = true;

	Scolor = color4(1.0, 0.0, 1.0, 1.0);
}


//this function converts the grid coridants to draw positions on the screen
void cube::setLoc(vec3 i){
	loc = i;
	tloc = Translate(loc.x * gridOfset, loc.y * gridOfset, loc.z * gridOfset);
	changed = true;
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
	glUniform4f(coloring, material_diffuse.x, material_diffuse.y, material_diffuse.z, material_diffuse.w);
}



void cube::draw(bool i) {
	//only update the matrix if somehting has changed about the cube
	if (changed) {
		calTranMat();
		changed = false;
	}
	else {
		glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
		glUniform4f(coloring, material_diffuse.x, material_diffuse.y, material_diffuse.z, material_diffuse.w);
	}

	if (selected) {
		glUniform4f(coloring, Scolor.x, Scolor.y, Scolor.z, Scolor.w);
	}
	else if (!vissible) {
		glUniform4f(coloring, hidden.x, hidden.y, hidden.z, hidden.w);
	}

	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // the top of the table
	//if we need to draw the outline
	if (i) {
		glUniform4f(coloring, outline.x, outline.y, outline.z, outline.w);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		model_veiw_base = tloc * ctm * Scale(1.0001, 1.0001, 1.0001);
		glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//draw with the selected color
void cube::drawSelec(color4 i) {
	calTranMat();
	glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
	glUniform4f(coloring, i.x, i.y, i.z, i.w);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void cube::update(){


}

void cube::init() {
	calTranMat();
	colorcube();
	if (placeIndex == 0) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);//old way
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(quad_color), quad_color);
	}
	else {
		//std::cout << "cube buffer loc" << std::endl;
		//std::cout << "start loc: " << sizeof(points) * placeIndex << " size: " << sizeof(points) << std::endl;
		//glBufferSubData(GL_ARRAY_BUFFER, (sizeof(points) + sizeof(quad_color) * placeIndex), sizeof(points), points);//old way
		//glBufferSubData(GL_ARRAY_BUFFER, ((sizeof(points) + sizeof(quad_color)) * placeIndex) , sizeof(quad_color), quad_color);
		//glBufferSubData(GL_ARRAY_BUFFER, (sizeof(points) * placeIndex), sizeof(points), points);//old way
		//glBufferSubData(GL_ARRAY_BUFFER, ((sizeof(points) + sizeof(quad_color)) * placeIndex) , sizeof(quad_color), quad_color);
	}
	glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
	glUniformMatrix4fv(coloring, 1, GL_TRUE, (material_diffuse));
}

