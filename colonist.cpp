#include "colonist.h"

colonist::colonist() {

	setDefColor(color4(1.0, 0.0, 0.0, 1.0));
}

void colonist::Cinit() {
	init();
}

void colonist::update() {
	creature::update();
}

//preform the following task i at location il
void colonist::setTask(int i, vec3 il) {
	task = i;
	createPathTo(il);
}


bool colonist::candoTask(int i) {
	if (i < 0 || i >= numberOfJobs) {
		return false; //wrong number input
	}
	else {
		return cando[i];
	}
}

int colonist::getSkill(int i) {
	if (i < 0) {
		return -2; //wrong number input
	}
	else if(i>= numberOfJobs){
		return -1; //too large of a number
	}
	else {
		return skills[i];
	}
}