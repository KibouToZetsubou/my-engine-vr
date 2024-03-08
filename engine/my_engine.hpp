#pragma once

#define LIB_NAME      "MyEngine"
#define LIB_VERSION   10

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <sstream>

#include <glm/glm.hpp>

#include "camera.hpp"
#include "common.hpp"
#include "material.hpp"
#include "object.hpp"

/**
 * The main class of the engine.
 *
 * You must call the init function before being able to call any other function.
 *
 * You must call quit at the end do free up the used memory.
 */
class LIB_API MyEngine
{
public:
    static void init(const std::string window_title, const int window_width, const int window_height);

    static void set_keyboard_callback(void (*new_keyboard_callback) (const unsigned char key, const int mouse_x, const int mouse_y));

    static void set_sky_color(const float red, const float green, const float blue);

    static bool is_running();

    static void render();

    static void timerCallback(int value);

    static void update();

    static void clear_screen();

    static void swap_buffers();

    static void stop();

    static void quit();

    static std::shared_ptr<Object> get_scene();

    static void set_scene(const std::shared_ptr<Object> new_scene);

    static void set_active_camera(const std::shared_ptr<Camera> new_active_camera);

    static std::shared_ptr<Object> find_object_by_name(const std::string name_to_find);

    static void set_screen_text(const std::string new_text);
private:
    static void resize_callback(const int width, const int height);

    static std::vector<std::pair<std::shared_ptr<Object>, glm::mat4>> build_render_list(const std::shared_ptr<Object> scene_root, const glm::mat4 parent_world_matrix);

    static std::shared_ptr<Object> find_object_by_name(const std::string name_to_find, const std::shared_ptr<Object> root);

    static bool is_initialized_flag;
    static bool is_running_flag;
    static int window_id;

    static int window_width;
    static int window_height;

    static std::shared_ptr<Object> scene;
    static std::shared_ptr<Camera> active_camera;
    static std::shared_ptr<Material> shadow_material;
    static std::string screen_text;
    static int frames;
    static float fps;
};
