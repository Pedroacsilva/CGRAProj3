#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <vector>
#include <cmath>
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

#include "GLObject.hpp"

GLObject::GLObject(VertexArray va) : m_VA(va) {}

GLObject::GLObject() {}

GLObject::~GLObject() {}

void GLObject::Bind() const { m_VA.Bind(); }

void GLObject::Unbind() const { m_VA.Unbind(); }

Cube::Cube() {
  float face_positions[] = {
      // Vertex Coordinates  Tex Coords       RGBA  Values       Normals
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,// 0
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.0f, -1.0f,// 1
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,   0.0f, 0.0f, -1.0f,// 2
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.0f, -1.0f,// 3
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, // 4
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.0f, 1.0f, // 5
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, // 6
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.0f, 1.0f, // 7
      //-----------------------------------------------------------------------------
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  1.0f, 0.0f, 0.0f,// 8
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f,// 9
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  1.0f, 0.0f, 0.0f,// 10
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,   1.0f, 0.0f, 0.0f,// 11
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.0f, // 12
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,-1.0f, 0.0f, 0.0f, // 13
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.0f, // 14
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  -1.0f, 0.0f, 0.0f, // 15
      //-----------------------------------------------------------------------------
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 1.0f, 0.0f,// 16
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,   0.0f, 1.0f, 0.0f,// 17
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 1.0f, 0.0f,// 18
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f,// 19
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f,// 20
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  0.0f, -1.0f, 0.0f,// 21
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f,// 22
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f// 23
  };

  unsigned int indices[] = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
                            8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
                            16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

  m_VB.Push(GL_ARRAY_BUFFER, sizeof(face_positions), face_positions,
            GL_STATIC_DRAW);
  m_IB.Push(indices, 6 * 6);
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_Layout.Push<float>(3, "Vertex Normals");
  m_VA.AddBuffer(m_VB, m_Layout);
}

Cube::~Cube() {}

Pyramid::Pyramid() {
  float vtx_positions[] = {0.0f,  0.5f, 0.0f,         0.0f,  1.0f,      0.5f, 0.5f, 0.5f, 1.0f,
                           -0.5f, 0.0f, -0.5f,        0.0f,  0.0f,      0.5f, 0.5f, 0.5f, 1.0f,
                           0.5f,  0.0f, -0.5f,        0.25f, 0.0f,      0.5f, 0.5f, 0.5f, 1.0f,
                           0.5f,  0.0f, 0.5f,         0.5f,  0.0f,      0.5f, 0.5f, 0.5f, 1.0f,
                           -0.5f, 0.0f, 0.5f,         0.75f, 0.0f,      0.5f, 0.5f, 0.5f, 1.0f};
  unsigned int indices[] = {0, 1, 2, 0, 2, 3, 0, 3, 4,
                            0, 4, 1, 4, 1, 2, 2, 3, 4};

  m_VB.Push(GL_ARRAY_BUFFER, sizeof(vtx_positions), vtx_positions,
            GL_STATIC_DRAW);
  m_IB.Push(indices, 6 * 3);
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_VA.AddBuffer(m_VB, m_Layout);
};

Pyramid::~Pyramid() {}


