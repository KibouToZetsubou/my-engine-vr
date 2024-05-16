#include "skybox.hpp"

#include <FreeImage.h>
#include <GL/glew.h>

unsigned short LIB_API Skybox::faces[] = {
   0, 2, 1,
   0, 3, 2,
   3, 6, 2,
   3, 7, 6,
   4, 3, 0,
   4, 7, 3,
   6, 4, 5,
   7, 4, 6,
   4, 1, 5,
   4, 0, 1,
   1, 6, 5,
   1, 2, 6,
};

/**
* Used to create a new instance of Skybox
* 
* @param textures The list of textures for the skybox
*/
LIB_API Skybox::Skybox(const std::vector<std::string> textures)
{
    float vertices[] = {
       -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
    };

    glGenVertexArrays(1, &this->vao_id);
    glBindVertexArray(this->vao_id);

    // Load cube data into a VBO
    glGenBuffers(1, &this->vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->vbo_faces);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_faces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Skybox::faces), (void*) Skybox::faces, GL_STATIC_DRAW);

    glGenTextures(1, &this->cubemap_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap_id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    for (unsigned int curSide = 0; curSide < 6; ++curSide)
    {
        FIBITMAP* fBitmap = FreeImage_Load(FreeImage_GetFileType(textures[curSide].c_str(), 0), textures[curSide].c_str());

        if (fBitmap == nullptr)
        {
            throw "[ERROR] Failed to load file " + textures[curSide];
        }

        int intFormat = GL_RGB;
        GLenum extFormat = GL_BGR;
        if (FreeImage_GetBPP(fBitmap) == 32)
        {
            intFormat = GL_RGBA;
            extFormat = GL_BGRA;
        }

        FreeImage_FlipHorizontal(fBitmap);  // Correct mirroring from cube's inside
        FreeImage_FlipVertical(fBitmap);    // Correct JPG's upside-down

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + curSide, 0, intFormat, FreeImage_GetWidth(fBitmap), FreeImage_GetHeight(fBitmap), 0, extFormat, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(fBitmap));

        FreeImage_Unload(fBitmap);
    }
}

/**
* Used to delete the VAO, VBOs and the cubemap ID
*/
LIB_API Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &this->vao_id);
    glDeleteBuffers(1, &this->vbo_vertices);
    glDeleteBuffers(1, &this->vbo_faces);

    glDeleteTextures(1, &this->cubemap_id);
}

/**
* Used to render the skybox
* 
* @param view_matrix It is unused, use any 4x4 matrix (e.g. identity matrix)
*/
void LIB_API Skybox::render(const glm::mat4 view_matrix) const
{
    Node::render(view_matrix);

    glBindVertexArray(this->vao_id);
    glDrawElements(GL_TRIANGLES, sizeof(Skybox::faces) / sizeof(unsigned short), GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}
