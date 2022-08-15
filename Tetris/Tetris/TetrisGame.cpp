#include "TetrisGame.h"
#include <assert.h>

// MEMBER FUNCTIONS

	// constructor
	//   initialize/assign variables
	//   reset() the game
	//   load font from file: fonts/RedOctober.ttf
	//   setup scoreText


TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, Point gameboardOffset, Point nextShapeOffset)
	: window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset){
	reset();
	// setup our font for drawing the score
	if(!scoreFont.loadFromFile("fonts/RedOctober.ttf")){
		assert(false && "Missing font: RedOctober.ttf");
	};
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(435, 325);
}

// Draw anything to do with the game,
//   includes the board, currentShape, nextShape, score
//   called every game loop
void TetrisGame::draw() {
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	drawGhostTetromino(ghostShape, gameboardOffset);
	window.draw(scoreText);
}

// Event and game loop processing
// handles keypress events (up, left, right, down, space)
void TetrisGame::onKeyPressed(sf::Event event) {
	if (event.key.code == sf::Keyboard::Up) {
		attemptRotate(currentShape);
		ghostShape = currentShape;
		drop(ghostShape);
	}
	else if (event.key.code == sf::Keyboard::Left) {
		attemptMove(currentShape, -1, 0);
		ghostShape = currentShape;
		drop(ghostShape);
	}
	else if (event.key.code == sf::Keyboard::Right) {
		attemptMove(currentShape, 1, 0);
		ghostShape = currentShape;
		drop(ghostShape);
	}
	else if (event.key.code == sf::Keyboard::Down) {
		if (!(attemptMove(currentShape, 0, 1))) {
			lock(currentShape);
			shapePlacedSinceLastGameLoop = true;
		}
	}
	else if (event.key.code == sf::Keyboard::Space) {
		drop(currentShape);
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}

// called every game loop to handle ticks & tetromino placement (locking)
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
	secondsSinceLastTick = secondsSinceLastTick + secondsSinceLastLoop;
	if (secondsSinceLastTick > secondsPerTick) {
		tick();
		secondsSinceLastTick = secondsSinceLastTick - secondsPerTick;
	}

	if (shapePlacedSinceLastGameLoop) {
		if (spawnNextShape()) {
			pickNextShape();
			int completedRows{ board.removeCompletedRows() };
			score = score + (completedRows * 100);
			updateScoreDisplay();
			determineSecondsPerTick();
		}
		else {
			reset();
		}
		shapePlacedSinceLastGameLoop = false;
	}
}

// A tick() forces the currentShape to move (if there were no tick,
// the currentShape would float in position forever). This should
// call attemptMove() on the currentShape.  If not successful, lock() 
// the currentShape (it can move no further), and record the fact that a
// shape was placed (using shapePlacedSinceLastGameLoop)
void TetrisGame::tick() {
	if (!(attemptMove(currentShape, 0, 1))) {
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}

// reset everything for a new game (use existing functions) 
//  - set the score to 0 and call updateScoreDisplay()
//  - call determineSecondsPerTick() to determine the tick rate.
//  - clear the gameboard,
//  - pick & spawn next shape
//  - pick next shape again (for the "on-deck" shape)
void TetrisGame::reset() {
	score = 0;
	updateScoreDisplay();
	determineSecondsPerTick();
	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();
}

// assign nextShape.setShape a new random shape  
void TetrisGame::pickNextShape() {
	nextShape.setShape(Tetromino::getRandomShape());
}

// copy the nextShape into the currentShape (through assignment)
//   position the currentShape to its spawn location.
//	 - return true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape() {
	currentShape = nextShape;
	ghostShape = currentShape;
	currentShape.setGridLoc(board.getSpawnLoc());
	ghostShape.setGridLoc(board.getSpawnLoc());
	drop(ghostShape);
	return isPositionLegal(currentShape);
}

// Test if a rotation is legal on the tetromino and if so, rotate it. 
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) rotate it (shape.rotateClockwise())
//	 3) test if temp rotation was legal (isPositionLegal()), 
//      if so - rotate the original tetromino.
//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape) {
	GridTetromino tetronimoToTest{ shape };
	tetronimoToTest.rotateClockwise();
	if (isPositionLegal(tetronimoToTest)) {
		shape.rotateClockwise();
		return true;
	}
	return false;
}

// test if a move is legal on the tetromino, if so, move it.
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) move it (temp.move())
//	 3) test if temp move was legal (isPositionLegal(),
//      if so - move the original.
//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {
	GridTetromino tetronimoToTest{ shape };
	tetronimoToTest.move(x, y);
	if (isPositionLegal(tetronimoToTest)) {
		shape.move(x, y);
		return true;
	}
	return false;
}

// drops the tetromino vertically as far as it can 
//   legally go.  Use attemptMove(). This can be done in 1 line.
void TetrisGame::drop(GridTetromino& shape) {
	while(attemptMove(shape, 0, 1));
}

// copy the contents (color) of the tetromino's mapped block locs to the grid.
//	 1) get current blockshape locs via tetromino.getBlockLocsMappedToGrid()
//	 2) copy the content (color) to the grid (via gameboard.setContent()) 
void TetrisGame::lock(const GridTetromino& shape) {
	std::vector<Point> blocks = shape.getBlockLocsMappedToGrid();
	bool valid = true;
	for (auto& block : blocks) {
		if (!(board.isValidPoint(block))) {
			valid = false;
		}
	}
	if (valid) {
		board.setContent(blocks, static_cast<int>(shape.getColor()));
	}
}

// Graphics methods ==============================================

