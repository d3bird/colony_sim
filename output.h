#pragma once

#include <iostream>
#include <string>

class output {
public:

	output();
	~output();

	void println(std::string i);
	void println(int i);
	void println(double i);
	void println(char i);

	void print(std::string i);
	void print(int i);
	void print(double i);
	void print(char i);

private:


};

