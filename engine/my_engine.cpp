#include "my_engine.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#ifdef _linux
#include <unistd.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <FreeImage.h>

#include "camera.hpp"
#include "common.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "point_light.hpp"

bool MyEngine::is_initialized_flag = false;
bool MyEngine::is_running_flag = false;
int MyEngine::window_id = 0;
std::shared_ptr<Object> MyEngine::scene;
std::shared_ptr<Camera> MyEngine::active_camera;
std::string MyEngine::screen_text;
int MyEngine::window_width = 0;
int MyEngine::window_height = 0;
std::shared_ptr<Material> MyEngine::shadow_material = std::make_shared<Material>();

// Frames:
int MyEngine::frames = 0;
float MyEngine::fps = 0.0f;

#ifdef _WINDOWS
#include <Windows.h>
int APIENTRY DllMain(HANDLE instDLL, DWORD reason, LPVOID _reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return true;
}
#else
    #define __stdcall
#endif


/**
 * Initializes the engine and creates a new window.
 *
 * This must be called before being able to call any other function.
 *
 * @param window_title The title of the window.
 * @param window_width The width of the window in pixels.
 * @param window_height The height of the window in pixels.
 */
void LIB_API MyEngine::init(const std::string window_title, const int window_width, const int window_height)
{
    if (MyEngine::is_initialized_flag)
    {
        ERROR("Engine has already been initialized.");

        return;
    }

    // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitContextVersion(4, 4);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);

    int argc = 0;
    glutInit(&argc, NULL);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    MyEngine::window_id = glutCreateWindow(window_title.c_str());
    glutReshapeWindow(window_width, window_height);
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        std::cout << "[ERROR] " << glewGetErrorString(error) << std::endl;
        return;
    }
    else if (GLEW_VERSION_4_4)
    {
        std::cout << "Driver supports OpenGL 4.4\n" << std::endl;
    }
    else
    {
        std::cout << "[ERROR] OpenGL 4.4 not supported\n" << std::endl;
        return;
    }

    // TODO: Figure out why this is always true, even in release mode.
    #ifdef _DEBUG
    // Register OpenGL debug callback
    glDebugMessageCallback((GLDEBUGPROC) debug_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    #endif

    // Log context properties:
    std::cout << "OpenGL properties:" << std::endl;
    std::cout << "   Vendor . . . :  " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "   Driver . . . :  " << glGetString(GL_RENDERER) << std::endl;

    int oglVersion[2];
    glGetIntegerv(GL_MAJOR_VERSION, &oglVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &oglVersion[1]);
    std::cout << "   Version  . . :  " << glGetString(GL_VERSION) << " [" << oglVersion[0] << "." << oglVersion[1] << "]" << std::endl;

    int oglContextProfile;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &oglContextProfile);
    if (oglContextProfile & GL_CONTEXT_CORE_PROFILE_BIT)
        std::cout << "                :  " << "Core profile" << std::endl;
    if (oglContextProfile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
        std::cout << "                :  " << "Compatibility profile" << std::endl;

    int oglContextFlags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &oglContextFlags);
    if (oglContextFlags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
        std::cout << "                :  " << "Forward compatible" << std::endl;
    if (oglContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
        std::cout << "                :  " << "Debug flag" << std::endl;
    if (oglContextFlags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT)
        std::cout << "                :  " << "Robust access flag" << std::endl;
    if (oglContextFlags & GL_CONTEXT_FLAG_NO_ERROR_BIT)
        std::cout << "                :  " << "No error flag" << std::endl;

    std::cout << "   GLSL . . . . :  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << std::endl;

    // Setup callbacks
    glutDisplayFunc(render);
    glutReshapeFunc(resize_callback);
    glutTimerFunc(1000, timer_callback, 0);

    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_NORMALIZE);
    //glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_TEXTURE_2D);

    // Configure lighting
    const glm::vec4 ambient(0.2f, 0.2f, 0.2f, 1.0f);
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(ambient));
    //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

    // Initialize FreeImage
    FreeImage_Initialise();

    // Configure the material used to draw shadows.
    MyEngine::shadow_material->set_ambient_color(glm::vec3(0.0f, 0.0f, 0.0f));
    MyEngine::shadow_material->set_diffuse_color(glm::vec3(0.0f, 0.0f, 0.0f));
    MyEngine::shadow_material->set_specular_color(glm::vec3(0.0f, 0.0f, 0.0f));
    MyEngine::shadow_material->set_shininess(0.0f);

    // Start the engine
    MyEngine::is_initialized_flag = true;
    MyEngine::is_running_flag = true;
}

/**
 * Debug message callback for OpenGL. See https://www.opengl.org/wiki/Debug_Output
 */
void LIB_API __stdcall MyEngine::debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
    std::cout << "OpenGL says: \"" << std::string(message) << "\"" << std::endl;
    std::cout << "  - Source: \"" << source << "\"" << std::endl;
}

