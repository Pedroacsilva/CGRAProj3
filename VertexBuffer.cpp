#include "VertexBuffer.hpp"
#ifndef GL_SETUP
#include <GL/glew.h>
#define GL_SETUP
#endif

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
  glGenBuffers(1, &m_RendererID);
  /*glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);*/
  Push(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer() {
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Push(GLenum target, unsigned int size, const void *data,
                        GLenum usage) {
  Bind();
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  Unbind();
}

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }