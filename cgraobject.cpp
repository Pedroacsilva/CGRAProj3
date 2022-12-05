#include "VideoTexture.hpp"
#ifndef CGRAOBJECT
#include "cgraobject.hpp"
#define CGRAOBJECT
#endif
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

#ifndef VT
#include "VideoTexture.cpp"
#define VT
#endif

// Classe para print mat4

void PrintMat4(glm::mat4 Matrix) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << Matrix[j][i] << "\t";
    }
    std::cout << "\n";
  }
}

// Classe base
CGRAobject::CGRAobject() { modeltr = glm::mat4(1.0); }

CGRAobject::~CGRAobject() {}

void CGRAobject::setModelTransformation(glm::mat4 &modeltransf) {
  modeltr = modeltransf;
}

void CGRAobject::setShader(DEECShader *shaderprog) { shader = shaderprog; }

void CGRAobject::SetUniform4f(glm::vec4 data, std::string uniformName) {
  int data_location =
      glGetUniformLocation(shader->shaderprogram, uniformName.c_str());
  glUniform4f(data_location, data[0], data[1], data[2], data[3]);
}

void CGRAobject::SetUniform1f(float data, std::string uniformName) 
{
    int data_location =
      glGetUniformLocation(shader->shaderprogram, uniformName.c_str());
  glUniform1f(data_location, data);
}

void CGRAobject::SetUniformMat4f(glm::mat4 data, std::string uniformName) {
  int data_location =
      glGetUniformLocation(shader->shaderprogram, uniformName.c_str());
  glUniformMatrix4fv(data_location, 1, GL_FALSE, &data[0][0]);
}

void CGRAobject::SetShininess(float data) {
  shininess = data;
}


void CGRAobject::SetColor(glm::vec4 in_color) { color = in_color; }

void CGRAobject::setChessTexture(bool filterlinear) {
  // Create one OpenGL texture

  // our image will be composed of a 2 by 2 pixels black and white checkerboard
  //  note: opengl by default uses 32bits per pixel unless it is changed with
  // glPixelStorei(GL_UNPACK_ALIGNMENT, #)
  static unsigned char data[] = {255, 255, 255, 0, 0,   0,   0,   0,
                                 0,   0,   0,   0, 255, 255, 255, 0};

  glGenTextures(1, &textureID);

  // All future texture functions will apply to this texture

  glBindTexture(GL_TEXTURE_2D, textureID);
  // Pass the image ( data ) to OpenGL
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  if (!filterlinear)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  //  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               data);

  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  hasTexture = true;
}

void CGRAobject::setTexture(const char *fname, bool filterlinear) {
  CGRAimage img;
  if (!img.loadPPM(fname)) {
    printf("(SET TEXTURE) Erro a abrir PPM.\n");
    return;
  }
  glGenTextures(1, &textureID);

  // All future texture functions will apply to this texture

  glBindTexture(GL_TEXTURE_2D, textureID);
  // Pass the image ( data ) to OpenGL
  //    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  //  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  if (!filterlinear)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // glPixelStorei(GL_PACK_ALIGNMENT,1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, img.data);

  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  hasTexture = true;
}

void CGRAobject::drawIt(glm::mat4 V, glm::mat4 P) {}

void CGRAobject::SetSpecularReaction(float data){
  specularReaction = data;  
}



/*--------------------------+
|         Compostos         |
+---------------------------*/

CGRACompound::CGRACompound(CGRAobject &Base) {
  Object = &Base;
  TransformFromMother =
      glm::mat4(1.0f); // Referencial da base é o referencial inicial do objecto
}

CGRACompound::~CGRACompound() {}

void CGRACompound::PushChild(CGRACompound *Child, glm::mat4 connection) {
  Child->SetTransformFromMother(connection);
  Children.push_back(Child);
}

void CGRACompound::DrawTree(glm::mat4 V, glm::mat4 P) {
  glm::mat4 newModelTr(1.0f), oldModelTr(1.0f);
  newModelTr = TransformFromMother * Object->modeltr;
  oldModelTr = Object->modeltr;
  Object->setModelTransformation(newModelTr);
  Object->drawIt(V, P);
  Object->setModelTransformation(oldModelTr);
  for (const auto &elemt : Children) {
    elemt->DrawTree(V, P);
  }
}

