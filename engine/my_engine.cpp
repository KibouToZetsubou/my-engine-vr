#include "my_engine.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <algorithm> // std::sort

#ifdef _linux
#include <unistd.h>
#endif

#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage.h>
#include <openvr.h>

#include "directional_light.hpp"
#include "mesh.hpp"
#include "point_light.hpp"
#include "simple_shader.hpp"
#include "passthrough_shader.hpp"
#include "spot_light.hpp"

bool MyEngine::is_initialized_flag = false;
bool MyEngine::is_running_flag = false;
int MyEngine::window_id = 0;
std::shared_ptr<Object> MyEngine::scene;
std::shared_ptr<Camera> MyEngine::active_camera;
std::string MyEngine::screen_text;
int MyEngine::window_width = 0;
int MyEngine::window_height = 0;

std::shared_ptr<Shader> MyEngine::ppl_shader = nullptr;
std::shared_ptr<Shader> MyEngine::passthrough_shader = nullptr;
std::shared_ptr<FBO> MyEngine::left_eye = nullptr;
std::shared_ptr<FBO> MyEngine::right_eye = nullptr;

std::shared_ptr<OvVR> MyEngine::ovvr = nullptr;

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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
    glDebugMessageCallback((GLDEBUGPROC)debug_callback, nullptr);
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

    // Initialize FreeImage
    FreeImage_Initialise();

    //Init OpenVR:
    MyEngine::ovvr = std::make_shared<OvVR>();
    if (ovvr->init() == false)
    {
        std::cout << "[ERROR] Unable to init OpenVR" << std::endl;
    }

    // Setup callbacks
    glutDisplayFunc(render);
    glutReshapeFunc(resize_callback);
    glutTimerFunc(1000, timer_callback, 0);

    // Configure shaders
    MyEngine::ppl_shader = std::make_shared<SimpleShader>();
    MyEngine::passthrough_shader = std::make_shared<PassthroughShader>();
    MyEngine::ppl_shader->use();

    // TODO: Generalize and improve this [See frameBufferObject example from teacher] - BMPG
    //MyEngine::passthrough_shader->bind(0, "in_Position");
    //MyEngine::passthrough_shader->bind(2, "in_TexCoord");

    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    MyEngine::left_eye = std::make_shared<FBO>(512, 512);
    MyEngine::right_eye = std::make_shared<FBO>(512, 512);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_width, window_height);

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

    std::vector<std::pair<std::shared_ptr<Object>, glm::mat4>> render_list = MyEngine::build_render_list(MyEngine::scene, glm::mat4(1.0f));
    std::sort(render_list.begin(), render_list.end(), [](const std::pair<std::shared_ptr<Object>, glm::mat4> a, const std::pair<std::shared_ptr<Object>, glm::mat4> b) {
        return a.first->get_priority() > b.first->get_priority();
        });

    const glm::mat4 inverse_camera_matrix = glm::inverse(MyEngine::active_camera->get_local_matrix());
    for (auto& item : render_list)
    {
        item.second = inverse_camera_matrix * item.second;
    }

    std::vector<int> light_types; // 0 = No light; 1 = Directional; 2 = Point; 3 = Spot
    std::vector<glm::vec3> light_ambients;
    std::vector<glm::vec3> light_diffuses;
    std::vector<glm::vec3> light_speculars;
    std::vector<glm::vec3> light_positions; // Light position in eye coordinates
    std::vector<glm::vec3> light_directions; // Directional, Spot
    std::vector<float> light_radiuses; // Point, Spot
    std::vector<float> light_cutoffs; // Spot
    std::vector<float> light_exponents; // Spot

    int number_of_lights = 0;

    // Gather all point lights
    for (const auto& object : render_list)
    {
        const std::shared_ptr<PointLight> point_light = std::dynamic_pointer_cast<PointLight>(object.first);

        if (point_light != nullptr)
        {
            ++number_of_lights;
            light_types.push_back(1);
            light_ambients.push_back(point_light->get_ambient_color());
            light_diffuses.push_back(point_light->get_diffuse_color());
            light_speculars.push_back(point_light->get_specular_color());
            light_positions.push_back(object.second * glm::vec4(point_light->get_position(), 1.0f));
            light_directions.push_back(glm::vec4()); // Unused
            light_radiuses.push_back(point_light->get_radius());
            light_cutoffs.push_back(0.0f); // Unused
            light_exponents.push_back(0.0f); // Unused
        }
    }

    // Gather all directional lights
    for (const auto& object : render_list)
    {
        const std::shared_ptr<DirectionalLight> directional_light = std::dynamic_pointer_cast<DirectionalLight>(object.first);

        if (directional_light != nullptr)
        {
            ++number_of_lights;
            light_types.push_back(0);
            light_ambients.push_back(directional_light->get_ambient_color());
            light_diffuses.push_back(directional_light->get_diffuse_color());
            light_speculars.push_back(directional_light->get_specular_color());
            light_positions.push_back(object.second * glm::vec4(directional_light->get_position(), 1.0f));
            light_directions.push_back(object.second * glm::vec4(directional_light->get_direction(), 1.0f));
            light_radiuses.push_back(0.0f); // Unused
            light_cutoffs.push_back(0.0f); // Unused
            light_exponents.push_back(0.0f); // Unused
        }
    }

    // Gather all spot lights
    for (const auto& object : render_list)
    {
        const std::shared_ptr<SpotLight> spot_light = std::dynamic_pointer_cast<SpotLight>(object.first);

        if (spot_light != nullptr)
        {
            ++number_of_lights;
            light_types.push_back(2);
            light_ambients.push_back(spot_light->get_ambient_color());
            light_diffuses.push_back(spot_light->get_diffuse_color());
            light_speculars.push_back(spot_light->get_specular_color());
            light_positions.push_back(object.second * glm::vec4(spot_light->get_position(), 1.0f));
            light_directions.push_back(object.second * glm::vec4(spot_light->get_direction(), 1.0f));
            light_radiuses.push_back(spot_light->get_radius());
            light_cutoffs.push_back(spot_light->get_cutoff());
            light_exponents.push_back(spot_light->get_exponent());
        }
    }

    //Update user position:
    ovvr->update();
    glm::mat4 headPos = ovvr->getModelviewMatrix();

    for (int i = 0; i < 2; i++)
    {
        // Get OpenVR matrices:
        OvVR::OvEye curEye = (OvVR::OvEye)i;
        glm::mat4 projMat = ovvr->getProjMatrix(curEye, 1.0f, 1024.0f);
        glm::mat4 eye2Head = ovvr->getEye2HeadMatrix(curEye);

        glm::mat4 ovrProjMat = projMat * glm::inverse(eye2Head);
        // Update camera modelview matrix:
        glm::mat4 ovrModelViewMat = glm::inverse(headPos); // Inverted because this is the camera matrix

        float interocular_distance = 50.0f;

        if (i == 0) //Left
        {
            MyEngine::left_eye->use();
            interocular_distance /= -2;
        }
        else  //Right
        {
            MyEngine::right_eye->use();
            interocular_distance /= 2;
        }


        // Render the left eye
        MyEngine::clear_screen();
        MyEngine::ppl_shader->use();

        MyEngine::ppl_shader->clear_uniforms();
        MyEngine::ppl_shader->set_int("number_of_lights", number_of_lights);
        //MyEngine::ppl_shader->set_vector_int("light_type", light_types);
        MyEngine::ppl_shader->set_vector_vec3("light_ambient", light_ambients);
        MyEngine::ppl_shader->set_vector_vec3("light_diffuse", light_diffuses);
        MyEngine::ppl_shader->set_vector_vec3("light_specular", light_speculars);
        MyEngine::ppl_shader->set_vector_vec3("light_position", light_positions);
        //MyEngine::ppl_shader->set_vector_vec3("light_direction", light_directions);
        //MyEngine::ppl_shader->set_vector_float("light_radius", light_radiuses);
        //MyEngine::ppl_shader->set_vector_float("light_cutoff", light_cutoffs);
        //MyEngine::ppl_shader->set_vector_float("light_exponent", light_exponents);

        const glm::mat4 projection_matrix = MyEngine::active_camera->get_projection_matrix(512, 512);
        const glm::mat4 projection_matrix_per_eye = glm::translate(projection_matrix, glm::vec3(interocular_distance, 0.0f, 0.0f));
        //MyEngine::ppl_shader->set_mat4("projection_matrix", projection_matrix_per_eye);
        MyEngine::ppl_shader->set_mat4("projection_matrix", ovrProjMat);

        // Normal rendering
        for (const auto& node : render_list)
        {
            const std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(node.first);
            if (mesh != nullptr)
            {
                // Load material information into the shader
                const std::shared_ptr<Material> material = mesh->get_material();
                material->render(node.second);
                MyEngine::ppl_shader->set_vec3("material_emission", material->get_emission_color());
                MyEngine::ppl_shader->set_vec3("material_ambient", material->get_ambient_color());
                MyEngine::ppl_shader->set_vec3("material_diffuse", material->get_diffuse_color());
                MyEngine::ppl_shader->set_vec3("material_specular", material->get_specular_color());
                MyEngine::ppl_shader->set_float("material_shininess", material->get_shininess());

                const std::shared_ptr<Texture> texture = material->get_texture();
                MyEngine::ppl_shader->set_bool("use_texture", texture != nullptr);
            }

            MyEngine::ppl_shader->render(node.second);
            node.first->render(node.second);
        }

        MyEngine::ppl_shader->render(ovrModelViewMat);


        if (i == 0) //Left
        {
            // Send rendered image to the proper OpenVR eye:      
            ovvr->pass(curEye, MyEngine::left_eye->get_color_buffer_id());
        }
        else  //Right
        {
            // Send rendered image to the proper OpenVR eye:    
            ovvr->pass(curEye, MyEngine::right_eye->get_color_buffer_id());
        }
    }

    // Update internal OpenVR settings:
    ovvr->render();

    // Copy the left eye to the window buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1024, 512);

    //TODO: maybe remove these magic numbers - BMPG
    MyEngine::left_eye->use_read();
    glBlitFramebuffer(0, 0, 512, 512, 0, 0, 512, 512, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    MyEngine::right_eye->use_read();
    glBlitFramebuffer(0, 0, 512, 512, 512, 0, 1024, 512, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    // Screen rendering
    //MyEngine::passthrough_shader->use();

    //const glm::mat4 ortho = glm::ortho(0.0f, (float) MyEngine::window_width, 0.0f, (float) MyEngine::window_height, -1.0f, 1.0f);
    //MyEngine::passthrough_shader->set_mat4("projection", ortho);

    // Set a matrix for the left "eye": [for now the only eye!] {We are a cyclops} - BMPG
    //glm::mat4 f = glm::mat4(1.0f);
    //MyEngine::passthrough_shader->set_mat4("modelview", f);

    //Color! Blue... - BMPG
    //MyEngine::passthrough_shader->set_vec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)); //Maybe we need to add support for vec4 - BMPG

    //MyEngine::passthrough_shader->render(glm::mat4(1.0f));

    //glDisableVertexAttribArray(1); // We don't need normals for the 2D quad

    // Bind the attempted FBO buffer as texture and render:
    //glBindTexture(GL_TEXTURE_2D, MyEngine::attemptFBO->get_texture_id());
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    /*const std::shared_ptr<Plane> left_plane = std::make_shared<Plane>();
    left_plane->set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    left_plane->render(f);*/

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
