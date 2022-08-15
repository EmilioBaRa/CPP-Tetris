#pragma once
#include <stdio.h>
#include <iostream>
#include <sstream>


class Point {
	friend class TestSuite;
private:
	int x;
	int y;

public:
	//constructor with non-defined variables
	Point();

	//constructor with x and y included
	Point(int x, int y);

	//get the value of x
	int getX() const;

	//get the value of y
	int getY() const;

	//set value of x
	void setX(int x);

	//set value of y
	void setY(int y);

	// set value of x and y
	void setXY(int x, int y);

	// swap x and y
	void swapXY();

	// multiply x by some factor
	void multiplyX(int factor);

	// multiply y by some factor
	void multiplyY(int factor);

	// return a string in the form "[x,y]" to represent the state of the Point instance 
	// (for debugging) You could use stringstream if you want to try a forming a string in 
	// the same way you use cout http://stev.org/post/cstringstreamexamplefor an example
	std::string toString();
};