/**
 * Sets a custom callback to be called when the user presses a key.
 *
 * @param new_keyboard_callback The new function callback.
 */
void LIB_API MyEngine::set_keyboard_callback(void (*new_keyboard_callback) (const unsigned char key, const int mouse_x, const int mouse_y))
{
    glutKeyboardFunc(new_keyboard_callback);
}

/**
 * Sets the background color of the scene.
 *
 * @param red The amount of red (0.0f - 1.0f).
 * @param green The amount of green (0.0f - 1.0f).
 * @param blue The amount of blue (0.0f - 1.0f).
 */
void LIB_API MyEngine::set_sky_color(const float red, const float green, const float blue)
{
    glClearColor(red, green, blue, 1.0f);
}

/**
 * Renders the scene.
 *
 * If the scene is nullptr, the function returns immediately.
 */
void LIB_API MyEngine::render()
{
    if (MyEngine::scene == nullptr || MyEngine::active_camera == nullptr)
    {
        return;
    }

    MyEngine::active_camera->set_window_size(MyEngine::window_width, MyEngine::window_height);

    // Disable all lights
    /*int max_lights;
    glGetIntegerv(GL_MAX_LIGHTS, &max_lights);
    for (int i = 0; i < max_lights; ++i)
    {
        glDisable(GL_LIGHT0 + i);
    }*/

    std::vector<std::pair<std::shared_ptr<Object>, glm::mat4>> render_list = MyEngine::build_render_list(MyEngine::scene, glm::mat4(1.0f));
    std::sort(render_list.begin(), render_list.end(), [](const std::pair<std::shared_ptr<Object>, glm::mat4> a, const std::pair<std::shared_ptr<Object>, glm::mat4> b) {
        return a.first->get_priority() > b.first->get_priority();
    });

    // Gather all point lights
    std::vector<std::shared_ptr<PointLight>> point_lights;
    for (const auto& object : render_list)
    {
        const std::shared_ptr<PointLight> point_light = std::dynamic_pointer_cast<PointLight>(object.first);

        if (point_light != nullptr)
        {
            point_lights.push_back(point_light);
        }
    }

    // TODO: Also handle directional lights and spot lights

    const glm::mat4 inverse_camera_matrix = glm::inverse(MyEngine::active_camera->get_local_matrix());

    // Normal rendering
    for (const auto& node : render_list)
    {
        const std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(node.first);
        if (mesh != nullptr && point_lights.size() >= 1)
        {
            // TODO: Do this stuff for every light
            const std::shared_ptr<Shader> shader = mesh->get_shader();
            shader->set_vec3("light_ambient", point_lights.at(0)->get_ambient_color());
            shader->set_vec3("light_diffuse", point_lights.at(0)->get_diffuse_color());
            shader->set_vec3("light_specular", point_lights.at(0)->get_specular_color());
            shader->set_vec3("light_position", point_lights.at(0)->get_position());
        }

        node.first->render(inverse_camera_matrix * node.second);
    }

    // Shadow rendering
    /*glDepthFunc(GL_LEQUAL);
    const glm::mat4 shadow_model_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.0f));
    for (const auto& node : render_list)
    {
        std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(node.first);
        if (mesh != nullptr && mesh->get_cast_shadows())
        {
            const std::shared_ptr<Material> original_material = mesh->get_material();
            mesh->set_material(MyEngine::shadow_material);
            const glm::mat4 shadow_matrix = shadow_model_scale_matrix * node.second;
            mesh->render(inverse_camera_matrix * shadow_matrix);
            mesh->set_material(original_material);
        }
    }
    glDepthFunc(GL_LESS);*/

    // Screen text rendering
    /*glClear(GL_DEPTH_BUFFER_BIT); // Make the text always appear in front

    // Set orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f, (float) MyEngine::window_width, 0.0f, (float) MyEngine::window_height, -1.0f, 1.0f)));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

    // Disable lighting
    glDisable(GL_LIGHTING);

    // Write the text
    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2f(16.0f, 5.0f);
    std::string fps = "FPS: " + std::to_string((int) MyEngine::fps);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*) fps.c_str());

    glRasterPos2f(16.0f, MyEngine::window_height - 32.0f);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*) MyEngine::screen_text.c_str());

    // Re-activate lighting
    glEnable(GL_LIGHTING);*/

    // Inc. frames:
    MyEngine::frames++;
}

/**
* Handles the FPS counting.
*
* @param value Parameter used by glutTimerFunc().
*/
void LIB_API MyEngine::timer_callback(int value)
{
    MyEngine::fps = MyEngine::frames;
    MyEngine::frames = 0;
    std::cout << "fps: " << MyEngine::fps << std::endl;

    // Register the next update:
    glutTimerFunc(1000, timer_callback, 0);
}

/**
* Sets the text to be rendered on screen.
*
* @param new_text The text to render on screen.
*/
void LIB_API MyEngine::set_screen_text(const std::string new_text)
{
    MyEngine::screen_text = new_text;
}

/**
 * Handles events.
 */
