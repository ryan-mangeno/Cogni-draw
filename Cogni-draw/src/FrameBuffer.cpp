#include "FrameBuffer.h"

#include <glad/glad.h>

#include <iostream>

FrameBuffer::FrameBuffer(FrameBufferSpecification&& spec)
    : Specification(std::move(spec))
{
    invalidate(spec);
}
FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_RenderID);
    glDeleteTextures(1, &m_SceneTexture);
    glDeleteTextures(1, &m_DepthTexture);
}
void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RenderID);
}
void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
void FrameBuffer::resize(unsigned int width, unsigned int height)
{
    Specification.viewport = { width, height };
    invalidate(Specification);
    glViewport(0, 0, width, height);
}
void FrameBuffer::clear_fbo()
{
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, clearColor);
    glFlush();
}
void FrameBuffer::bind_scene_tex(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_SceneTexture);
}
void FrameBuffer::bind_depth_tex(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
}
void FrameBuffer::invalidate(const FrameBufferSpecification& spec)
{


    if (m_RenderID)
    {
        glDeleteFramebuffers(1, &m_RenderID);
        glDeleteTextures(1, &m_SceneTexture);
        glDeleteTextures(1, &m_DepthTexture);
    }

    Specification = spec;

    glGenFramebuffers(1, &m_RenderID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

    glGenTextures(1, &m_SceneTexture);
    glBindTexture(GL_TEXTURE_2D, m_SceneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, spec.viewport.x, spec.viewport.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SceneTexture, 0);
    const GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

    glGenTextures(1, &m_DepthTexture);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, spec.viewport.x, spec.viewport.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Frame Buffer Creation Failed";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}