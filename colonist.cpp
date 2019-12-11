#include "colonist.h"

colonist::colonist() {

	setDefColor(color4(1.0, 0.0, 0.0, 1.0));
}

void colonist::Cinit() {
	init();
}

bool colonist::update() {
	bool arrived = creature::update();
	if (arrived) {

		idle = true;
		wandering = false;
		wander();

	}
	return arrived;
}

//preform the following task i at location il
void colonist::setTask(int i, vec3 il) {
	task = i;

	if (idle || wandering) {
	idle = false;
	wandering = false;
	createPathTo(il, false);
	}
	else {
	
		createPathTo(il, true);
	}
}

void colonist::wander() {
	

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> xaxis(0, 20);
		std::uniform_real_distribution<double> yaxis(0, 20);

		int xspot = (int) xaxis(mt);
		int yspot = (int)yaxis(mt);

		createPathTo(vec3(xspot, 0, yspot), false);
		wandering = true;
	
}


//returns if the colinist can do the task
bool colonist::candoTask(int i) {
	if (i < 0 || i >= numberOfJobs) {
		return false; //wrong number input
	}
	else {
		return cando[i];
	}
}

//returns the skill that colinist has with the job
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