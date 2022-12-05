#include "RenderBuffer.hpp"

#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

#ifndef FBO
#include "FrameBuffer.cpp"
#define FBO
#endif

RenderBuffer::RenderBuffer() {
  glGenRenderbuffers(1, &m_Renderer_ID);
  glBindRenderbuffer(GL_RENDERBUFFER, m_Renderer_ID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 640, 480);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer() {}

void RenderBuffer::Bind() {
  glBindRenderbuffer(GL_RENDERBUFFER, m_Renderer_ID);
}

void RenderBuffer::Unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

void const RenderBuffer::AttachToFrameBuffer(FrameBuffer &fbo) 
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Renderer_ID);
}