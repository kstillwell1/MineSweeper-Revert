#include "tile.h"
#include "mineSweeper.h"
#include "tileColors.h"
#include <iostream>
#include <queue>
#include <utility>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <conio.h>
#include <sstream>

MineSweeperGame::~MineSweeperGame()
{
	if (board.board != nullptr)
	{
		for (int i = 0; i < getNumRows(); i++)
		{
			Pool.free(board.board[i]);
		}
		delete[] board.board;
	}
}

void MineSweeperGame::setNumRows(int rows)
{
	currNumRows = rows;
}

int MineSweeperGame::getNumRows()
{
	return currNumRows;
}

void MineSweeperGame::setNumCols(int cols)
{
	currNumCols = cols;
}

int MineSweeperGame::getNumCols()
{
	return currNumCols;
}

void MineSweeperGame::setNumMines(int mineCount)
{
	currNumMines = mineCount;
}

int MineSweeperGame::getNumMines()
{
	return currNumMines;
}

std::pair<int, int> MineSweeperGame::getCurrentCoords()
{
	for (int row = 0; row < currNumRows; row++)
	{
		for (int col = 0; col < currNumCols; col++)
		{
			if (board.board[row][col].isCursorOnTile == true)
			{
				return { row, col };
			}
		}
	}
}

void MineSweeperGame::allocateBoard(int row, int col, int mines)
{
	board.board = new Tile* [row];

	for (int i = 0; i < row; i++)
	{
		board.board[i] = Pool.allocate(col);
	}

	setMines(mines);
}

void MineSweeperGame::setMines(int mines)
{
	int currSetMines = 0;
	int row = 0, col = 0;
	int maxRow = getNumRows();
	int maxCol = getNumCols();

	while (currSetMines != mines)
	{
		row = std::rand() % maxRow;
		col = std::rand() % maxCol;

		if (board.board[row][col].mine == false)
		{
			board.board[row][col].mine = true;
			board.board[row][col].actionedTile = true;
			currSetMines++;
		}
	}
}

void MineSweeperGame::userDifficultySelection()
{
	int answer;
	std::cout << "Welcome to Mine Sweeper, Please enter the number of the difficulty you would like to play: " << std::endl;
	std::cout << "1. EASY\n";
	std::cout << "2. MEDIUM\n";
	std::cout << "3. HARD\n";

	std::cin >> answer;

	switch (answer)
	{
		case (1):
		{
			setDifficulty(9, 9, 10); //easy difficulty
			break;
		}
		case (2):
		{
			setDifficulty(16, 16, 40); //medium difficulty
			break;
		}
		case (3):
		{
			setDifficulty(25, 25, 99); //hard difficulty
			break;
		}
		default:
		{
			std::cout << "Invalid Selection";
			userDifficultySelection();
		}
	}
}

void MineSweeperGame::setDifficulty(int rows, int cols, int mines)
{
	if (rows == currNumRows)
	{
		resetBoard(board.board, currNumRows, currNumCols);
	}
	else
	{
		if (board.board != nullptr)
		{
			for (int i = 0; i < getNumRows(); i++)
			{
				Pool.free(board.board[i]);
			}
			delete[] board.board;
		}

		setNumRows(rows);
		setNumCols(cols);
		setNumMines(mines);
		allocateBoard(currNumRows, currNumCols, currNumMines);
	}
}

void MineSweeperGame::resetBoard(Tile** board, int rows, int cols)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			board[row][col] = Tile();
		}
	}
}

std::string MineSweeperGame::setTileColor(int row, int col)
{
	int temp = checkAdjacentTiles(row, col);

	switch (temp)
	{
		case (1):
		{
			return GREEN;
			break;
		}
		case (2):
		{
			return BRIGHT_GREEN;
			break;
		}
		case (3):
		{
			return YELLOW;
			break;
		}
		case (4):
		{
			return BRIGHT_BLUE;
			break;
		}
		case (5):
		{
			return MAGENTA;
			break;
		}
		case (6):
		{
			return BRIGHT_RED;
			break;
		}
		case (7):
		{
			return RED;
			break;
		}
		case (8):
		{
			return CYAN;
			break;
		}
		default:
		{
			return RESET;
		}
	}
}

