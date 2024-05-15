#pragma once



 //////////////
 // #INCLUDE //
 //////////////

    // GLEW:
#include <GL/glew.h>

// FreeGLUT:
//#include <GL/freeglut.h>   

// GLM:
#include <glm/gtc/packing.hpp>

// OpenVR:
#include <openvr.h>

// C/C++:
#include <iostream>
#include <vector>

#include "common.hpp"


////////////////
// CLASS OvVR //
////////////////

/**
 * @brief OpenGL-OpenVR interface.
 */
class LIB_API OvVR
{
    //////////
public: //
    //////////	   

    struct Controller;

    /**
     * Eye enums
     */
    enum OvEye
    {
        EYE_LEFT = 0,
        EYE_RIGHT = 1,

        // Terminator:
        EYE_LAST
    };


    /**
    * Constructor
    */
    OvVR() : vrSys(nullptr), vrModels(nullptr), vrComp(nullptr)
    {
        controllers.clear();
    }


    /**
     * Destructor
     */
    ~OvVR()
    {}


    /**
     * @brief Init VR components.
     * @return TF
     */
    bool init()
    {
        vr::EVRInitError error = vr::VRInitError_None;

        // Init VR system:
        std::cout << "Using OpenVR " << vr::k_nSteamVRVersionMajor << "." << vr::k_nSteamVRVersionMinor << "." << vr::k_nSteamVRVersionBuild << std::endl;
        vrSys = vr::VR_Init(&error, vr::VRApplication_Scene);
        if (error != vr::VRInitError_None)
        {
            vrSys = nullptr;
            std::cout << "[ERROR] Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(error) << std::endl;
            return false;
        }

        // Init render models:
        vrModels = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &error);
        if (vrModels == nullptr)
        {
            vrSys = nullptr;
            vr::VR_Shutdown();
            std::cout << "[ERROR] Unable to get render model interface: " << vr::VR_GetVRInitErrorAsEnglishDescription(error) << std::endl;
            return false;
        }

        // Initialize the compositor:
        vrComp = vr::VRCompositor();
        if (!vrComp)
        {
            vrModels = nullptr;
            vrSys = nullptr;
            vr::VR_Shutdown();
            std::cout << "[ERROR] Unable to get VR compositor" << std::endl;
            return false;
        }

        // Tweaks:        
        //vrComp->ShowMirrorWindow();        

        // Init controller tracking:
        controllers.clear();
        for (unsigned int c = 0; c < vr::k_unMaxTrackedDeviceCount; c++)
        {
            if (vrSys->GetTrackedDeviceClass(c) == vr::TrackedDeviceClass_Controller)
            {
                std::cout << "   Found controller at " << c << std::endl;
                Controller* cont = new Controller();
                cont->id = c;

                unsigned int bufferLen = vrSys->GetStringTrackedDeviceProperty(c, vr::Prop_RenderModelName_String, nullptr, 0, nullptr);
                if (bufferLen == 0)
                {
                    std::cout << "[ERROR] Unable to get controller render model" << std::endl;
                    delete cont;
                    continue;
                }

                std::string result;
                result.resize(bufferLen);
                vrSys->GetStringTrackedDeviceProperty(c, vr::Prop_RenderModelName_String, &result[0], bufferLen, nullptr);
                std::cout << "   Controller render model: '" << result << "'" << std::endl;
                controllers.push_back(cont);
            }
        }

