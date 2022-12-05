#pragma once

class FrameBuffer {
  unsigned int m_Renderer_ID;

public:
  FrameBuffer();
  ~FrameBuffer();
  void Bind();
  void Unbind();
  unsigned int GetID();
  void Validate();
};