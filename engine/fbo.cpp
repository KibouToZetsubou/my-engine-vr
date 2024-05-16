#include "fbo.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>


/**
 * Creates a new instance of a Frame Buffer Object with the given width and height
 *
 * @param width The width of the FBO
 * @param height The height of the FBO
 */
LIB_API FBO::FBO(const int width, const int height)
{
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);

    // TODO: Transform the following gl gen/bind/tex in a single Texture
    glGenTextures(1, &this->color_buffer_id);
    glBindTexture(GL_TEXTURE_2D, this->color_buffer_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->color_buffer_id, 0);

    // TODO: I don't think this is necessary; we already binded this texture before.
    glBindTexture(GL_TEXTURE_2D, this->color_buffer_id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &this->width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &this->height);

    // TODO: I don't think this is necessary; we already binded this framebuffer before.
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);

    // Create depth buffer
    glGenRenderbuffers(1, &this->depth_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depth_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, this->width, this->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth_buffer_id);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw "Failed to create the FBO.";
    }
}


/**
* Used to bind the FBO
* 
*/
void LIB_API FBO::use() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glViewport(0, 0, this->width, this->height);
}

/**
* Used to bind the FBO in readonly mode
*
*/
void LIB_API FBO::use_read() const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);
    glViewport(0, 0, this->width, this->height);
}

/**
* Used to retreive the ID of the color buffer
* 
* @return The ID of the color buffer
*/
unsigned int LIB_API FBO::get_color_buffer_id() const
{
    return this->color_buffer_id;
}
