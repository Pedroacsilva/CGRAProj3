#pragma once

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#ifndef VA
#include "VertexArray.cpp"
#define VA
#endif

#ifndef VB
#include "VertexBuffer.cpp"
#define VB
#endif

#ifndef VBL
#include "VertexBufferLayout.hpp"
#define VBL
#endif

#ifndef IB
#include "IndexBuffer.cpp"
#define IB
#endif

class GLObject {
public:

  VertexArray m_VA;
  VertexBuffer m_VB;
  VertexBufferLayout m_Layout;
  IndexBuffer m_IB;
//  float m_Positions;

  GLObject(VertexArray va);
  GLObject();
  ~GLObject();

  void Bind() const;
  void Unbind() const;

  void AddVA(const VertexArray va);

  void AddIndexBuffer(const void * data, unsigned int count);
};



class Cube : public GLObject {
public:
  Cube();
  ~Cube();
};


class Pyramid : public GLObject{
public:
  Pyramid();
  ~Pyramid();
};

class Cylinder : public GLObject{
public:
  Cylinder();
  ~Cylinder();
};


class Sphere : public GLObject{
public:
  Sphere();
  ~Sphere();
};