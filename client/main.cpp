#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <chrono>

#include <my_engine.hpp>
#include <node.hpp>
#include <ovo_parser.hpp>
#include <perspective_camera.hpp>
#include <ortho_camera.hpp>

#include "rush_hour.hpp"
#include "direction.hpp"

std::shared_ptr<OrthoCamera> saved_camera_ortho = nullptr;
std::shared_ptr<PerspectiveCamera> saved_camera_perspec = nullptr;
bool perspective_camera_is_used = false;

void start_level(int level_id) 
{
    std::shared_ptr<Object> scene_root;
    if(level_id == 1)
    {
        scene_root = OVOParser::from_file("./Level1.ovo");
        MyEngine::set_sky_color(0.15f, 0.15f, 0.15f);
        MyEngine::set_scene(scene_root);

        std::shared_ptr<Mesh> box_1 = std::dynamic_pointer_cast<Mesh>(MyEngine::find_object_by_name("Box001"));
        box_1->set_cast_shadows(false);
        std::shared_ptr<Mesh> box_2 = std::dynamic_pointer_cast<Mesh>(MyEngine::find_object_by_name("Box002"));
        box_2->set_cast_shadows(false);
        std::shared_ptr<Mesh> cyllinder = std::dynamic_pointer_cast<Mesh>(MyEngine::find_object_by_name("Cylinder001"));
        cyllinder->set_cast_shadows(false);

    }
    else if (level_id == 2)
    {
        scene_root = OVOParser::from_file("./Level2.ovo");
        MyEngine::set_sky_color(0.81f, 0.53f, 0.22f);
        MyEngine::set_scene(scene_root);
    }
    else if (level_id == 3) 
    {
        scene_root = OVOParser::from_file("./Level3.ovo");
        MyEngine::set_sky_color(0.53f, 0.81f, 0.92f);
        MyEngine::set_scene(scene_root);
    }

    std::shared_ptr<Mesh> plane = std::dynamic_pointer_cast<Mesh>(MyEngine::find_object_by_name("Plane001"));
    plane->set_cast_shadows(false);

    if (saved_camera_ortho == nullptr || saved_camera_perspec == nullptr)
    {
        std::shared_ptr<OrthoCamera> camera_1 = std::make_shared<OrthoCamera>();
        camera_1->set_zoom(350.0f);
        camera_1->set_name("Camera");
        camera_1->set_position(glm::vec3(0.0f, 150.0f, 0.0f));
        camera_1->set_rotation(glm::vec3(-90.0f, 0.0f, 0.0f));

        std::shared_ptr<PerspectiveCamera> camera_2 = std::make_shared<PerspectiveCamera>();
        camera_2->set_name("Camera 2");
        camera_2->set_position(glm::vec3(0.0f, 123.0f, 55.0f));
        camera_2->set_rotation(glm::vec3(-65.0f, 0.0f, 0.0f));

        saved_camera_ortho = camera_1;
        saved_camera_perspec = camera_2;
    }

    scene_root->add_child(saved_camera_ortho);
    scene_root->add_child(saved_camera_perspec);
    MyEngine::set_active_camera(saved_camera_ortho);

    RushHour::set_perspective_camera(saved_camera_perspec);

    RushHour::load_level(level_id);

    std::stringstream text;
    text << "Car:\n";
    text << "[w & s] - Move vertically\n";
    text << "[a & d] - Move horizzontally\n";
    text << "[1 - 9] - Select car\n";
    text << "Camera:\n";
    text << "[q & e] - Rotate plane (only ortho)\n";
    text << "[r & t] - Camera elevation (only ortho)\n";
    text << "[u & i] - Camera switching\n";
    text << "Levels:\n";
    text << "[b] - Level 1\n";
    text << "[n] - Level 2\n";
    text << "[m] - Level 3";
    MyEngine::set_screen_text(text.str());
}

int main(int argc, char* argv[])
{
    //Engine setup
    MyEngine::init("Rush Hour", 800, 600);

    MyEngine::set_keyboard_callback([](const unsigned char key, const int mouse_x, const int mouse_y)
    {
        if (key == 27) // ESC
        {
            MyEngine::stop();
        }

        if (!perspective_camera_is_used)
        {
            if (key == 'r') // Camera down
            {
                std::shared_ptr<OrthoCamera> camera = std::dynamic_pointer_cast<OrthoCamera>(MyEngine::find_object_by_name("Camera"));

                //camera->set_position(glm::vec3(camera->get_position().x, camera->get_position().y - 5.0f, camera->get_position().z));

                camera->set_zoom(camera->get_zoom() - 5.0f);
            }

            if (key == 't') // Camera up
            {
                std::shared_ptr<OrthoCamera> camera = std::dynamic_pointer_cast<OrthoCamera>(MyEngine::find_object_by_name("Camera"));

                //camera->set_position(glm::vec3(camera->get_position().x, camera->get_position().y + 5.0f, camera->get_position().z));

                camera->set_zoom(camera->get_zoom() + 5.0f);
            }

            if (key == 'e') // Scene left
            {
                std::shared_ptr<Node> root = std::dynamic_pointer_cast<Node>(MyEngine::find_object_by_name("[root]"));

                root->set_rotation(glm::vec3(root->get_rotation().x, root->get_rotation().y - 5.0f, root->get_rotation().z));
            }

            if (key == 'q') // Scene right
            {
                std::shared_ptr<Node> root = std::dynamic_pointer_cast<Node>(MyEngine::find_object_by_name("[root]"));

                root->set_rotation(glm::vec3(root->get_rotation().x, root->get_rotation().y + 5.0f, root->get_rotation().z));
            }
        }


        if (key == 'u') //Switch camera 1
        {
            MyEngine::set_active_camera(saved_camera_ortho);
            perspective_camera_is_used = false;
        }

        if (key == 'i') //Switch camera 2
        {
            MyEngine::set_active_camera(saved_camera_perspec);
            perspective_camera_is_used = true;

            std::shared_ptr<Node> root = std::dynamic_pointer_cast<Node>(MyEngine::find_object_by_name("[root]"));
            root->set_rotation(glm::vec3(0.0f, 0.0f, 0.0f));

            std::shared_ptr<OrthoCamera> camera = std::dynamic_pointer_cast<OrthoCamera>(MyEngine::find_object_by_name("Camera"));
            camera->set_zoom(350.0f);
        }

        if (key == 'b')
        {
            start_level(1);
        }

        if (key == 'n')
        {
            start_level(2);
        }

        if (key == 'm')
        {
            start_level(3);
        }

        if (key >= '1' && key <= '9')
        {
            RushHour::select_vehicle(static_cast<int>(key) - 48);
        }

        switch (key)
        {
            case 'w':
            {
                RushHour::move(Direction::UP);
                break;
            }
            case 'a':
            {
                RushHour::move(Direction::LEFT);
                break;
            }
            case 's':
            {
                RushHour::move(Direction::DOWN);
                break;
            }
            case 'd':
            {
                RushHour::move(Direction::RIGHT);
                break;
            }
        }
    });

    start_level(1);

    while (MyEngine::is_running())
    {
        MyEngine::update();

        MyEngine::clear_screen();
        MyEngine::render();
        MyEngine::swap_buffers();

        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    MyEngine::quit();

    return 0;
}
