#include "leap.hpp"

#include <glm/gtc/matrix_transform.hpp>

LIB_API Leap::Leap() : connection{ nullptr }, curFrame{ nullptr }, lastFrameId{ 0 }
{
    // Build a sphere procedurally:   
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles     
    GLfloat radius = 5.0f;
    int gradation = 10;
    for (alpha = 0.0; alpha < glm::pi<float>(); alpha += glm::pi<float>() / gradation)
    {
        for (beta = 0.0f; beta < 2.01f * glm::pi<float>(); beta += glm::pi<float>() / gradation)
        {
            x = radius * cos(beta) * sin(alpha);
            y = radius * sin(beta) * sin(alpha);
            z = radius * cos(alpha);
            this->vertices.push_back(glm::vec3(x, y, z));
            x = radius * cos(beta) * sin(alpha + glm::pi<float>() / gradation);
            y = radius * sin(beta) * sin(alpha + glm::pi<float>() / gradation);
            z = radius * cos(alpha + glm::pi<float>() / gradation);
            this->vertices.push_back(glm::vec3(x, y, z));
        }
    }

    std::cout << "vertices size: " << vertices.size() << std::endl;

    // Init buffers:   
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
}

LIB_API Leap::~Leap()
{
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

unsigned int LIB_API Leap::get_vertices_size() const
{
    return this->vertices.size();
}

void LIB_API Leap::bind_vao()
{
    glBindVertexArray(this->vao);
}

/**
 * Initializes Leap Motion connection.
 */
bool LIB_API Leap::init()
{
    // Create and open connection:   
    if (LeapCreateConnection(nullptr, &connection) != eLeapRS_Success)
    {
        std::cout << "[ERROR] Unable to connect to the Leap Motion" << std::endl;
        return false;
    }
    if (LeapOpenConnection(connection) != eLeapRS_Success)
    {
        std::cout << "[ERROR] Unable to open connection to the Leap Motion" << std::endl;
        LeapDestroyConnection(connection);
        connection = nullptr;
        return false;
    }

    // Get device (only one supported):
    const uint32_t timeout = 1000;
    uint32_t computedArraySize = 0;
    for (uint32_t retry = 3; retry > 0; retry--)
    {
        LEAP_CONNECTION_MESSAGE msg;
        LeapPollConnection(connection, timeout, &msg);

        eLeapRS ret = LeapGetDeviceList(connection, nullptr, &computedArraySize);
        if (ret == eLeapRS_NotConnected)
            continue;
        else
            if (ret == eLeapRS_Success)
                if (computedArraySize)
                    break;
    }

    std::cout << "Number of available devices: " << computedArraySize << std::endl;
    if (computedArraySize != 1)
    {
        std::cout << "[ERROR] Invalid number of devices (must be 1)" << std::endl;
        LeapCloseConnection(connection);
        LeapDestroyConnection(connection);
        connection = nullptr;
        return false;
    }

    // Get ref to device:      
    if (LeapGetDeviceList(connection, &leapDevice, &computedArraySize) != eLeapRS_Success)
    {
        std::cout << "[ERROR] Unable to get ref to device" << std::endl;
        LeapCloseConnection(connection);
        LeapDestroyConnection(connection);
        connection = nullptr;
        return false;
    }

    // Done:
    return true;
}


/**
 * Releases Leap Motion resources.
 */
bool LIB_API Leap::free()
{
    // Clean up Leap Motion:
    LeapCloseConnection(connection);
    LeapDestroyConnection(connection);
    connection = nullptr;

    // Done:
    return true;
}


/**
 * Acquire new events.
 */
bool LIB_API Leap::update()
{
    const uint32_t timeout = 1000;
    LEAP_CONNECTION_MESSAGE msg;

    // Get latest update:
    int64_t now = LeapGetNow();
    int32_t frameSkipping = -1;
    do
    {
        if (LeapPollConnection(connection, timeout, &msg) != eLeapRS_Success)
        {
            std::cout << "[ERROR] Unable to poll connection" << std::endl;
            return false;
        }
        frameSkipping++;
    } while (msg.tracking_event->info.timestamp < now - 100);

    // Only tracking events are processed:
    if (msg.type != eLeapEventType_Tracking)
        return false;

    // We are lagging badly:
    if (frameSkipping > 5)
        std::cout << "[WARNING] Lag detected (" << frameSkipping << " frames skipped)" << std::endl;

    // Update only when necessary:   
    if (msg.tracking_event->tracking_frame_id > lastFrameId)
    {
        lastFrameId = msg.tracking_event->tracking_frame_id;
        curFrame = *msg.tracking_event;
    }

    // Done:
    return true;
}


/**
 * Gets a pointer to the last updated frame.
 */
const LIB_API LEAP_TRACKING_EVENT* Leap::getCurFrame() const
{
    return &curFrame;
}