// Draw a tetris block sprite on the canvas		
// The block position is specified in terms of 2 offsets: 
//    1) the top left (of the gameboard in pixels)
//    2) an x & y offset into the gameboard - in blocks (not pixels)
//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
//       to get the pixel offset.
//	 1) set the block color using blockSprite.setTextureRect()
//   2) set the block location using blockSprite.setPosition()   
//	 3) draw the block using window.draw()
//   For details/instructions on these 3 operations see:
//       www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
//       use member variables: window and blockSprite (assigned in constructor)
void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, TetColor color) {
	blockSprite.setColor(sf::Color{ 255, 255, 255, 255 });
	blockSprite.setTextureRect(sf::IntRect(static_cast<int>(color) * BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	blockSprite.setPosition(topLeft.getX() + (BLOCK_WIDTH * xOffset), topLeft.getY() + (BLOCK_HEIGHT * yOffset));//X and Y offsets represent the blocks measure, topLeft representa the most top-left of the screen.
	window.draw(blockSprite);
}

//   Iterate through each row & col, use drawBlock() to 
//   draw a block if it isn't empty.
void TetrisGame::drawGameboard() {
	for (int y = 0; y < Gameboard::MAX_Y; y++) {
		for (int x = 0; x < Gameboard::MAX_X; x++) {
			if (board.getContent(x, y) != Gameboard::EMPTY_BLOCK) {
				drawBlock(gameboardOffset, x, y, static_cast<TetColor>(board.getContent(x, y)));
			}
		}
	}
}

// Draw a tetromino on the window
//	 Iterate through each mapped loc & drawBlock() for each.
//   The topLeft determines a 'base point' from which to calculate block offsets
//      If the Tetromino is on the gameboard: use gameboardOffset
void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft) {
	std::vector<Point> pointVector{ tetromino.getBlockLocsMappedToGrid() };
	for (const auto& point : pointVector) {
		drawBlock(topLeft, point.getX(), point.getY(), static_cast<TetColor>(tetromino.getColor()));
	}

}

void TetrisGame::drawGhostBlock(const Point& topLeft, int xOffset, int yOffset, TetColor color) {
	blockSprite.setColor(sf::Color{ 255, 255, 255, 100 });
	blockSprite.setTextureRect(sf::IntRect(static_cast<int>(color) * BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	blockSprite.setPosition(topLeft.getX() + (BLOCK_WIDTH * xOffset), topLeft.getY() + (BLOCK_HEIGHT * yOffset));//X and Y offsets represent the blocks measure, topLeft representa the most top-left of the screen.
	window.draw(blockSprite);
}

void TetrisGame::drawGhostTetromino(const GridTetromino& tetromino, const Point& topLeft) {
	std::vector<Point> pointVector{ tetromino.getBlockLocsMappedToGrid() };
	for (const auto& point : pointVector) {
		drawGhostBlock(topLeft, point.getX(), point.getY(), static_cast<TetColor>(tetromino.getColor()));
	}

}

//enum class Difficulty {
//	EASY,
//	MEDIUM,
//	HARD
//};
// update the score display
// form a string "score: ##" to display the current score
// user scoreText.setString() to display it.
void TetrisGame::updateScoreDisplay() {
	std::string actualScore{"score:  " + std::to_string(score)};
	scoreText.setString(actualScore);
	//Difficulty difficulty = Difficulty::EASY;
	static bool mediumDifficulty = false;
	static bool hardDifficulty = false;
	static bool extremeDifficulty = false;

	if (score == 0) {
		mediumDifficulty = false;
		hardDifficulty = false;
		extremeDifficulty = false;
		secondsPerTick = .75;
	}
	if (score >= 100 && secondsPerTick > MIN_SECONDS_PER_TICK && !mediumDifficulty) {
		secondsPerTick = secondsPerTick - .15;
		mediumDifficulty = true;
	}
	if (score >= 1000 && secondsPerTick > MIN_SECONDS_PER_TICK && !hardDifficulty) {
		secondsPerTick = secondsPerTick - .15;
		hardDifficulty = true;
	}
	if (score >= 3000 && secondsPerTick > MIN_SECONDS_PER_TICK && !extremeDifficulty) {
		secondsPerTick = secondsPerTick - .15;
		extremeDifficulty = true;
	}
	std::cout << secondsPerTick << "\n";
}

// State & gameplay/logic methods ================================
// return true if shape is within borders (isShapeWithinBorders())
//	 and the shape's mapped board locs are empty.
//   Make use of Gameboard's areLocsEmpty() and pass it the shape's mapped locs.
bool TetrisGame::isPositionLegal(const GridTetromino& shape) const {
	//return isShapeWithinBorders(shape) && board.areLocsEmpty(shape.getBlockLocsMappedToGrid());
	if (isShapeWithinBorders(shape) && board.areLocsEmpty(shape.getBlockLocsMappedToGrid())) {
		return true;
	}

	return false;
}

// return true if the shape is within the left, right, and lower border of
//	 the grid, but *NOT* the top border. (return false otherwise)
//   * Ignore the upper border because we want shapes to be able to drop
//     in from the top of the gameboard.
//   All of a shape's blocks must be inside these 3 borders to return true
bool TetrisGame::isShapeWithinBorders(const GridTetromino& shape) const {

	for (auto block : shape.getBlockLocsMappedToGrid()) {
		if (Gameboard::MIN_INDEX > block.getX() || Gameboard::MAX_X <= block.getX() || block.getY() >= Gameboard::MAX_Y) {
			return false;
		}
	}

	return true;
}

// set secsPerTick 
//   - basic: use MAX_SECS_PER_TICK
//   - advanced: base it on score (higher score results in lower secsPerTick)
void TetrisGame::determineSecondsPerTick() {

}