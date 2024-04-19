#include "fbo.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

LIB_API FBO::FBO()
{
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);

    // TODO: What should this do?
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0);

    // Create depth buffer
    glGenRenderbuffers(1, &this->depth_buffer_id);
    glBindFramebuffer(GL_RENDERBUFFER, this->depth_buffer_id);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth_buffer_id);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw "Failed to create the FBO.";
    }
}
