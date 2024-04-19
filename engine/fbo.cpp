#include "fbo.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

LIB_API FBO::FBO()
{
    this->size_X = 1024; //TODO: Change these magic numbers - BMPG
    this->size_Y = 512; //TODO: Change these magic numbers - BMPG

    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);

    // TODO: What should this do?
    // I think it's the "color" texture/buffer of the FBO rendering, maybe im wrong - BMPG
    // TODO: Transform the following gl gen/bind/tex in a single Texture
    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->size_X, this->size_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); //TODO: Change these magic numbers - BMPG
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texture_id, 0);

    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &this->size_X);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &this->size_Y);

    glBindFramebuffer(GL_FRAMEBUFFER, this->id);

    // Create depth buffer
    glGenRenderbuffers(1, &this->depth_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depth_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, this->size_X, this->size_Y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth_buffer_id);

    GLenum dBuff[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, dBuff);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw "Failed to create the FBO.";
    }
}

bool LIB_API FBO::render()
{
    // Bind buffers:
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glViewport(0, 0, this->size_X, this->size_Y);

    //Done
    return true;
}

unsigned int LIB_API FBO::get_texture_id()
{
    return this->texture_id;
}