void CGRACompound::SetTransformFromMother(glm::mat4 &modeltransf) {
  TransformFromMother = modeltransf;
  modeltr = TransformFromMother * modeltr;
}

void CGRACompound::PropagateModelTransformation(glm::mat4 &modeltransf) {
  Object->modeltr = modeltransf * Object->modeltr;
  for (const auto &elemt : Children) {
    elemt->TransformFromMother = modeltransf * elemt->TransformFromMother;
  }
}

/*--------------------------+
|         Extrusões         |
+---------------------------*/

CGRAExtrusion::CGRAExtrusion(std::vector<glm::vec3> pontos) {
  std::vector<float> vtx_info;
  unsigned int numPontos = pontos.size();
  //  std::cout << "num de pontos: " << numPontos << "\n";
  float x_new = 0.0f, y_new = 0.0f, z_new = 0.0f, x, y, z;
  float u = - 1.0f / (numPontos - 1);//, v = -1.0f / (numPontos - 1);
  // Push dos pontos iniciais
  for (const auto &elemt : pontos) {
    u += 1.0f / (numPontos - 1);
//    std::cout << "EXTRUSAO: u = " << u << "\n";
    // XYZ 
    vtx_info.emplace_back(elemt[0]);
    vtx_info.emplace_back(elemt[1]);
    vtx_info.emplace_back(elemt[2]);
    // UV
    vtx_info.emplace_back(u);
    vtx_info.emplace_back(0.0f);
//    std::cout << u << "\t" << v << "\n";
    // RGBA 
    vtx_info.emplace_back(0.5f);
    vtx_info.emplace_back(0.5f);
    vtx_info.emplace_back(0.5f);
    vtx_info.emplace_back(1.0f);
    // Normals (?) (Teria de calcular a normal entre cada trio de pontos? Desta forma será complicado, oop. Terei de refactorar tudo!)
    // PLACEHOLDER NORMALS
    vtx_info.emplace_back(1.0f);
    vtx_info.emplace_back(0.0f);
    vtx_info.emplace_back(0.0f);
  }
  u = - 1.0f / (numPontos - 1);
  for (const auto &elemt : pontos) {
    u += 1.0f / (numPontos - 1);
//    std::cout << "EXTRUSAO: v = " << v << "\n";
    x = elemt[0];
    y = elemt[1];
    z = elemt[2];
    x_new = x;
    y_new = y;
    z_new = z + 1.0f;
    // Push novos vértices
    // XYZ
    vtx_info.emplace_back(x_new);
    vtx_info.emplace_back(y_new);
    vtx_info.emplace_back(z_new);
    // UV
    vtx_info.emplace_back(u);
    vtx_info.emplace_back(1.0);
//    std::cout << u << "\t" << v << "\n";
    // RGBA 
    vtx_info.emplace_back(0.5f);
    vtx_info.emplace_back(0.5f);
    vtx_info.emplace_back(0.5f);
    vtx_info.emplace_back(1.0f);
    // Normals (?) (Teria de calcular a normal entre cada trio de pontos? Desta forma será complicado, oop. Terei de refactorar tudo!)
    // PLACEHOLDER NORMALS
    vtx_info.emplace_back(1.0f);
    vtx_info.emplace_back(0.0f);
    vtx_info.emplace_back(0.0f);
  }

  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < numPontos; i++) {
    indices.emplace_back(i);             // Entre [0, numPontos]
    indices.emplace_back(i + numPontos); // Entre [numPontos, 2 * numPontos - 1]
    indices.emplace_back((i + 1) % numPontos +
                         numPontos); // Entre [numPontos, 2 * numPontos - 1]

    /*    std::cout << "i = " << i << ": " << i << ", " << i + numPontos << ", "
                  << i + 1 + numPontos << "\t";*/

    indices.emplace_back(i);                   // Entre [0, numPontos]
    indices.emplace_back((i + 1) % numPontos); // Entre [0, numPontos]
    indices.emplace_back(((i + 1) % numPontos) +
                         numPontos); // Entre [numPontos, 2* numPontos - 1]
    /*    std::cout << "i = " << i << ": " << i << ", " << ((i + 1) % numPontos)
                  << ", " << ((i + 1) % numPontos) + numPontos << "\n";*/
  }

  m_VB.Push(GL_ARRAY_BUFFER, vtx_info.size() * sizeof(float), vtx_info.data(),
            GL_STATIC_DRAW);

  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "RGBA");
  m_Layout.Push<float>(3, "Normals");
  m_IB.Push(indices.data(), indices.size());
  m_VA.AddBuffer(m_VB, m_Layout);
  hasTexture = true;
}

