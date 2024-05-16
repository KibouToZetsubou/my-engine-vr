#include "rush_hour.hpp"

std::shared_ptr<PerspectiveCamera> RushHour::current_camera;
GameBoard RushHour::gameboard;
GameState RushHour::game_state;
int RushHour::selected_car = 0;

void RushHour::select_vehicle(const int car_id)
{
	if (RushHour::game_state.get_cars().size() < car_id) 
	{
		return; //To prevent crashes
	}

    std::shared_ptr<Mesh> prev_selected_car_mesh = std::dynamic_pointer_cast<Mesh>(MyEngine::find_object_by_name("Car00" + std::to_string(RushHour::selected_car)));
    if (prev_selected_car_mesh != nullptr)
    {
        prev_selected_car_mesh->get_material()->set_emission_color(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    std::shared_ptr<Mesh> selected_car_mesh = std::dynamic_pointer_cast<Mesh>(MyEngine::find_object_by_name("Car00" + std::to_string(car_id)));
    if (selected_car_mesh != nullptr)
    {
        selected_car_mesh->get_material()->set_emission_color(glm::vec3(0.5f, 0.5f, 0.5f));
    }

    RushHour::selected_car = car_id;

    std::cout << "Selected car: " << car_id << std::endl;

	RushHour::update_graphics();
}

void RushHour::move(const Direction direction)
{
	GameState new_game_state = RushHour::game_state;

	new_game_state.make_move(RushHour::selected_car, direction);

	if (new_game_state.is_valid())
	{
		RushHour::game_state = new_game_state;
		RushHour::update_graphics();
	}

	if (new_game_state.has_won())
	{
		std::cout << "You won!" << std::endl;
	}

	std::cout << std::endl;
	RushHour::game_state.print();
	std::cout << std::endl;
}

void RushHour::load_level(const int level)
{
	if (level == 1)
	{
		const Car car1(1, 1, 2, 2, Orientation::HORIZONTAL);
		const Car car2(2, 2, 3, 2, Orientation::VERTICAL);
		const Car car3(3, 3, 3, 2, Orientation::HORIZONTAL);
		const Car car4(4, 4, 0, 3, Orientation::VERTICAL);

		RushHour::game_state = GameState(RushHour::gameboard, { car1, car2, car3, car4 });
	}
	else if (level == 2)
	{
		const Car car1(1, 1, 2, 2, Orientation::HORIZONTAL);
		const Car car2(2, 3, 2, 2, Orientation::VERTICAL);
		const Car car3(3, 1, 3, 2, Orientation::VERTICAL);
		const Car car4(4, 3, 0, 3, Orientation::HORIZONTAL);
		const Car car5(5, 2, 4, 3, Orientation::HORIZONTAL);
		const Car car6(6, 5, 4, 2, Orientation::VERTICAL);
		const Car car7(7, 0, 0, 2, Orientation::VERTICAL);
		const Car car8(8, 2, 0, 2, Orientation::VERTICAL);

		RushHour::game_state = GameState(RushHour::gameboard, { car1, car2, car3, car4, car5, car6, car7, car8 });
	}
	else if (level == 3)
	{
		const Car car1(1, 2, 2, 2, Orientation::HORIZONTAL);
		const Car car2(2, 2, 1, 2, Orientation::HORIZONTAL);
		const Car car3(3, 1, 0, 2, Orientation::VERTICAL);
		const Car car4(4, 4, 0, 3, Orientation::VERTICAL);
		const Car car5(5, 1, 2, 3, Orientation::VERTICAL);
		const Car car6(6, 3, 4, 2, Orientation::VERTICAL);
		const Car car7(7, 0, 5, 3, Orientation::HORIZONTAL);
		const Car car8(8, 4, 4, 2, Orientation::HORIZONTAL);
		const Car car9(9, 5, 0, 2, Orientation::VERTICAL);

		RushHour::game_state = GameState(RushHour::gameboard, { car1, car2, car3, car4, car5, car6, car7, car8, car9 });
	}

	RushHour::game_state.print();
	RushHour::select_vehicle(1);
}

void RushHour::update_graphics()
{
	for (const auto& car : RushHour::game_state.get_cars())
	{
		std::shared_ptr<Node> car_node = std::dynamic_pointer_cast<Node>(MyEngine::find_object_by_name("Car00" + std::to_string(car.get_id())));

		car_node->set_position(glm::vec3(car.get_col() * 20.0f, 0.0f, car.get_row() * 20.0f));

		if (car.get_orientation() == Orientation::HORIZONTAL)
		{
			car_node->set_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
		}
		else if (car.get_orientation() == Orientation::HORIZONTAL)
		{
			car_node->set_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	Car current_car =  RushHour::game_state.get_cars()[RushHour::selected_car - 1];
	std::shared_ptr<Node> selected_car = std::dynamic_pointer_cast<Node>(MyEngine::find_object_by_name("Car00" + std::to_string(RushHour::selected_car)));
	glm::vec3 car_pos = selected_car->get_position();

	if (current_car.get_orientation() == Orientation::HORIZONTAL)
	{
		RushHour::current_camera->set_position(glm::vec3(car_pos.x - 100, car_pos.y + 75, car_pos.z - 50));
		RushHour::current_camera->set_rotation(glm::vec3(-75.0f, -90.0f, 25.0f));
	}
	else if (current_car.get_orientation() == Orientation::VERTICAL)
	{
		RushHour::current_camera->set_position(glm::vec3(car_pos.x - 50, car_pos.y + 75, car_pos.z));
		RushHour::current_camera->set_rotation(glm::vec3(-75.0f, 0.0f, 0.0f));
	}
}

void RushHour::set_perspective_camera(std::shared_ptr<PerspectiveCamera> new_camera) 
{
	RushHour::current_camera = new_camera;
}