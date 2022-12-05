#pragma once

#include "FrameBuffer.hpp"
class RenderBuffer {
  unsigned int m_Renderer_ID;

public:
  RenderBuffer();
  ~RenderBuffer();
  void Bind();
  void Unbind();
  void const AttachToFrameBuffer(FrameBuffer &fbo);
};