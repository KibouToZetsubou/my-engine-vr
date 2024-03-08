#pragma once

#include "game_board.hpp"
#include "game_state.hpp"
#include "direction.hpp"

#include <my_engine.hpp>
#include <perspective_camera.hpp> 

class RushHour
{
public:
	static void select_vehicle(const int car_id);
	static void move(const Direction direction);
	static void load_level(const int level);
	static void set_perspective_camera(std::shared_ptr<PerspectiveCamera> new_camera);
private:
	static void update_graphics();

	static std::shared_ptr<PerspectiveCamera> current_camera;
	static GameBoard gameboard;
	static GameState game_state;
	static int selected_car;
};
