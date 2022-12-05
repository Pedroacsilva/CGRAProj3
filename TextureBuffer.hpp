#pragma once

#include "FrameBuffer.hpp"
class TextureBuffer {
  unsigned int m_Renderer_ID;

public:
  TextureBuffer();
  ~TextureBuffer();
  void Bind();
  void Unbind();
  void Push();
  void const AttachToFrameBuffer(FrameBuffer &fbo);
};