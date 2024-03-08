#pragma once

#include "orientation.hpp"

class Car
{
public:
	Car(const int id, const int col, const int row, const int length, const Orientation orientation);

	int get_id() const;
	int get_col() const;
	int get_row() const;
	int get_length() const;
	Orientation get_orientation() const;

	void set_col(const int new_col);
	void set_row(const int new_row);
private:
	int id;
	int col;
	int row;
	int length;
	Orientation orientation;
};
