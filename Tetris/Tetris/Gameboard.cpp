#include "Point.h"
#include "Gameboard.h"
#include <assert.h>

// constructor - empty() the grid
Gameboard::Gameboard() {
	empty();
}

// return the content at a given point (assert the point is valid)
int Gameboard::getContent(const Point& pt) const {
	int row{ pt.getY() };
	int column{ pt.getX() };

	assert(isValidPoint(pt));
	return grid[row][column];
}

// return the content at an x,y grid loc (assert the point is valid)
int Gameboard::getContent(int x, int y) const {
	assert(isValidPoint(x, y));
	return grid[y][x];
}

//returns the spawnLoc Point
Point Gameboard::getSpawnLoc() const {
	return spawnLoc;
}

// set the content at a given point (only if the point is valid)
void Gameboard::setContent(const Point& pt, int content) {
	assert(isValidPoint(pt));
	grid[pt.getY()][pt.getX()] = content;
}

// set the content at an x,y position (only if the point is valid)
void Gameboard::setContent(int x, int y, int content) {
	assert(isValidPoint(x, y));
	grid[y][x] = content;
}

// set the content for a set of points (only if the points are valid)
void Gameboard::setContent(const std::vector<Point>& locs, int content) {
	for (const auto& pt : locs) {
		int row{ pt.getY() };
		int column{ pt.getX() };
		assert(isValidPoint(pt));
		grid[row][column] = content;
	}
}

// return true if the content at ALL (valid) points is empty
//   *** IMPORTANT NOTE: invalid x,y values can be passed to this method.
//   Invalid meaning outside the bounds of the grid.
//   * ONLY TEST VALID POINTS (disregard the others - and ensure you
//   don't use them to index into the grid).  Use isValidPoint() 
//   Testing invalid points would likely result in an out of bounds
//     error or segmentation fault!
//   If none of the points are valid, return true
bool Gameboard::areLocsEmpty(std::vector<Point> locs) const {
	for (auto& pt : locs) {
		if (isValidPoint(pt) && getContent(pt) != EMPTY_BLOCK) { //We can return false when at least one block is empty "we don´t need to test all 
			return false;										//since we need everything to have the value of EMPTY_BLOCK"
		}
	}
	return true; //if everything is true or invalid points
}

// removes all completed rows from the board
//   use getCompletedRowIndices() and removeRows() 
//   return the # of completed rows removed
int Gameboard::removeCompletedRows() {
	std::vector<int> completedRow{ getCompletedRowIndices() };

	for (int row : completedRow) {
		removeRow(row);
	}
	return completedRow.size(); //Number of rows removed
}

// fill the board with EMPTY_BLOCK 
//   (iterate through each rowIndex and fillRow() with EMPTY_BLOCK))
void Gameboard::empty() {
	for (int rowIndex = MIN_INDEX; rowIndex < MAX_Y; rowIndex++) {
		fillRow(rowIndex, EMPTY_BLOCK);
	}
}

// print the grid contents to the console (for debugging purposes)
//   use std::setw(2) to space the contents out (#include <iomanip>).
void Gameboard::printToConsole() const {
	for (int y = MIN_INDEX; y < MAX_Y; y++) {
		for (int x = MIN_INDEX; x < MAX_X; x++) {
			std::cout << std::setw(2) << grid[y][x];
		}
		std::cout << "\n";
	}
}

// return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
bool Gameboard::isRowCompleted(int rowIndex) const {
	for (int columnIndex = MIN_INDEX; columnIndex < MAX_X; columnIndex++) {
		if (grid[rowIndex][columnIndex] == EMPTY_BLOCK) {
			return false;
		} 
	}
	return true;
}

// scan the board for completed rows.
//   Iterate through grid rows and use isRowCompleted(rowIndex)
//   return a vector of completed row indices.
std::vector<int> Gameboard::getCompletedRowIndices() const {
	std::vector<int> completedRows;
	for (int row = MIN_INDEX; row < MAX_Y; row++) {
		if (isRowCompleted(row)) {
			completedRows.push_back(row);
		}
	}
	return completedRows;
}

// In gameplay, when a full row is completed (filled with content)
// it gets "removed".  To be exact, the row itself is not removed
// but the content from the row above it is copied into it.
// This continues all the way up the grid until the first row
// is copied into the second row.  Finally, the first row is 
// filled with EMPTY_BLOCK
// given a row index:
//   1) Starting at rowIndex, copy each row above the removed
//     row "one-row-downwards" in the grid.
//     (loop from y=rowIndex down to 0, and copyRowIntoRow(y-1, y)).
//   2) call fillRow() on the first row (and place EMPTY_BLOCKs in it).
void Gameboard::removeRow(int rowIndex) {
	for (int y = rowIndex; y > MIN_INDEX; y--) {
		copyRowIntoRow(y - 1, y);
	}
	fillRow(MIN_INDEX, EMPTY_BLOCK);
}

// given a vector of row indices, remove them 
//   (iterate through the vector and and call removeRow()
//   on each row index). 
void Gameboard::removeRows(const std::vector<int>& rowIndices) {
	for (int row : rowIndices) {
		removeRow(row);
	}
}

// fill a given grid row with specified content
void Gameboard::fillRow(int rowIndex, int content){
	for (int columnIndex = MIN_INDEX; columnIndex < MAX_X; columnIndex++) {
		grid[rowIndex][columnIndex] = content;
	}
}

// copy a source row's contents into a target row.
void Gameboard::copyRowIntoRow(int sourceRowIndex, int targetRowIndex) {
	for (int column = MIN_INDEX; column < MAX_X; column++) {
		grid[targetRowIndex][column] = grid[sourceRowIndex][column];
	}
}

// return true if the point is on the grid, false otherwise
bool Gameboard::isValidPoint(const Point& p) const {
	int row{ p.getY() };
	int column{ p.getX() };

	return (row >= MIN_INDEX && row < MAX_Y&& column >= MIN_INDEX && column < MAX_X);
}

// return true if the x,y location is on the grid, false otherwise
bool Gameboard::isValidPoint(int x, int y) const {
	return (y >= MIN_INDEX && y < MAX_Y && x >= MIN_INDEX && x < MAX_X);
}

//// return true if the point is on the grid, false otherwise
//bool Gameboard::isValidPoint(const Point& p) const {
//	int row{ p.getY() };
//	int column{ p.getX() };
//
//	return grid[row][column] != EMPTY_BLOCK;
//}
//
//// return true if the x,y location is on the grid, false otherwise
//bool Gameboard::isValidPoint(int x, int y) const {
//	return grid[y][x] != EMPTY_BLOCK;
//}