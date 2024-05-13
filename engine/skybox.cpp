#include "skybox.hpp"

#include <GL/glew.h>

// FreeImage:
#include <FreeImage.h>


static float cubeVertices[] = // Vertex and tex. coords are the same
{
   -1.0f,  1.0f,  1.0f,
   -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
   -1.0f,  1.0f, -1.0f,
   -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
};
static unsigned short cubeFaces[] =
{
   0, 1, 2,
   0, 2, 3,
   3, 2, 6,
   3, 6, 7,
   4, 0, 3,
   4, 3, 7,
   6, 5, 4,
   7, 6, 4,
   4, 5, 1,
   4, 1, 0,
   1, 5, 6,
   1, 6, 2,
};

LIB_API Skybox::Skybox(std::vector<std::string> cubemapNames)
{
    glGenVertexArrays(1, &this->vao_id);
    glBindVertexArray(this->vao_id);

    // Load cube data into a VBO:
    glGenBuffers(1, &this->vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->vbo_faces);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_faces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeFaces), cubeFaces, GL_STATIC_DRAW);

    generateCubeMap(cubemapNames);

    this->set_scale(glm::vec3(100.0f));
}

LIB_API Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &this->vao_id);
    glDeleteBuffers(1, &this->vbo_vertices);
    glDeleteBuffers(1, &this->vbo_faces);

    glDeleteTextures(1, &this->cubemap_id);
}

LIB_API void Skybox::generateCubeMap(std::vector<std::string> cubemapNames)
{
    // Create and bind cubemap:   
    glGenTextures(1, &this->cubemap_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap_id);

    // Set params:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Set filters:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Load sides:
    for (int curSide = 0; curSide < 6; curSide++)
    {
        // Load texture:
        FIBITMAP* fBitmap = FreeImage_Load(FreeImage_GetFileType(cubemapNames[curSide].c_str(), 0), cubemapNames[curSide].c_str());
        if (fBitmap == nullptr)
            std::cout << "[ERROR] loading file '" << cubemapNames[curSide] << "'" << std::endl;
        int intFormat = GL_RGB;
        GLenum extFormat = GL_BGR;
        if (FreeImage_GetBPP(fBitmap) == 32)
        {
            intFormat = GL_RGBA;
            extFormat = GL_BGRA;
        }

        // Fix mirroring:
        FreeImage_FlipHorizontal(fBitmap);  // Correct mirroring from cube's inside
        FreeImage_FlipVertical(fBitmap);    // Correct JPG's upside-down

        // Send texture to OpenGL:
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + curSide, 0, intFormat, FreeImage_GetWidth(fBitmap), FreeImage_GetHeight(fBitmap), 0, extFormat, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(fBitmap)); 

        // Free resources:
        FreeImage_Unload(fBitmap);
    }
}

void LIB_API Skybox::render(const glm::mat4 view_matrix) const
{
    //Node::render(view_matrix);

    glBindVertexArray(this->vao_id);
    glDrawElements(GL_TRIANGLES, sizeof(cubeFaces) / sizeof(unsigned short), GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}
