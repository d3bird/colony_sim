#include "hud.h"

hud::hud() {
	width = 135;//315
	length = 135;//
	height = 135;//



	points[0] = point4(width, (height), 0, 1.0);
	points[1] = point4(-width, (height), 0, 1.0);
	points[2] = point4(-width, -(height), 0, 1.0);
	points[3] = point4(width, -(height), 0, 1.0);
	background = color4(1.0, 0.0, 0.0, 1.0);
}

void hud::draw() {
	glUniform4f(coloring, background.x, background.y, background.z, background.w);
	glDrawArrays(GL_TRIANGLE_FAN, 36, 42);
}

void hud::update() {


}

void hud::init() {
	//if (index == 0) {
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(points), points);//old way
	//}

}



