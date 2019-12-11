#pragma once
#include "Angel.h"
#include "creature.h"
#include <string>
#include <random>

class colonist : public creature{

public:

	colonist();
	bool update();
	void Cinit();

	//preform the following task i at location il
	void setTask(int i, vec3 il);

	//getters
	bool isIdle() { return idle; }
	int getTask() { return task; }
	std::string getName() { return name; }
	bool candoTask(int i);
	int getSkill(int i);

	void wander();

private:

	int index;
	std::string name;
	const static int numberOfJobs = 1;
	//skills at job types
	double skills[numberOfJobs];
	//jobs that he can/cannot do
	bool cando[numberOfJobs];

	//is it currently going to a job
	bool idle;
	int task;
	bool wandering;
};

