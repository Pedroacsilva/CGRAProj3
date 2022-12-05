#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>


struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;
  std::string name;

  static unsigned int GetSizeOfType(unsigned int type) {
    switch (type) {
    case GL_FLOAT:
      return 4;
      break;
    case GL_UNSIGNED_INT:
      return 4;
      break;
    case GL_UNSIGNED_BYTE:
      return 1;
    }
    return 0;
  }
};

class VertexBufferLayout {
public:
  VertexBufferLayout() : m_Stride(0){};
  ~VertexBufferLayout() { m_Stride = 0; };

  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;
  const std::vector<VertexBufferElement> GetElements() const {
    return m_Elements;
  }
  inline unsigned int GetStride() const { return m_Stride; }

  template <typename T> void Push(unsigned int count, std::string string = "") {}
};

template <> inline void VertexBufferLayout::Push<float>(unsigned int count, std::string string) {
//  std::cout << "(VBL) Data name: " << string << "\n";
  m_Elements.push_back({GL_FLOAT, count, GL_FALSE, string});
  m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}
template <>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count, std::string string ) {
//  std::cout << "(VBL) Data name: " << string << "\n";
  m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE, string});
  m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}
template <>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count, std::string string ) {
//  std::cout << "(VBL) Data name: " << string << "\n";
  m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_FALSE, string});
  m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}