#include "Tetromino.h"

Tetromino::Tetromino(){
	setShape(TetShape::T);//Default constructor to T shapes
}

//get the color of the tetronimo
TetColor Tetromino::getColor() const {
	return color;
}

//get the shape of the tetronimo
TetShape Tetromino::getShape() const {
	return shape;
}

// set the shape
//  -clear any blockLocs set previously
//  -set the blockLocs for the shape
//  -set the color for the shape
void Tetromino::setShape(TetShape shape) {
	blockLocs.empty();
	color = static_cast<TetColor>(shape);

	//switch (static_cast<int>(shape)) {
	switch (shape) {
		case TetShape::S: //S
			blockLocs = { Point(0,0),  Point(-1,0),  Point(0,1), Point(1,1) };
			break;
		case TetShape::Z:  //Z
			blockLocs = { Point(0,0),  Point(1,0),  Point(0,1), Point(-1,1) };
			break;
		case TetShape::L: //L
			blockLocs = { Point(0,0),  Point(0,1),  Point(0,-1), Point(1,-1) };
			break;
		case TetShape::J: //J
			blockLocs = { Point(0,0),  Point(0,-1),  Point(-1,-1), Point(0,1) };
			break;
		case TetShape::O: //O
			blockLocs = { Point(0,0),  Point(0,1),  Point(1,1), Point(1,0) };
			break;
		case TetShape::I: //I
			blockLocs = { Point(0,0),  Point(0,-1),  Point(0,1), Point(0,2) };
			break;
		case TetShape::T: //T
			blockLocs = { Point(0,0),  Point(-1,0),  Point(1,0), Point(0,-1) };
			break;
		default:
			blockLocs = { Point(0,0) };
	}
	
}

//rotate the shape 90 degrees around[0, 0](clockwise)
// to do this:
//  -iterate through blockLocs
//  -rotate each Point 90 degrees around [0,0]
// hint: rotate the point [1,2] clockwise around[0,0] and note
// how the x,y values change. There are 2 functions in the Point
// class that can be used to accomplish a rotation.
void Tetromino::rotateClockwise() {
	
	//int blockQuantity{ static_cast<int>(blockLocs.size()) };
	for (Point& p : blockLocs) {
		p.multiplyX(-1);
		p.swapXY();
	}
	/*for (int vectorElement = 0; vectorElement < blockQuantity; vectorElement++) {
			blockLocs[vectorElement].multiplyX(-1);
			blockLocs[vectorElement].swapXY();
	}*/
	
}

//print a grid to display the current shape
// to do this: print out a “grid”of text to represent a co-ordinate
// system.  Start at top left [-3,3] go to bottom right [3,-3]
// (use nested for loops)
//  for each [x,y]point, loop through the blockLocs and if the point exists
//in the list, print an 'x' instead of a '.'.You should end up with something
//like this: (results will vary depending on shape and rotation, eg: this
//  one shows a T shape rotated clockwise once)
//  .......
//  .......
//  ...x...
//  ..xx...
//  ...x...
//  .......
void Tetromino::printToConsole() const {
	bool blockExists{ false };
	int blockQuantity{ static_cast<int>(blockLocs.size()) };

	for (int y = 3; y >= -3; y--) {
		for (int x = -3; x <= 3; x++) {
			for (int vectorElement = 0; vectorElement < blockQuantity; vectorElement++) {
				if (blockLocs[vectorElement].getX() == x && blockLocs[vectorElement].getY() == y) {
					blockExists = true;
				}
			}
			if (blockExists) {
				std::cout << "x";
				blockExists = false;
			}
			else {
				std::cout << ".";
			}
		}
		std::cout << "\n";
	}
}

//Returns a random TetShape shape
TetShape Tetromino::getRandomShape() {
	return static_cast<TetShape>(rand() % static_cast<int>(TetShape::COUNT));
}
