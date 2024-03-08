#include "game_state.hpp"

#include <string>
#include <vector>

#include "direction.hpp"

GameState::GameState()
{
    this->is_valid_flag = false;
    this->game_board = GameBoard();
    this->cars = {};
}

GameState::GameState(const GameBoard game_board, const std::vector<Car> cars)
{
	this->game_board = game_board;
	this->cars = cars;
	this->is_valid_flag = check_validity();
}

bool GameState::is_valid() const
{
	return this->is_valid_flag;
}

bool GameState::has_won() const
{
    return (this->game_board.get_physical_board()[2][5] == 1);
}

bool GameState::check_validity()
{
	this->game_board.clean();

	for (const auto& car : this->cars)
	{
		for (int i = 0; i < car.get_length(); i++)
		{
			const int row = car.get_row() + (i * (car.get_orientation() == Orientation::VERTICAL));
			const int col = car.get_col() + (i * (car.get_orientation() == Orientation::HORIZONTAL));

			// Are we out of bounds vertically ?
			if (row < 0 || row >= this->game_board.get_size())
			{
				return false;
			}

			// Are we out of bounds horizontally ?
			if (col < 0 || col >= this->game_board.get_size())
			{
				return false;
			}

			// Is this cell already full ?
			if (this->game_board.get_physical_board()[row][col] != 0)
			{
				return false;
			}

			this->game_board.set_cell(row, col, car.get_id());
		}
	}

	return true;
}

void GameState::print() const
{
    if (!this->is_valid_flag)
    {
        std::cout << "Invalid state!" << std::endl;

        return;
    }

    for (int row = 0; row < this->game_board.get_size(); ++row)
    {
        for (int col = 0; col < this->game_board.get_size(); ++col)
        {
            const int cell_value = this->game_board.get_cell(row, col);

            std::cout << " " << (cell_value < 10 ? "0" : "") << cell_value;
        }

        std::cout << std::endl;
    }
}

void GameState::make_move(int car_id, Direction direction)
{
    const int car_index = car_id - 1;

	const Orientation current_car_orientation = this->cars[car_index].get_orientation();

	if (current_car_orientation == Orientation::HORIZONTAL)
	{
		if (direction == Direction::UP || direction == Direction::DOWN)
		{
			this->is_valid_flag = false;

			return;
		}
	}
	else
	{
		if (direction == Direction::LEFT || direction == Direction::RIGHT)
		{
			this->is_valid_flag = false;

			return;
		}
	}

	switch (direction)
	{
		case Direction::UP:
			this->cars[car_index].set_row(this->cars[car_index].get_row() - 1);
			break;
		case Direction::LEFT:
			this->cars[car_index].set_col(this->cars[car_index].get_col() - 1);
			break;
		case Direction::DOWN:
			this->cars[car_index].set_row(this->cars[car_index].get_row() + 1);
			break;
		case Direction::RIGHT:
			this->cars[car_index].set_col(this->cars[car_index].get_col() + 1);
			break;
	}

	this->is_valid_flag = check_validity();
}

std::vector<Car> GameState::get_cars() const
{
	return this->cars;
}
