#include "Pumpkin.h"


typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

Pumpkin::Pumpkin() {

	theta[0] = 90.0;
	theta[1] = 0.0;
	theta[2] = 45.0;

	material_diffuse = color4(0.0, 0.8, 1.0, 1.0); // color of the chair


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

	vissible = false;

	tloc = Translate(loc.x, loc.y, loc.z);// the location of the table

	placeIndex = 0;

	changed = true;

	Scolor = color4(1.0, 0.0, 1.0, 1.0);
	fruitColor = color4(1.0, 0.4588, 0.941, 1.0);
	stemColor = color4(0.1882, 0.4941, 0.251, 1.0);
	tilt = 40;
	air = false;
	fruit_scale = 0.25;


	hasFruit = true;

	updateSpeed = 0.001;
	updateSpeed_Scale = 0.0001;
	current_time = 0;

	stage = 0;
	ytilt_current = 0;
	ytilt_Y = -47;
	ytilt_M = -32;
	ytilt_O = -21;


	fscale_stage = 0;
	fscale_Y = 0.0699999;
	fscale_M = 0.16;
	fscale_O = 0.26;
	
	tilt = ytilt_Y;
	fruit_scale = fscale_Y;

	stage = 0;
	harvested = false;
}


//this function converts the grid coridants to draw positions on the screen
void Pumpkin::setLoc(vec3 i) {
	loc = i;
	tloc = Translate((loc.x * gridOfset), (loc.y * gridOfset), (loc.z * gridOfset));
	changed = true;
}


void Pumpkin::calTranMat() {
	ctm = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);//rotes the cube
	stemBase = tloc * ctm * Scale(0.25, 0.25, 0.75);

	stemuppre = Translate((loc.x * gridOfset) +0.04, (loc.y * gridOfset)+.1, (loc.z * gridOfset)) * 
		RotateX(theta[0]) * RotateY(theta[1]+ tilt) * RotateZ(theta[2] + 157)
		* Scale(0.8, 0.25, 0.25);

	fruit = Translate((loc.x * gridOfset)+ 0.1, (loc.y * gridOfset), (loc.z * gridOfset)) * ctm * Scale(fruit_scale, fruit_scale, fruit_scale);

	glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
	glUniform4f(coloring, material_diffuse.x, material_diffuse.y, material_diffuse.z, material_diffuse.w);
}

void Pumpkin::calcOutlinesmats() {

	out_stemBase = tloc * ctm * Scale(0.25001, 0.25001, 0.75001);

	out_stemuppre = Translate((loc.x * gridOfset) + 0.04, (loc.y * gridOfset) + .1, (loc.z * gridOfset)) *
		RotateX(theta[0]) * RotateY(theta[1] + tilt) * RotateZ(theta[2] + 157)
		* Scale(0.8001, 0.25001, 0.25001);

	out_fruit = Translate((loc.x * gridOfset) + 0.1, (loc.y * gridOfset), (loc.z * gridOfset)) * ctm * Scale(fruit_scale + 0.001, fruit_scale + 0.001, fruit_scale + 0.001);

}

void Pumpkin::draw(bool i) {
	//only update the matrix if somehting has changed about the cube
	if (changed) {
		calTranMat();
		changed = false;
	}
	else {
		glUniformMatrix4fv(trans, 1, GL_TRUE, (model_veiw_base));
		glUniform4f(coloring, material_diffuse.x, material_diffuse.y, material_diffuse.z, material_diffuse.w);
	}


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUniform4f(coloring, stemColor.x, stemColor.y, stemColor.z, stemColor.w);
	glUniformMatrix4fv(trans, 1, GL_TRUE, (stemBase));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glUniformMatrix4fv(trans, 1, GL_TRUE, (stemuppre));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glUniform4f(coloring, fruitColor.x, fruitColor.y, fruitColor.z, fruitColor.w);
	glUniformMatrix4fv(trans, 1, GL_TRUE, (fruit));
	glDrawArrays(GL_TRIANGLES, 0, 36); 

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//if we need to draw the outline
	if (i) {

		calcOutlinesmats();
		glUniform4f(coloring, outline.x, outline.y, outline.z, outline.w);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glUniformMatrix4fv(trans, 1, GL_TRUE, (out_stemBase));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(trans, 1, GL_TRUE, (out_stemuppre));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniformMatrix4fv(trans, 1, GL_TRUE, (out_fruit));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//draw with the selected color
void Pumpkin::drawSelec(color4 i) {

}

void Pumpkin::update() {

	float chan_tilt =  (compute_time()) * updateSpeed;
	float chan_scale = (compute_time()) * updateSpeed_Scale;
	//std::cout << fruit_scale << std::endl;



	switch (stage){
	case 0:
		if (fruit_scale >= fscale_M && tilt >= ytilt_M) {
			fruit_scale = fscale_M;
			tilt = ytilt_M;
			stage = 1;
		}
		else {
			if (fruit_scale < fscale_M) {
				fruit_scale += chan_scale;
			}

			if (tilt < ytilt_M) {
				tilt += chan_tilt;
			}
		}
		break;
	case 1:
		if (fruit_scale >= fscale_O && tilt >= ytilt_O) {
			fruit_scale = fscale_O;
			tilt = ytilt_O;
			stage = 2;
		}
		else {
			if (fruit_scale < fscale_O) {
				fruit_scale += chan_scale;
			}

			if (tilt < ytilt_O) {
				tilt += chan_tilt;
			}
		}
		break;
	default://rests the growth of the plant if harvested
		if (harvested) {
			stage = 0;
			fruit_scale = fscale_Y;
			tilt = ytilt_Y;
			harvested = false;
		}
		break;
	}

	//if (tilt >= 360) {
	//	tilt = 0;
	//}
	//if (fruit_scale >= 5) {
	//	fruit_scale = 0;
	//}
	calTranMat();
	set_last_time();
}

void Pumpkin::init() {
	calTranMat();
}

void Pumpkin::printMesurments() {
	std::cout << "tilt: " << tilt << std::endl;
	std::cout << "fruit size: " << fruit_scale << std::endl;
}


void Pumpkin::raisStem() {
	tilt -= 1;
	changed = true;
}

void Pumpkin::LowerStem() {
	tilt += 1;
	changed = true;
}

void Pumpkin::enlargeFruit() {
	fruit_scale += 0.01;
	changed = true;
}

void Pumpkin::decreaseFruit() {
	changed = true;
	fruit_scale -= 0.01;
}

//needed to comput the time for the animation
void Pumpkin::set_last_time()
{
	// When did we last update object
	last_time = glutGet(GLUT_ELAPSED_TIME);
}

// Update the last time object was modified to now.
void Pumpkin::reset_time()
{
	// What is the time in the simulation.
	current_time = 0;
}

// Returns the amount of time since we last updated the object.
GLint Pumpkin::compute_time()
{
	return(glutGet(GLUT_ELAPSED_TIME) - last_time);
}