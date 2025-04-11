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
    glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
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

    for (size_t i = 0; i < m_ColorAttachments.size(); i++) 
    {
        glClearBufferfv(GL_COLOR, i, clearColor);
    }

    glClearBufferfv(GL_DEPTH, 0, clearColor);

    glFlush();
}

void FrameBuffer::clear_fbo_target(uint32_t slot)
{
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, slot, clearColor);
}
void FrameBuffer::bind_scene_tex(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[slot]);
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
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthTexture);
    }

    Specification = spec;

    glGenFramebuffers(1, &m_RenderID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

    m_ColorAttachments.resize(spec.num_color_attachments);
    for (size_t i = 0; i < spec.num_color_attachments; i++)
    {
        glGenTextures(1, &m_ColorAttachments[i]);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, spec.viewport.x, spec.viewport.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorAttachments[i], 0);
    }

    glGenTextures(1, &m_DepthTexture);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, spec.viewport.x, spec.viewport.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

    std::vector<GLenum> drawBuffers(spec.num_color_attachments);
    for (size_t i = 0; i < spec.num_color_attachments; ++i)
    {
        drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(spec.num_color_attachments, drawBuffers.data());

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Frame Buffer Creation Failed";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::blit_buffer()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RenderID);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachments[0], 0); 

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RenderID);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_ColorAttachments[1], 0); 

    // blit from color attachment 0 (source) to color attachment 1 (destination), will make these parmams
    glBlitFramebuffer(0, 0, Specification.viewport.x, Specification.viewport.y,
                      0, 0, Specification.viewport.x, Specification.viewport.y,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST); 

    //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); 
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 

}


void FrameBuffer::set_draw_target(uint32_t slot)
{
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + slot);
}

void FrameBuffer::draw_to_all_targets()
{
    glDrawBuffers(m_ColorAttachments.size(), m_ColorAttachments.data()); 
}