int MineSweeperGame::getFlagCount()
{
	int flagCount = 0;
	for (int row = 0; row < currNumRows; row++)
	{
		for (int col = 0; col < currNumCols; col++)
		{
			if (board.board[row][col].flag == true && board.board[row][col].revealed == true) continue;
			if (board.board[row][col].flag == true)
			{
				flagCount++;
			}
		}
	}
	return flagCount;
}

void MineSweeperGame::printBoard()
{
	std::ostringstream oss;

	oss << "\x1B[H"; // reposition cursor only, don't clear the whole screen

	oss << "Use Arrow Keys to move, Enter to reveal, and F to flag a tile" << "\x1B[K\n";
	oss << "\x1B[K\n"; // clear the blank line

	oss << "Flag Counter" << "(" << RED << getFlagCount() << RESET << ")" << "\x1B[K\n";

	oss << "\x1B[K\n";
	oss << "\x1B[K\n";

	for (int row = 0; row < currNumRows; ++row)
	{
		oss << "\t|";
		for (int col = 0; col < currNumCols; ++col)
		{
			if (board.board[row][col].isExploding == true)
			{
				if (board.board[row][col].userHitMine == true)
				{
					oss << BRIGHT_RED << "M" << RESET;
				}
				else
				{
					oss << BRIGHT_MAGENTA << "X" << RESET;
				}
			}
			else if (board.board[row][col].revealed == true)
			{
				if (checkAdjacentTiles(row, col) == 0 && board.board[row][col].isCursorOnTile == true)
				{
					oss << BG_BRIGHT_WHITE << " " << RESET;
				}
				else if (checkAdjacentTiles(row, col) == 0)
				{
					oss << " ";
				}
				else if (board.board[row][col].isCursorOnTile == true)
				{
					oss << BG_BRIGHT_WHITE << setTileColor(row, col) << board.board[row][col].adjacentMines << RESET;
				}
				else
				{
					oss << setTileColor(row, col) << board.board[row][col].adjacentMines << RESET;
				}
			}
			else if (gameEnd == true)
			{
				if (didUserWin == true && board.board[row][col].mine == true)
				{
					oss << RED << "F" << RESET;
				}
				else if (board.board[row][col].userHitMine == true)
				{
					oss << BRIGHT_RED << "M" << RESET;
				}
				else if (board.board[row][col].flag == true)
				{
					if (board.board[row][col].mine == false)
					{
						oss << WHITE << "F" << RESET;
					}
					else if (board.board[row][col].mine == true)
					{
						oss << RED << "F" << RESET;
					}
					else
					{
						oss << "M";
					}
				}
				else if (board.board[row][col].mine == true)
				{
					oss << "M";
				}
				else
				{
					oss << "-";
				}
			}
			else if (board.board[row][col].flag == true)
			{
				if (board.board[row][col].isCursorOnTile == true)
				{
					oss << BG_BRIGHT_WHITE << RED << "F" << RESET;
				}
				else
				{
					oss << RED << "F" << RESET;
				}
			}
			else if (board.board[row][col].isCursorOnTile == true)
			{
				oss << BG_BRIGHT_WHITE << BLACK << "*" << RESET;
			}
			else
			{
				oss << "-";
			}
			oss << "|";
		}
		oss << "\x1B[K" << "\n";
	}
	oss << "\x1B[J";

	std::cout << oss.str();
	std::cout.flush();
}

std::pair<int, int> MineSweeperGame::getHitMine()
{
	for (int row = 0; row < currNumRows; row++)
	{
		for (int col = 0; col < currNumCols; col++)
		{
			if (board.board[row][col].userHitMine == true)
			{
				return { row, col };
			}
		}
	}
}

bool MineSweeperGame::isTileInBounds(int row, int col)
{
	if (row < 0 || row >= currNumRows || col < 0 || col >= currNumCols)
	{
		return false;
	}
	return true;
}

void MineSweeperGame::BOOM()
{
	std::queue<std::pair<int, int>> tileBoomQueue;
	tileBoomQueue.push({ getHitMine().first, getHitMine().second });
	while (!tileBoomQueue.empty())
	{
		std::pair<int, int> current = tileBoomQueue.front();

		tileBoomQueue.pop();

		int r = current.first, c = current.second;

		if (isTileInBounds(r - 1, c) && board.board[r - 1][c].isExploding != true)
		{
			board.board[r - 1][c].isExploding = true;
			tileBoomQueue.push({ r - 1, c }); //up
		}
		if (isTileInBounds(r, c + 1) && board.board[r][c + 1].isExploding != true)
		{
			board.board[r][c + 1].isExploding = true;
			tileBoomQueue.push({ r, c + 1 }); //right
		}
		if (isTileInBounds(r + 1, c) && board.board[r + 1][c].isExploding != true)
		{
			board.board[r + 1][c].isExploding = true;
			tileBoomQueue.push({ r + 1, c }); //down
		}
		if (isTileInBounds(r, c - 1) && board.board[r][c - 1].isExploding != true)
		{
			board.board[r][c - 1].isExploding = true;
			tileBoomQueue.push({ r, c - 1 }); //left
		}

		printBoard();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	resetTilesFromBoom();
	printBoard();
}

void MineSweeperGame::resetTilesFromBoom()
{
	for (int row = 0; row < currNumRows; row++)
	{
		for (int col = 0; col < currNumCols; col++)
		{
			board.board[row][col].isExploding = false;
		}
	}
}

void MineSweeperGame::revealTiles(int row, int col)
{
	std::queue<std::pair<int, int>> tilesToReveal;
	tilesToReveal.push({ row, col });

	while (!tilesToReveal.empty())
	{
		std::pair<int, int> current = tilesToReveal.front();
		tilesToReveal.pop();

		int r = current.first, c = current.second;

		if (r < 0 || r >= currNumRows || c < 0 || c >= currNumCols) { continue; }
		if (board.board[r][c].revealed == true) { continue; }

		int currAdjacentMines = checkAdjacentTiles(r, c);

		if (currAdjacentMines == 0)
		{
			tilesToReveal.push({ r - 1, c - 1 });
			tilesToReveal.push({ r - 1, c });
			tilesToReveal.push({ r - 1, c + 1 });
			tilesToReveal.push({ r, c - 1 });
			tilesToReveal.push({ r, c + 1 });
			tilesToReveal.push({ r + 1, c - 1 });
			tilesToReveal.push({ r + 1, c });
			tilesToReveal.push({ r + 1, c + 1 });
		}
	}

	//recursion
	/*if (row < 0 || row >= currNumRows || col < 0 || col >= currNumCols) { return; }
	if (board.board[row][col].revealed == true) { return; }

	int adjacentMines = checkAdjacentTiles(row, col);

	if (adjacentMines == 0)
	{
		revealTiles(row - 1, col - 1);
		revealTiles(row - 1, col);
		revealTiles(row - 1, col + 1);
		revealTiles(row, col - 1);
		revealTiles(row, col + 1);
		revealTiles(row + 1, col - 1);
		revealTiles(row + 1, col);
		revealTiles(row + 1, col + 1);
	}*/
}

int MineSweeperGame::checkAdjacentTiles(int row, int col)
{
	int adjacentMineNum = 0;
	if (row < 0 || row >= currNumRows || col < 0 || col >= currNumCols) { return adjacentMineNum; }
	{
		board.board[row][col].revealed = true;
		board.board[row][col].actionedTile = true;

		if (row - 1 >= 0 && col - 1 >= 0 && board.board[row - 1][col - 1].mine == true) { adjacentMineNum++; }
		if (row - 1 >= 0 && board.board[row - 1][col].mine == true) { adjacentMineNum++; }
		if (row - 1 >= 0 && col + 1 < currNumCols && board.board[row - 1][col + 1].mine == true) { adjacentMineNum++; }
		if (col - 1 >= 0 && board.board[row][col - 1].mine == true) { adjacentMineNum++; }
		if (col + 1 < currNumCols && board.board[row][col + 1].mine == true) { adjacentMineNum++; }
		if (row + 1 < currNumRows && col - 1 >= 0 && board.board[row + 1][col - 1].mine == true) { adjacentMineNum++; }
		if (row + 1 < currNumRows && board.board[row + 1][col].mine == true) { adjacentMineNum++; }
		if (row + 1 < currNumRows && col + 1 < currNumCols && board.board[row + 1][col + 1].mine == true) { adjacentMineNum++; }
	}

	board.board[row][col].adjacentMines = adjacentMineNum;

	return board.board[row][col].adjacentMines;
}

std::pair<int, int> MineSweeperGame::cursorMovement()
{
	while (true)
	{
		int key = _getch();

		if (key == 13) // Enter
		{
			if (board.board[getCurrentCoords().first][getCurrentCoords().second].flag == true)
			{
				continue;
			}
			break;
		}

		if (key == 'f' || key == 'F')
		{
			if (board.board[getCurrentCoords().first][getCurrentCoords().second].revealed == false)
			{
				board.board[getCurrentCoords().first][getCurrentCoords().second].flag = !(board.board[getCurrentCoords().first][getCurrentCoords().second].flag);
				printBoard();
			}
			continue;
		}

		if (key == 0 || key == 224)
		{
			int input = _getch();

			int currRow = getCurrentCoords().first;
			int currCol = getCurrentCoords().second;

			switch (input)
			{
				case 72: // up
				{
					if (currRow - 1 >= 0)
					{
						board.board[currRow][currCol].isCursorOnTile = false;
						board.board[currRow - 1][currCol].isCursorOnTile = true;
					}
					break;
				}
				case 80: // down
				{
					if (currRow + 1 < currNumRows)
					{
						board.board[currRow][currCol].isCursorOnTile = false;
						board.board[currRow + 1][currCol].isCursorOnTile = true;
					}
					break;
				}
				case 75: // left
				{
					if (currCol - 1 >= 0)
					{
						board.board[currRow][currCol].isCursorOnTile = false;
						board.board[currRow][currCol - 1].isCursorOnTile = true;
					}
					break;
				}
				case 77: // right
				{
					if (currCol + 1 < currNumCols)
					{
						board.board[currRow][currCol].isCursorOnTile = false;
						board.board[currRow][currCol + 1].isCursorOnTile = true;
					}
					break;
				}
			}

			printBoard();
		}
	}

	return { getCurrentCoords().first, getCurrentCoords().second };
}

void MineSweeperGame::userTurn()
{
	std::pair<int, int> myCoords = cursorMovement();

	if (myCoords.first < 0 || myCoords.first >= currNumRows || myCoords.second < 0 || myCoords.second >= currNumCols || board.board[myCoords.first][myCoords.second].revealed == true)
	{
		std::cout << "Invalid input, please try again " << std::endl;
		userTurn();
	}
	else if (board.board[myCoords.first][myCoords.second].mine == true)
	{
		board.board[myCoords.first][myCoords.second].userHitMine = true;

		gameEnd = true;
		std::cout << "You hit a mine " << std::endl;
	}
	else
	{
		revealTiles(myCoords.first, myCoords.second);
	}
}

bool MineSweeperGame::checkWin()
{
	for (int row = 0; row < currNumRows; row++)
	{
		for (int col = 0; col < currNumCols; col++)
		{
			if (board.board[row][col].actionedTile == false) return false;
		}
	}
	didUserWin = true;
	return true;
}

void MineSweeperGame::postGameQuestions()
{
	char answer;
	std::cout << std::endl;
	std::cout << "Would you like to play again? (y/n): \n";
	std::cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		gameEnd = false;
		return;
	}
	else if (answer == 'n' || answer == 'N')
	{
		playAgain = true;
	}
	else
	{
		std::cout << "Answer not valid, try again \n";
		postGameQuestions();
	}
}

void MineSweeperGame::gameLoop()
{
	std::cout << sizeof(Tile*) << std::endl;
	std::cout << sizeof(Tile) << std::endl;
	srand(time(NULL));
	while (!playAgain)
	{
		userDifficultySelection();
		board.board[0][0].isCursorOnTile = true;
		while (gameEnd != true)
		{
			printBoard();
			userTurn();
			if (gameEnd == true)
			{
				printBoard();

				BOOM();
				break;
			}
			if (checkWin())
			{
				gameEnd = true;
				printBoard();
				std::cout << "Congrats on your win" << std::endl;
			}
		}
		postGameQuestions();
	}
}
