#pragma once

#include <iostream>
#include <vector>
#include "mesh.hpp"
#include "car.hpp"

class GameBoard
{
public:
	GameBoard();
	void clean();

	std::vector<std::vector<int>> get_physical_board() const;
	int get_size() const;

	int get_cell(const int row, const int col) const;
	void set_cell(const int row, const int col, const int value);

	void set_size(const int new_size);
private:
	int size;
	std::vector<std::vector<int>> physical_board;
};
