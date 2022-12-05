#include "FrameBuffer.hpp"
#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

FrameBuffer::FrameBuffer() { glGenFramebuffers(1, &m_Renderer_ID); }

void FrameBuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_Renderer_ID); }

void FrameBuffer::Unbind() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &m_Renderer_ID);
}

void FrameBuffer::Validate() 
{
	Bind();
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Incomplete Frame Buffer!!!\n";
	Unbind();
}