        // Done:
        return true;
    }


    /**
     * Release VR components.
     * @return TF
     */
    bool free()
    {
        for (unsigned int c = 0; c < controllers.size(); c++)
            delete controllers[c];
        controllers.clear();

        vr::VR_Shutdown();
        vrComp = nullptr;
        vrModels = nullptr;
        vrSys = nullptr;

        // Done:      
        return true;
    }


    /**
     * Get tracking system name.
     * @return tracking system name
     */
    std::string getTrackingSysName()
    {
        unsigned int bufferLen = vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, nullptr, 0, nullptr);
        if (bufferLen == 0)
            return std::string();

        std::string result;
        result.resize(bufferLen);
        vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, &result[0], bufferLen, nullptr);
        return result;
    }


    /**
     * Print render models to the screen.
     */
    bool printRenderModels()
    {
        for (unsigned int c = 0; c < vrModels->GetRenderModelCount(); c++)
        {
            char buffer[256];
            vrModels->GetRenderModelName(c, buffer, 256);
            std::cout << "   " << c << ") " << buffer << " model" << std::endl;

            for (unsigned int d = 0; d < vrModels->GetComponentCount(buffer); d++)
            {
                char cbuffer[256];
                vrModels->GetComponentName(buffer, d, cbuffer, 256);
                std::cout << "     " << d << ") " << cbuffer << std::endl;
            }
        }

        // Done:
        return true;
    }


    /**
     * Get manufacturer name.
     * @return manufacturer system name
     */
    std::string getManufacturerName()
    {
        unsigned int bufferLen = vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ManufacturerName_String, nullptr, 0, nullptr);
        if (bufferLen == 0)
            return std::string();

        std::string result;
        result.resize(bufferLen);
        vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ManufacturerName_String, &result[0], bufferLen, nullptr);
        return result;
    }


    /**
     * Get model number (name).
     * @return model number (name)
     */
    std::string getModelNumber()
    {
        unsigned int bufferLen = vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ModelNumber_String, nullptr, 0, nullptr);
        if (bufferLen == 0)
            return std::string();

        std::string result;
        result.resize(bufferLen);
        vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ModelNumber_String, &result[0], bufferLen, nullptr);
        return result;
    }


    /**
     * Get HMD proper horizontal resolution in pixels.
     * @return HMD horizontal resolution in pixels
     */
    unsigned int getHmdIdealHorizRes()
    {
        unsigned int result, dummy;
        vrSys->GetRecommendedRenderTargetSize(&result, &dummy);
        return result;
    }


    /**
     * Get HMD proper vertical resolution in pixels.
     * @return HMD vertical resolution in pixels
     */
    unsigned int getHmdIdealVertRes()
    {
        unsigned int result, dummy;
        vrSys->GetRecommendedRenderTargetSize(&dummy, &result);
        return result;
    }


    /**
     * Converts an OpenVR 4x3 matrix into an OpenGL one.
     * @param matrix OpenVR 34 matrix
     * @return OpenGL-friendly matrix
     */
    static glm::mat4 ovr2ogl(const vr::HmdMatrix34_t& matrix)
    {
        return glm::mat4(matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], 0.0f,
            matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], 0.0f,
            matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], 0.0f,
            matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], 1.0f);
    }


    /**
     * Converts an OpenVR 4x4 matrix into an OpenGL one.
     * @param matrix OpenVR 44 matrix
     * @return Overvision-friendly matrix
     */
    static glm::mat4 ovr2ogl(const vr::HmdMatrix44_t& matrix)
    {
        return glm::mat4(matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], matrix.m[3][0],
            matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], matrix.m[3][1],
            matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], matrix.m[3][2],
            matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], matrix.m[3][3]);
    }


    /**
     * Update poses and internal params. Invoke that once per frame.
     * @return TF
     */
    bool update()
    {
        // Main update method:
        vrComp->WaitGetPoses(vrPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

        // If used, update controllers:
        for (auto c : controllers)
        {
            if (vrPoses[c->id].bPoseIsValid)
                c->matrix = ovr2ogl(vrPoses[c->id].mDeviceToAbsoluteTracking);
        }

        // Done:
        return true;
    }


    /**
     * Get the projection matrix for the given eye and plane params.
     * @param eye left or right eye (use enum)
     * @param nearPlane user camera near plane distance
     * @param farPlane user camera far plane distance
     * @return projection matrix ready for OpenGL
     */
    glm::mat4 getProjMatrix(OvEye eye, float nearPlane, float farPlane)
    {
        switch (eye)
        {
        case EYE_LEFT: return ovr2ogl(vrSys->GetProjectionMatrix(vr::Eye_Left, nearPlane, farPlane)); break;
        case EYE_RIGHT: return ovr2ogl(vrSys->GetProjectionMatrix(vr::Eye_Right, nearPlane, farPlane)); break;
        default: return glm::mat4(1.0f);
        }
    }


    /**
     * Get the eye-to-head modelview matrix for the given eye.
     * @param eye left or right eye (use enum)
     * @return eye-to-head modelview matrix ready for OpenGL
     */
    glm::mat4 getEye2HeadMatrix(OvEye eye)
    {
        switch (eye)
        {
        case EYE_LEFT: return ovr2ogl(vrSys->GetEyeToHeadTransform(vr::Eye_Left)); break;
        case EYE_RIGHT: return ovr2ogl(vrSys->GetEyeToHeadTransform(vr::Eye_Right)); break;
        default: return glm::mat4(1.0f);
        }
    }


    /**
     * Get the user's head modelview position.
     * @return modelview matrix ready for OpenGL
     */
    glm::mat4 getModelviewMatrix()
    {
        if (vrPoses[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid == false)
            return glm::mat4(1.0f);

        glm::mat4 headPos = ovr2ogl(vrPoses[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
        return headPos;
    }


    /**
     * Get the number of identified and initialized controllers.
     * @return number of controllers
     */
    unsigned int getNrOfControllers()
    {
        return (unsigned int)controllers.size();
    }


    /**
     * Get pointer to internal controller reference.
     * @param pos controller position in the list
     * @return pointer or nullptr if error
     */
    Controller* getController(unsigned int pos) const
    {
        if (pos >= controllers.size())
            return nullptr;
        return controllers.at(pos);
    }


    /**
     * Enable/disable reprojection
     * @param flag true or false
     */
    void setReprojection(bool flag)
    {
        vrComp->ForceInterleavedReprojectionOn(flag);
    }


    /**
     * Pass the left and right textures to the HMD.
     * @param eye left or right eye (use enum)
     * @param eyeTexture OpenGL texture handle
     */
    void pass(OvEye eye, unsigned int eyeTexture)
    {
        const vr::Texture_t t = { reinterpret_cast<void*>(uintptr_t(eyeTexture)), vr::TextureType_OpenGL, vr::ColorSpace_Linear };
        switch (eye)
        {
        case EYE_LEFT:  vrComp->Submit(vr::Eye_Left, &t); break;
        case EYE_RIGHT: vrComp->Submit(vr::Eye_Right, &t); break;
        }
    }


    /**
     * Once passed the left and right textures, invoke this method to terminate rendering.
     */
    void render()
    {
        vrComp->PostPresentHandoff();
    }


    // Controller data:
    struct Controller
    {
        unsigned int id;
        vr::VRControllerState_t pControllerState;
        glm::mat4 matrix;

        // Const/dest:
        Controller() : id(0), matrix(glm::mat4(1.0f)) {}
        ~Controller() {}


        /**
         * Get controller's position matrix.
         * @param id controller id
         * @return model matrix ready for OpenGL
         */
        glm::mat4 getMatrix()
        {
            return matrix;
        }


        /**
         * Get controller's axis position.
         * @param axis axis id, between 0 and 4
         * @param bPressed inout, button pressed (use masks to determine which ones)
         * @param bTouched inout, button touched (use masks to determine which ones)
         * @return axis values
         */
        glm::vec2 getAxis(unsigned int axisId, unsigned long long int& bPressed, unsigned long long int& bTouched)
        {
            if (axisId >= vr::k_unControllerStateAxisCount)
                return glm::vec2(0.0f);

            glm::vec2 res;
            res.x = pControllerState.rAxis[axisId].x;
            res.y = pControllerState.rAxis[axisId].y;
            bPressed = pControllerState.ulButtonPressed;
            bTouched = pControllerState.ulButtonTouched;

            // Done:
            return res;
        }


        /**
         * Returns true when the Nth button is enabled (given the input mask).
         * @param id button id
         * @param mask button mask
         * @return true when pressed, false otherwise
         */
        bool isButtonPressed(unsigned long long int id, unsigned long long int mask)
        {
            uint64_t buttonMask = 1ull << id;
            if (mask & buttonMask)
                return true;
            else
                return false;
        }
    };


    ///////////
private: //
    ///////////	

       // OVR objects:	
    vr::IVRSystem* vrSys;
    vr::IVRRenderModels* vrModels;
    vr::IVRCompositor* vrComp;
    vr::TrackedDevicePose_t vrPoses[vr::k_unMaxTrackedDeviceCount];
    std::vector<Controller*> controllers;
};