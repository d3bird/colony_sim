#include "creature.h"

creature::creature() {
	placeIndex = 0;


	width = 135;//315
	length = 135;//
	height = 135;//


	loc.x = 0;
	loc.y = 0;
	loc.z = 0;

	goal = loc;

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

	theta[0] = 90.0;
	theta[1] = 0.0;
	theta[2] = 0.0;

	defColor = color4(0, 0, 1.0, 1.0);

	movespeed = 0.001;
	destOffset = 0.02;
	newGoal = false;
	arivived = true;
}

void creature::draw() {
	calTranMat();
	glUniform4f(coloring, defColor.x, defColor.y, defColor.z, defColor.w);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void creature::draw(bool i) {
	calTranMat();
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

bool creature::update() {
	if (newGoal) {
		pathFiding();
		tloc = Translate(loc.x * gridOfset, loc.y * gridOfset, loc.z * gridOfset);
		model_veiw_base = tloc * ctm;
		glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
	}
	return arivived;

}

//create the path to the point given
//for now it just adds it to the que, going stright to the point
void creature::createPathTo(vec3 i) {
	addLocToQue(i);

}

//this function moves the creature through the queued up locations
void creature::pathFiding() {

	//std::cout << goal.z << " " << loc.z << std::endl;

	vec2 dir = vec2(goal.x - loc.x, goal.z - loc.z);
	//  if (length(dir)==0.0) {

	//  }else{
	if (std::sqrt(dot(dir, dir)) > 0.01) {
		dir = (compute_time()) * movespeed; //* normalize(dir);
		// Update location

		//loc.x += dir.x;
		if (loc.x < goal.x) {
			loc.x += dir.x;
		}
		else {
			loc.x -= dir.x;
		}

		if (loc.z < goal.z) {
			loc.z += dir.y;
		}
		else {
			loc.z -= dir.y;
		}

	}
	else {
		//rests the location
		loc.x = goal.x;
		loc.z = goal.z;
		//grabs the next location off of the queue
		if (travelingPoints.empty()) {
			newGoal = false;
			arivived = true;
		}
		else {
			vec3 temp = travelingPoints.front();
			travelingPoints.pop();
			goal = temp;
			newGoal = true;
			arivived = false;
		}
	}
	set_last_time();

}
	
void creature::addLocToQue(vec3 i) {

	if (!newGoal) {
		newGoal = true;
		arivived = false;
		setGoal(i);
		set_last_time();
	}
	else {
		travelingPoints.push(i);
	}
}

void creature::init() {
	colorcube();
	calTranMat();
	if (placeIndex == 0) {
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)*2, sizeof(points), points);
	}
}

void creature::setLoc(vec3 i) {
	loc = i;
	goal = i;
	tloc = Translate(loc.x * gridOfset, loc.y * gridOfset, loc.z * gridOfset);
	changed = true;
	newGoal = false;
}

void creature::setGoal(vec3 i) {
	std::cout << "goal was to z: " << goal.z << " y: " << goal.y << " x: " << goal.x << std::endl;
	goal.x = i.x;
	goal.y = i.y;
	goal.z = i.z;
	std::cout << "goal changed to: " << goal.z << " " << goal.y << " " << goal.x << std::endl;
	newGoal = true;
}


void creature::MyQuad(int a, int b, int c, int d) {
	static int i = 0;
	/*
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
	*/
	//quad_color[i] = material_diffuse;
	points[i] = vertices[a];// for the table base
	i++;
	//quad_color[i] = material_diffuse;
	points[i] = vertices[b];// for the table base
	i++;
	//quad_color[i] = material_diffuse;//
	points[i] = vertices[c];// for the table base
	i++;
	//quad_color[i] = material_diffuse;
	points[i] = vertices[a];// for the table base
	i++;
	//quad_color[i] = material_diffuse;
	points[i] = vertices[c];// for the table base
	i++;
	//quad_color[i] = material_diffuse;
	points[i] = vertices[d];// for the table base
	i++;
	i %= 36;


}


void creature::colorcube() {
	MyQuad(1, 0, 3, 2);
	MyQuad(2, 3, 7, 6);
	MyQuad(3, 0, 4, 7);
	MyQuad(6, 5, 1, 2);
	MyQuad(4, 5, 6, 7);
	MyQuad(5, 4, 0, 1);
}

void creature::calTranMat() {
	ctm = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);//rotes the cube
	model_veiw_base = tloc * ctm;
	glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
	glUniform4f(coloring, defColor.x, defColor.y, defColor.z, defColor.w);
}


void creature::set_last_time()
{
	// When did we last update object
	last_time = glutGet(GLUT_ELAPSED_TIME);
}

// Update the last time object was modified to now.
void creature::reset_time()
{
	// What is the time in the simulation.
	current_time = 0;
}

// Returns the amount of time since we last updated the object.
GLint creature::compute_time()
{
	return(glutGet(GLUT_ELAPSED_TIME) - last_time);
}