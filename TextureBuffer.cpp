#include "TextureBuffer.hpp"
#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

#ifndef FBO
#include "FrameBuffer.cpp"
#define FBO
#endif

TextureBuffer::TextureBuffer() {
  glGenTextures(1, &m_Renderer_ID);
  glBindTexture(GL_TEXTURE_2D, m_Renderer_ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

TextureBuffer::~TextureBuffer() {}

void TextureBuffer::Bind() { glBindTexture(GL_TEXTURE_2D, m_Renderer_ID); }

void const TextureBuffer::AttachToFrameBuffer(FrameBuffer &fbo) {
  fbo.Bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_Renderer_ID, 0);
}
