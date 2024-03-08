#include "car.hpp"

#include "orientation.hpp"

Car::Car(const int id, const int col, const int row, const int length, const Orientation orientation)
{
    this->id = id;
	this->set_col(col);
	this->set_row(row);
	this->length = length;
	this->orientation = orientation;
}

int Car::get_id() const
{
	return this->id;
}

int Car::get_col() const
{
	return this->col;
}

int Car::get_row() const
{
	return this->row;
}

int Car::get_length() const
{
	return this->length;
}

Orientation Car::get_orientation() const
{
	return this->orientation;
}

void Car::set_col(int new_col)
{
	this->col = new_col;
}

void Car::set_row(int new_row)
{
	this->row = new_row;
}
