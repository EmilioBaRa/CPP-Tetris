#include "Point.h"

//constructor with non-defined variables
Point::Point() {
	x = 0;
	y = 0;
}

//constructor with x and y included
Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}

//get the value of x
int Point::getX() const {
	return x;
}

//get the value of y
int Point::getY() const {
	return y;
}

// set value of x 
void Point::setX(int x) {
	this->x = x;
}

// set value of y
void Point::setY(int y) {
	this->y = y;
}

// set value of x and y
void Point::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}

// swap x and y values
void Point::swapXY() {
	int swapping{ x };
	x = y;
	y = swapping;
}

// multiply x by some factor
void Point::multiplyX(int factor) {
	x = x * factor;
}

// multiply y by some factor
void Point::multiplyY(int factor) {
	y = y * factor;
}

// return a string in the form "[x,y]" to represent the state of the Point instance 
// (for debugging) You could use stringstream if you want to try a forming a string in 
// the same way you use cout http://stev.org/post/cstringstreamexamplefor an example
std::string Point::toString() {
	std::stringstream stringingText;

	stringingText << "[" << x << "," << y << "]\n";
	std::string text{ stringingText.str() };

	return text;
}