Cylinder::Cylinder(){
  const float raio = 0.5f;
  const float PI = 3.14f;
  float theta = 0.0f;
  std::vector <float> vtx_info;
  for(int i = 0; i < 20; i++){
    //XYZ
    vtx_info.emplace_back(std::cos(theta) * raio);      vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(std::sin(theta) * raio);
    //UV
    vtx_info.emplace_back(i / 20);                      vtx_info.emplace_back(1.0f);
    //RGB
    vtx_info.emplace_back(0.5f);                        vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(0.5f);            vtx_info.emplace_back(1.0f);
    //Normal
    glm::vec3 normal = glm::normalize(glm::vec3(std::cos(theta), 0.0f, std::sin(theta)));
    vtx_info.emplace_back(normal[0]);                   vtx_info.emplace_back(normal[1]);     vtx_info.emplace_back(normal[2]);

    theta += 2 * PI / 20;
  }
  theta = 0.0f;
  for(int i = 0; i < 20; i++){
    //XYZ
    vtx_info.emplace_back(std::cos(theta) * raio);      vtx_info.emplace_back(-0.5f);         vtx_info.emplace_back(std::sin(theta) * raio);
    //UV
    vtx_info.emplace_back(i / 20);                      vtx_info.emplace_back(1.0f);
    //RGB
    vtx_info.emplace_back(0.5f);                        vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(0.5f);            vtx_info.emplace_back(1.0f);
    //Normal
    glm::vec3 normal = glm::normalize(glm::vec3(std::cos(theta), 0.0f, std::sin(theta)));
    vtx_info.emplace_back(normal[0]);           vtx_info.emplace_back(normal[1]);     vtx_info.emplace_back(normal[2]);

    theta += 2 * PI / 20;
  }
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_Layout.Push<float>(3, "Vertex Normals");
  m_VB.Push(GL_ARRAY_BUFFER, vtx_info.size() * sizeof(float), vtx_info.data(), GL_STATIC_DRAW);
  std::vector <int> indices;
  for(int i = 0; i < 20; i++){
    indices.push_back(i); indices.push_back(i + 1); indices.push_back(i + 20);
    indices.push_back(i + 20); indices.push_back(i + 1); indices.push_back(i + 21); 
    std::cout << i << ", " << i + 1 << ", " << i + 20 << "\n";
    std::cout << i + 20 << ", " << i + 1 << ", " << i + 21 << "\n";
  }
  indices.pop_back();indices.pop_back();indices.pop_back();
  indices.push_back(0); indices.push_back(19); indices.push_back(20); 

  m_IB.Push(indices.data(), indices.size());
  m_VA.AddBuffer(m_VB, m_Layout);
}


Cylinder::~Cylinder(){}


Sphere::Sphere(){
  /*    Eqs paramétricas
  x = (r * cos(phi) * cos(theta))
  y = (r * cos(phi) * sin(theta))
  z = r * sin(phi)                */
  const float radius = 1.0f;
  const float PI = 3.14f;
  float x, y, z, xy;
  //float nx, ny, nz;
  float u, v;
  float sectorStep = 2 * PI / 40;
  float stackStep = PI / 40;
  float sectorAngle, stackAngle;
  std::vector <float> vtx_info;

  for(int i = 0; i <= 40; i++){
    stackAngle = PI / 2 - i * stackStep;    //Começar no pólo norte
    xy = radius * std::cos(stackAngle);
    z = radius * std::sin(stackAngle);
    for(int j = 0; j <= 40; j++){
      sectorAngle = j * sectorStep;
      x = xy * std::cos(sectorAngle);
      y = xy * std::sin(sectorAngle);
      //XYZ
      vtx_info.emplace_back(x);       vtx_info.emplace_back(y);       vtx_info.emplace_back(z);
      //V
      u = static_cast<float>(j) / 40;
      v = static_cast<float>(i) / 40;
      vtx_info.emplace_back(u);       vtx_info.emplace_back(v);
      //RGBA
      vtx_info.emplace_back(0.5f);    vtx_info.emplace_back(0.5f);    vtx_info.emplace_back(0.5f);            vtx_info.emplace_back(1.0f);
      //Normals
      vtx_info.emplace_back(x);       vtx_info.emplace_back(y);       vtx_info.emplace_back(z);


    }
  }
      m_VB.Push(GL_ARRAY_BUFFER, vtx_info.size() * sizeof(float), vtx_info.data(), GL_STATIC_DRAW);
      m_Layout.Push<float>(3, "Vertex Coordinates");
      m_Layout.Push<float>(2, "Texture Coordinates");
      m_Layout.Push<float>(4, "Vertex Colors");
      m_Layout.Push<float>(3, "Vertex Normals");
  //Prepare IBO
  std::vector <int> indices;
  int k1, k2;
  for(int i = 0; i < 40; ++i){
    k1 = i * 41;
    k2 = k1 + 41;
    for(int j = 0; j < 40; ++j, ++k1, ++k2){
      if(i != 0){
        indices.push_back(k1);  indices.push_back(k2); indices.push_back(k1 + 1);
      }
      if(i != 39){
        indices.push_back(k1 + 1);  indices.push_back(k2); indices.push_back(k2 + 1); 
      }
    }
  }
  m_IB.Push(indices.data(), indices.size());
  m_VA.AddBuffer(m_VB, m_Layout);


}

Sphere::~Sphere(){}