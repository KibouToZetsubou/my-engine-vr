#pragma once

#include <iostream>
#include <vector>
#include "game_board.hpp"
#include "direction.hpp"
#include "car.hpp"

class GameState
{
public:
    GameState();
	GameState(const GameBoard new_gameboard, const std::vector<Car> new_vehicles);

	std::vector<Car> get_cars() const;

	bool is_valid() const;
	bool has_won() const;

	void make_move(const int car_id, const Direction direction);

	void print() const;
private:
	bool check_validity();

	bool is_valid_flag;

	GameBoard game_board;

	std::vector<Car> cars;
};
