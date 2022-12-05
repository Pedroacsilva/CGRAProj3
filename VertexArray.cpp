#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"

VertexArray::VertexArray() {
  glGenVertexArrays(1, &m_Renderer_ID);
  glBindVertexArray(m_Renderer_ID);
}

VertexArray::~VertexArray() {}

void VertexArray::Bind() const { glBindVertexArray(m_Renderer_ID); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddBuffer(const VertexBuffer &vb,
                            const VertexBufferLayout &layout) {
  Bind();
  vb.Bind();
  const auto &elements = layout.GetElements();
  unsigned long int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto &element = elements[i];
    glVertexAttribPointer(i, element.count, element.type, element.normalized,
                          layout.GetStride(), (const void *) offset);
    glEnableVertexAttribArray(i);
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
  Unbind();
  vb.Unbind();
}
