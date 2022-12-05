#pragma once

class VertexBuffer {
  unsigned int m_RendererID;

public:
  VertexBuffer(const void *data, unsigned int size);
  VertexBuffer();
  ~VertexBuffer();

  void Push(GLenum target, unsigned int size, const void * data, GLenum usage);

  void Bind() const;
  void Unbind() const;
};