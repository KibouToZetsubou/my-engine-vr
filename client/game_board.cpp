#include "game_board.hpp"

GameBoard::GameBoard()
{
	this->set_size(6); //Standard rush-hour size

	std::vector<int> row(this->get_size(), 0);
	std::vector<std::vector<int>> temp_board(this->get_size(), row);

	for (int i = 0; i < this->size; i++)
	{
		for (int j = 0; j < this->size; j++)
		{
			temp_board[i][j] = 0;
		}
	}

	this->physical_board = temp_board;
}

void GameBoard::clean()
{
	for (int i = 0; i < this->size; i++)
	{
		for (int j = 0; j < this->size; j++)
		{
			this->physical_board[i][j] = 0;
		}
	}
}

std::vector<std::vector<int>> GameBoard::get_physical_board() const
{
	return this->physical_board;
}

int GameBoard::get_size() const
{
	return this->size;
}

int GameBoard::get_cell(const int row, const int col) const
{
	return this->physical_board[row][col];
}

void GameBoard::set_cell(const int row, const int col, const int value)
{
	this->physical_board[row][col] = value;
}

void GameBoard::set_size(const int new_size)
{
	this->size = new_size;
}

/*

Board of integers, all 0.
Cells occupied by vehicle ids.
Maybe, the red car is always the first, the horizontal 1's.
If [5][2] == 01 then WIN.

Level1:
00 00 00 00 04 00
00 00 00 00 04 00
00 01 01 00 04 00
00 00 02 03 03 00
00 00 02 00 00 00
00 00 00 00 00 00

Level2:
07 00 08 04 04 04
07 00 08 00 00 00
00 01 01 02 00 00
00 03 00 02 00 00
00 03 05 05 05 06
00 00 00 00 00 06

Level3:
00 03 00 00 04 09
00 03 02 02 04 09
00 05 01 01 04 00
00 05 00 00 00 00
00 05 00 06 08 08
07 07 07 06 00 00

*/