void LIB_API MyEngine::update()
{
    glutMainLoopEvent();
}

/**
 * Clears the window content.
 */
void LIB_API MyEngine::clear_screen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Swaps the front and back buffers.
 */
void LIB_API MyEngine::swap_buffers()
{
    glutSwapBuffers();
}

/**
 * Issues a stop directive to the engine. Check if the engine should quit by using the is_running() function.
 *
 * Multiple calls to this function have no effect.
 */
void LIB_API MyEngine::stop()
{
    MyEngine::is_running_flag = false;
}

/**
 * De-initializes the engine. This functino should be called only once and no other function should be called after.
 *
 * Multiple calls to this function cause undefined behaviour.
 */
void LIB_API MyEngine::quit()
{
    FreeImage_DeInitialise();

    glutLeaveMainLoop();
}

/**
 * Returns the status of the running flag. This flag is set to false when calling the stop function.
 *
 * @return The state of the running flag.
 */
bool LIB_API MyEngine::is_running()
{
    return MyEngine::is_running_flag;
}

/**
 * Returns the current scene root.
 *
 * @return The root object of the scene.
 */
std::shared_ptr<Object> LIB_API MyEngine::get_scene()
{
    return MyEngine::scene;
}

/**
 * Sets the scene to be rendered.
 *
 * NOTE: A value of nullptr causes undefined behaviour.
 *
 * @param new_scene The root object of the new scene.
 */
void LIB_API MyEngine::set_scene(const std::shared_ptr<Object> new_scene)
{
    MyEngine::scene = new_scene;
    MyEngine::active_camera = nullptr;
}

/**
 * Sets the camera to use to render the scene.
 *
 * NOTE: A value of nullptr causes undefined behaviour.
 *
 * @param new_active_camera The new active camera.
 */
void LIB_API MyEngine::set_active_camera(const std::shared_ptr<Camera> new_active_camera)
{
    // Disable the old camera
    if (MyEngine::active_camera != nullptr)
    {
        MyEngine::active_camera->set_active(false);
    }

    new_active_camera->set_window_size(MyEngine::window_width, MyEngine::window_height);
    new_active_camera->set_active(true);

    MyEngine::active_camera = new_active_camera;
}

/**
 * Handles the resizing of the window.
 *
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
void LIB_API MyEngine::resize_callback(const int width, const int height)
{
    MyEngine::window_width = width;
    MyEngine::window_height = height;

    std::cout << "width: " << MyEngine::window_width << std::endl;
    std::cout << "height: " << MyEngine::window_height << std::endl;

    if (MyEngine::active_camera != nullptr)
    {
        MyEngine::active_camera->set_window_size(MyEngine::window_width, MyEngine::window_height);
    }

    glViewport(0, 0, width, height);
}

/**
* Returns the list of objects to be rendered inside of the scene.
*
* @param scene_root The root object of the scene.
* @param parent_world_matrix The world matrix used to render the scene.
*
* @return The list of objects to be rendered.
*/
std::vector<std::pair<std::shared_ptr<Object>, glm::mat4>> LIB_API MyEngine::build_render_list(const std::shared_ptr<Object> scene_root, const glm::mat4 parent_world_matrix)
{
    std::vector<std::pair<std::shared_ptr<Object>, glm::mat4>> render_list;

    render_list.push_back(std::make_pair(scene_root, parent_world_matrix * scene_root->get_local_matrix()));

    for (const auto& child : scene_root->get_children())
    {
        std::vector<std::pair<std::shared_ptr<Object>, glm::mat4>> child_render_list = MyEngine::build_render_list(child, parent_world_matrix * scene_root->get_local_matrix());

        render_list.insert(render_list.end(), child_render_list.begin(), child_render_list.end());
    }

    return render_list;
}

/**
 * Find an object in the current scene by name. If multiple objects have the same name, only one is returned. Which one is undefined.
 *
 * @param name_to_find The name of the object to find.
 * @return A pointer to the object or nullptr if no object was found with the specified name.
 */
std::shared_ptr<Object> LIB_API MyEngine::find_object_by_name(const std::string name_to_find)
{
    const auto object = find_object_by_name(name_to_find, MyEngine::scene);

    if (object == nullptr)
    {
        WARNING("Could not find object with name \"" << name_to_find << "\".");
    }

    return object;
}

/**
 * Find an object in the current scene by name. If multiple objects have the same name, only one is returned. Which one is undefined.
 *
 * @param name_to_find The name of the object to find.
 * @param root The root object
 * @return A pointer to the object or nullptr if no object was found with the specified name.
 */
std::shared_ptr<Object> LIB_API MyEngine::find_object_by_name(const std::string name_to_find, const std::shared_ptr<Object> root)
{
    for (const auto& child : root->get_children())
    {
        if (child->get_name() == name_to_find)
        {
            return child;
        }

        const auto found = MyEngine::find_object_by_name(name_to_find, child);

        if (found != nullptr)
        {
            return found;
        }
    }

    return nullptr;
}
