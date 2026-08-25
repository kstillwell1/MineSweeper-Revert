#pragma once
#include "tile.h"
#include <string>

class MineSweeperGame
{
private:
	Board board;
	bool playAgain = false;
	bool gameEnd = false;
	bool didUserWin = false;
	int currNumRows = 0;
	int currNumCols = 0;
	int currNumMines = 0;

	const int easyRows = 9, easyCols = 9, easyMines = 10;
	const int mediumRows = 16, mediumCols = 16, mediumMines = 40;
	const int hardRows = 25, hardCols = 25, hardMines = 99;


	void allocateBoard(int row, int col, int mines);
	void setMines(int mines);
	void userDifficultySelection();
	int checkAdjacentTiles(int row, int col);
	void setDifficulty(int rows, int cols, int mines);
	void resetBoard(Tile** board, int rows, int cols);
	std::string setTileColor(int row, int col);
	void printBoard();
	void revealTiles(int row, int col);
	void userTurn();
	bool checkWin();
	std::pair<int, int> cursorMovement();
	void postGameQuestions();
	std::pair<int, int> getHitMine();
	void BOOM();
	void resetTilesFromBoom();
	bool isTileInBounds(int row, int col);

	void setNumRows(int rows);
	int getNumRows();
	void setNumCols(int Cols);
	int getNumCols();
	void setNumMines(int mineCount);
	int getNumMines();
	std::pair<int, int> getCurrentCoords();
	int getFlagCount();

public:
	~MineSweeperGame();
	void gameLoop();
};