CGRAExtrusion::~CGRAExtrusion() {}

void CGRAExtrusion::drawIt(glm::mat4 V, glm::mat4 P) {
  m_VA.Bind();
  m_IB.Bind();

  glm::mat4 mvp = P * V * modeltr;
  
  SetUniformMat4f(mvp, "u_MVP");
  SetUniform4f(color, "u_Colors");
  SetUniformMat4f(modeltr, "u_Model");
  SetUniform1f(shininess, "u_Shininess");
  SetUniform1f(specularReaction, "u_SpecularReaction");
  if(hasTexture)
    glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

/*--------------------------+
|         Revoluções        |
+---------------------------*/

CGRARevolution::CGRARevolution(std::vector<glm::vec3> pontos) {
  const float PI = 3.14f;
  const float thetaStep = 2 * PI / 10;
  float theta = 0.0f;
  std::vector<float> vtx_info;
  float x_new = 0.0f, /*y_new = 0.0f,*/ z_new = 0.0f, x, y, z;
  // Aplicar 10 rotações ao redor do eixo z aos pontos recebidos.
/*  for (int i = 0; i < 10; i++) {
    for (const auto &elemt : pontos) {
      x = elemt[0];
      y = elemt[1];
      z = elemt[2];
      x_new = static_cast<float>(std::cos(theta) * x - std::sin(theta) * y);
      y_new = static_cast<float>(std::sin(theta) * x + std::cos(theta) * y);
      vtx_info.emplace_back(x_new);
      vtx_info.emplace_back(y_new);
      vtx_info.emplace_back(z);
    }
    theta += thetaStep;
  }*/
  x_new = 0.0f; /*y_new = 0.0f;*/ z_new = 0.0f;
    // Aplicar 10 rotações ao redor do eixo y aos pontos recebidos.
  for (int i = 0; i < 10; i++) {
    for (const auto &elemt : pontos) {
      x = elemt[0];
      y = elemt[1];
      z = elemt[2];
      x_new = static_cast<float>(std::cos(theta) * x + std::sin(theta) * z);
      z_new = static_cast<float>(-1 * std::sin(theta) * x + std::cos(theta) * z);
      // XYZ
      vtx_info.emplace_back(x_new);
      vtx_info.emplace_back(y);
      vtx_info.emplace_back(z_new);
      // UV
      float v = (i % 2 == 0) ? 0.0f : 1.0f;
      vtx_info.emplace_back(v);
      vtx_info.emplace_back(i * 0.10f);
//      std::cout << "v = " << v << "\n";
//      (i % 2 == 0) ? vtx_info.emplace_back(1.0f) : vtx_info.emplace_back(0.0f);
//      std::cout << "Revo UV: " << i * 0.10f << ", " << v << "\n";
    }
    theta += thetaStep;
  }

  // Indices:
  // No fim das rotações, teremos 10 vezes mais vértices.
  // Seja P_i o i-ésimo ponto inicial e P^n_i o i-ésimo ponto na n-ésima
  // iteração da rotação. Pretendemos unir triângulos compostos pelos vértices
  // (P^n_i, P^n_(i+1), P^(n+1)_(i+1)) e pelos vértices (P^n_i, P^(n+1)_i,
  // P^(n+1)_(i+1))

  std::vector<int> indices;
  int p1, p2;
  int pStep = pontos.size();
  // Esta implementação funciona apenas para 2 pontos!! Terei de fazer outro
  // nested loop para iterar (ver caso esfera) p1 entre 1 até Nº de pontos -1.
  // Se tiver tempo quando o projecto estiver estável, trabalho nisso. Por
  // enquanto, apenas quero   qualquer coisa a ser desenhada no ecra

  // Iterar ao longo das rotações
  for (unsigned int i = 0; i < 9; i++) {
    // Iterar ao longo dos pontos
    for (unsigned int j = 0; j < pontos.size(); j++) {
      p1 = i * pStep;
      p2 = p1 + 1;
      indices.emplace_back(p1);
      indices.emplace_back(p2);
      indices.emplace_back(p2 + pStep);

      indices.emplace_back(p1);
      indices.emplace_back(p2 + pStep);
      indices.emplace_back(p1 + pStep);
    }
  }

  // Caso degenerado, vértices vão OOB. Agora que me lembro, podia ter feito
  // indice % limite.

  indices.emplace_back(pStep * 9);
  indices.emplace_back(pStep * 9 + 1);
  indices.emplace_back(1);
  indices.emplace_back(pStep * 9);
  indices.emplace_back(1);
  indices.emplace_back(0);

  m_VB.Push(GL_ARRAY_BUFFER, vtx_info.size() * sizeof(float), vtx_info.data(),
            GL_STATIC_DRAW);
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_IB.Push(indices.data(), indices.size());
  m_VA.AddBuffer(m_VB, m_Layout);
}

CGRARevolution::~CGRARevolution() {}

void CGRARevolution::drawIt(glm::mat4 V, glm::mat4 P) {
  m_VA.Bind();
  m_IB.Bind();

  glm::mat4 mvp = P * V * modeltr;
  
  SetUniformMat4f(mvp, "u_MVP");
  SetUniform4f(color, "u_Colors");
  SetUniformMat4f(modeltr, "u_Model");
  SetUniform1f(shininess, "u_Shininess");
  SetUniform1f(specularReaction, "u_SpecularReaction");
  if(hasTexture)
    glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

/*--------------------------+
|         Quadrado          |
+---------------------------*/

CGRASquare::CGRASquare() {
  float face_positions[] = {
      // Vertex Coordinates
/*      -0.5f, -0.5f, 0.0f, // 0
      0.5f,  -0.5f, 0.0f, // 1
      0.5f,  0.5f,  0.0f, // 2
      -0.5f, 0.5f,  0.0f  // 3*/
    // XYZ                UV          RGBA                    Normals
      -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 0
      0.5f,  0.0f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 1
      0.5f,  0.0f,  0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 2
      -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 3
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};
  m_VB.Push(GL_ARRAY_BUFFER, sizeof(face_positions), face_positions,
            GL_STATIC_DRAW);
  m_IB.Push(indices, 6);
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_Layout.Push<float>(3, "Vertex Normals");
  m_VA.AddBuffer(m_VB, m_Layout);
  setShader(shader);
}

CGRASquare::~CGRASquare() {}

void CGRASquare::drawIt(glm::mat4 V, glm::mat4 P) {
  m_VA.Bind();
  m_IB.Bind();

  glm::mat4 mvp = P * V * modeltr;
  
  SetUniformMat4f(mvp, "u_MVP");
  SetUniform4f(color, "u_Colors");
  SetUniformMat4f(modeltr, "u_Model");
  SetUniform1f(shininess, "u_Shininess");
  SetUniform1f(specularReaction, "u_SpecularReaction");
  if(hasTexture)
    glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

/*--------------------------+
|         Background        |
+---------------------------*/

CGRABackground::CGRABackground() {
  float face_positions[] = {
    // XYZ                UV          RGBA                    Normals
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 0
      1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 1
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 2
      -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 3
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};
  m_VB.Push(GL_ARRAY_BUFFER, sizeof(face_positions), face_positions,
            GL_STATIC_DRAW);
  m_IB.Push(indices, 6);
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_Layout.Push<float>(3, "Vertex Normals");
  m_VA.AddBuffer(m_VB, m_Layout);
  setShader(shader);
}

CGRABackground::~CGRABackground() {}

void CGRABackground::drawIt(glm::mat4 V, glm::mat4 P) {
  m_VA.Bind();
  m_IB.Bind();

  glm::mat4 mvp = P * V * modeltr;
  
  SetUniformMat4f(mvp, "u_MVP");
  SetUniform4f(color, "u_Colors");
  SetUniformMat4f(modeltr, "u_Model");
  SetUniform1f(shininess, "u_Shininess");
  SetUniform1f(specularReaction, "u_SpecularReaction");
  if(hasTexture){
    glBindTexture(GL_TEXTURE_2D, textureID);
//    std::cout << "VideoTexture ID: " << textureID << "\n";
  }
  glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void CGRABackground::SetVideoTexture(GLuint vt_id){
  textureID = vt_id;
  hasTexture = true;
}

/*--------------------------+
|         Cubo              |
+---------------------------*/

CGRACube::CGRACube() {
  float face_positions[] = {
      // Vertex Coordinates  Tex Coords       RGBA  Values              Normals
      -0.5f, -0.5f, -0.5f,    0.00f, 0.33f,     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,// 0
      0.5f, -0.5f, -0.5f,     0.25f, 0.33f,     0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.0f, -1.0f,// 1
      0.5f, 0.5f, -0.5f,      0.25f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f,   0.0f, 0.0f, -1.0f,// 2
      -0.5f, 0.5f, -0.5f,     0.00f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.0f, -1.0f,// 3
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f,     0.75f, 0.33f,     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, // 4
      0.5f, -0.5f, 0.5f,      0.50f, 0.33f,     0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.0f, 1.0f, // 5
      0.5f, 0.5f, 0.5f,       0.50f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, // 6
      -0.5f, 0.5f, 0.5f,      0.75f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 0.0f, 1.0f, // 7
      //-----------------------------------------------------------------------------
      0.5f, -0.5f, 0.5f,      0.50f, 0.33f,     0.5f, 0.5f, 0.5f, 1.0f,  1.0f, 0.0f, 0.0f,// 8
      0.5f, -0.5f, -0.5f,     0.25f, 0.33f,     0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f,// 9
      0.5f, 0.5f, -0.5f,      0.25f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f,  1.0f, 0.0f, 0.0f,// 10
      0.5f, 0.5f, 0.5f,       0.50f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f,   1.0f, 0.0f, 0.0f,// 11
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f,     0.75f, 0.33f,     0.5f, 0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.0f, // 12
      -0.5f, -0.5f, -0.5f,    1.00f, 0.33f,     0.5f, 0.5f, 0.5f, 1.0f,-1.0f, 0.0f, 0.0f, // 13
      -0.5f, 0.5f, -0.5f,     1.00f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.0f, // 14
      -0.5f, 0.5f, 0.5f,      0.75f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f,  -1.0f, 0.0f, 0.0f, // 15
      //-----------------------------------------------------------------------------
      -0.5f, 0.5f, 0.5f,      0.75f, 1.00f,     0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 1.0f, 0.0f,// 16
      0.5f, 0.5f, 0.5f,       0.50f, 1.00f,     0.5f, 0.5f, 0.5f, 1.0f,   0.0f, 1.0f, 0.0f,// 17
      0.5f, 0.5f, -0.5f,      0.50f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f,  0.0f, 1.0f, 0.0f,// 18
      -0.5f, 0.5f, -0.5f,     0.75f, 0.66f,     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f,// 19
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f,     0.75f, 0.00f,     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f,// 20
      0.5f, -0.5f, 0.5f,      0.50f, 0.00f,     0.5f, 0.5f, 0.5f, 1.0f,  0.0f, -1.0f, 0.0f,// 21
      0.5f, -0.5f, -0.5f,     0.50f, 0.33f,       0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f,// 22
      -0.5f, -0.5f, -0.5f,    0.75f, 0.33f,       0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f// 23
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

CGRACube::~CGRACube() {}

void CGRACube::drawIt(glm::mat4 V, glm::mat4 P) {
  m_VA.Bind();
  m_IB.Bind();

  glm::mat4 mvp = P * V * modeltr;
  
  SetUniformMat4f(mvp, "u_MVP");
  SetUniform4f(color, "u_Colors");
  SetUniformMat4f(modeltr, "u_Model");
  SetUniform1f(shininess, "u_Shininess");
  SetUniform1f(specularReaction, "u_SpecularReaction");
  if(hasTexture)
    glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

/*--------------------------+
|         Esfera            |
+---------------------------*/

CGRASphere::CGRASphere() {

  /*    Eqs paramétricas
x = (r * cos(phi) * cos(theta))
y = (r * cos(phi) * sin(theta))
z = r * sin(phi)
xy = r *  cos(phi)            */
  const float radius = 1.0f;
  const float PI = 3.14f;
  float x, y, z, xy;
  //float nx, ny, nz;
  float u, v;
  float sectorStep = 2 * PI / 10;
  float stackStep = PI / 10;
  float sectorAngle, stackAngle;
  std::vector <float> vtx_info;

  for(int i = 0; i <= 10; i++){
    stackAngle = PI / 2 - i * stackStep;    //Começar no pólo norte
    xy = radius * std::cos(stackAngle);
    z = radius * std::sin(stackAngle);
    for(int j = 0; j <= 10; j++){
      sectorAngle = j * sectorStep;
      x = xy * std::cos(sectorAngle);
      y = xy * std::sin(sectorAngle);
      //XYZ
      vtx_info.emplace_back(x);       vtx_info.emplace_back(y);       vtx_info.emplace_back(z);
      //UV
      u = static_cast<float>(j) / 10;
      v = static_cast<float>(i) / 10;
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
  // Preparar IBO
  std::vector<int> indices;
  int p1, p2;
  for (int i = 0; i < 10; i++) {
    p1 = i * 11;
    p2 = p1 + 11;
    for (int j = 0; j < 10; j++, p1++, p2++) {
      // Fragmentos do pólo norte podem ser descritos por um triângulo dado que
      // os vértices com phi = 180 colapsam no mesmo ponto.
      if (i != 0) {
        indices.emplace_back(p1);
        indices.emplace_back(p2);
        indices.emplace_back(p1 + 1);
      }
      // Fragmentos do pólo sul podem ser descritos por um triângulo dado que os
      // vértices com phi = 180 colapsam no mesmo ponto.
      if (i != 9) {
        indices.emplace_back(p1 + 1);
        indices.emplace_back(p2);
        indices.emplace_back(p2 + 1);
      }
    }
  }
  m_IB.Push(indices.data(), indices.size());
  m_VA.AddBuffer(m_VB, m_Layout);
}

CGRASphere::~CGRASphere() {}

void CGRASphere::drawIt(glm::mat4 V, glm::mat4 P) {
  m_VA.Bind();
  m_IB.Bind();

  glm::mat4 mvp = P * V * modeltr;
  
  SetUniformMat4f(mvp, "u_MVP");
  SetUniform4f(color, "u_Colors");
  SetUniformMat4f(modeltr, "u_Model");
  SetUniform1f(shininess, "u_Shininess");
  SetUniform1f(specularReaction, "u_SpecularReaction");
  if(hasTexture)
    glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

/*--------------------------+
|         Cilindro          |
+---------------------------*/

CGRACylinder::CGRACylinder() {
  const float raio = 0.5f;
  const float PI = 3.14f;
  float theta = 0.0f;
  std::vector <float> vtx_info;
  for(int i = 0; i < 10; i++){
    //XYZ
    vtx_info.emplace_back(std::cos(theta) * raio);      vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(std::sin(theta) * raio);
    //UV
    vtx_info.emplace_back(2 * i / 9);                      vtx_info.emplace_back(1.0f);
    //RGBA
    vtx_info.emplace_back(0.5f);                        vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(0.5f);            vtx_info.emplace_back(1.0f);
    //Normal
    glm::vec3 normal = glm::normalize(glm::vec3(std::cos(theta), 0.0f, std::sin(theta)));
    vtx_info.emplace_back(normal[0]);                   vtx_info.emplace_back(normal[1]);     vtx_info.emplace_back(normal[2]);

    theta += 2 * PI / 10;
  }
  theta = 0.0f;
  for(int i = 0; i < 10; i++){
    //XYZ
    vtx_info.emplace_back(std::cos(theta) * raio);      vtx_info.emplace_back(-0.5f);         vtx_info.emplace_back(std::sin(theta) * raio);
    //UV
    vtx_info.emplace_back(2 * i / 9);                      vtx_info.emplace_back(0.0f);
    //RGB
    vtx_info.emplace_back(0.5f);                        vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(0.5f);            vtx_info.emplace_back(1.0f);
    //Normal
    glm::vec3 normal = glm::normalize(glm::vec3(std::cos(theta), 0.0f, std::sin(theta)));
    vtx_info.emplace_back(normal[0]);           vtx_info.emplace_back(normal[1]);     vtx_info.emplace_back(normal[2]);

    theta += 2 * PI / 10;
  }
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_Layout.Push<float>(3, "Vertex Normals");
  m_VB.Push(GL_ARRAY_BUFFER, vtx_info.size() * sizeof(float), vtx_info.data(),
            GL_STATIC_DRAW);
  std::vector<int> indices;
  for (int i = 0; i < 10; i++) {
    indices.emplace_back(i);
    indices.emplace_back(i + 1);
    indices.emplace_back(i + 10);
    indices.emplace_back(i + 10);
    indices.emplace_back(i + 1);
    indices.emplace_back(i + 11);
  }
  // Caso degenerado
  indices.pop_back();
  indices.pop_back();
  indices.pop_back();
  indices.emplace_back(0);
  indices.emplace_back(9);
  indices.emplace_back(10);

  m_IB.Push(indices.data(), indices.size());
  m_VA.AddBuffer(m_VB, m_Layout);
}

CGRACylinder::~CGRACylinder() {}

void CGRACylinder::drawIt(glm::mat4 V, glm::mat4 P) {
  m_VA.Bind();
  m_IB.Bind();

  glm::mat4 mvp = P * V * modeltr;
  
  SetUniformMat4f(mvp, "u_MVP");
  SetUniform4f(color, "u_Colors");
  SetUniformMat4f(modeltr, "u_Model");
  SetUniform1f(shininess, "u_Shininess");
  SetUniform1f(specularReaction, "u_SpecularReaction");
  if(hasTexture)
    glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

/*--------------------------+
|         Cone              |
+---------------------------*/

CGRACone::CGRACone() {
  const float raio = 0.5f;
  const float PI = 3.14f;
  float theta = 0.0f;
  std::vector<float> vtx_info;

  for(int i = 0; i < 10; i++){
    //XYZ
    vtx_info.emplace_back(0.0f);      vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(0.0f);
    //UV
    vtx_info.emplace_back(i / 10.0f);                      vtx_info.emplace_back(1.0f);
    //RGBA
    vtx_info.emplace_back(0.5f);                        vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(0.5f);            vtx_info.emplace_back(1.0f);
    //Normal
    glm::vec3 normal = glm::normalize(glm::vec3(std::cos(theta), 0.0f, std::sin(theta)));
    vtx_info.emplace_back(normal[0]);                   vtx_info.emplace_back(normal[1]);     vtx_info.emplace_back(normal[2]);
    theta += 2 * PI / 10;
  }
  theta = 0.0f;
  for(int i = 0; i < 10; i++){
    //XYZ
    vtx_info.emplace_back(std::cos(theta) * raio);      vtx_info.emplace_back(-0.5f);         vtx_info.emplace_back(std::sin(theta) * raio);
    //UV
    vtx_info.emplace_back(static_cast<float>(i / 10.0f));                      vtx_info.emplace_back(0.0f);
    //RGB
    vtx_info.emplace_back(0.5f);                        vtx_info.emplace_back(0.5f);          vtx_info.emplace_back(0.5f);            vtx_info.emplace_back(1.0f);
    //Normal
    glm::vec3 normal = glm::normalize(glm::vec3(std::cos(theta), 0.0f, std::sin(theta)));
    vtx_info.emplace_back(normal[0]);           vtx_info.emplace_back(normal[1]);     vtx_info.emplace_back(normal[2]);

    theta += 2 * PI / 10;

//    std::cout << "Base: (u, v) -> (" << static_cast<float>(i / 9.0f) << ", " << 0.0f << ").\n";
  }
  m_VB.Push(GL_ARRAY_BUFFER, vtx_info.size() * sizeof(float), vtx_info.data(),
            GL_STATIC_DRAW);
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_Layout.Push<float>(3, "Vertex Normals");

  // Conectar indices
  std::vector<int> indices;
  for(int i = 0; i < 9; i++){
    indices.emplace_back(i);
    indices.emplace_back(i + 10);
    indices.emplace_back(i + 11);
  }
  indices.emplace_back(9); indices.emplace_back(19); indices.emplace_back(10);

  m_IB.Push(indices.data(), indices.size());
  m_VA.AddBuffer(m_VB, m_Layout);
}

CGRACone::~CGRACone() {}

void CGRACone::drawIt(glm::mat4 V, glm::mat4 P) {
  m_VA.Bind();
  m_IB.Bind();

  glm::mat4 mvp = P * V * modeltr;
  
  SetUniformMat4f(mvp, "u_MVP");
  SetUniform4f(color, "u_Colors");
  SetUniformMat4f(modeltr, "u_Model");
  SetUniform1f(shininess, "u_Shininess");
  SetUniform1f(specularReaction, "u_SpecularReaction");
  if(hasTexture)
    glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}
