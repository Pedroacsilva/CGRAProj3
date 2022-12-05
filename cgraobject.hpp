#include "VideoTexture.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "deecshader.h"
#include "cgraimage.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <vector>

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include <cmath>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <vector>
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


void PrintMat4(glm::mat4 Matrix);

class CGRAobject {
public:
  glm::mat4 modeltr;
  glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  DEECShader *shader;
  VertexArray m_VA;
  VertexBuffer m_VB;
  VertexBufferLayout m_Layout;
  IndexBuffer m_IB;
  GLuint numVertices;
  GLuint textureID;
  bool hasTexture;
  float shininess = 1.0f;
  float specularReaction = 0.0f;

  CGRAobject();
  virtual ~CGRAobject() = 0;
  void setModelTransformation(glm::mat4 &modeltransf);
  void setShader(DEECShader *shaderprog);
  void SetColor(glm::vec4 color);
  void SetUniform4f(glm::vec4 data, std::string uniformName);
  void SetUniformMat4f(glm::mat4 data, std::string uniformName);
  void SetUniform1f(float data, std::string uniformName);
  void SetShininess(float data);
  void SetSpecularReaction(float data);
  void Bind() const;
  void Unbind() const;
  void AddVA(const VertexArray va);
  void AddIndexBuffer(const void *data, unsigned int count);
  void setTexture(const char *,bool filterlinear=false);
  void setChessTexture(bool filterlinear=false);
  virtual void drawIt(glm::mat4 V, glm::mat4 P);
};

class CGRACompound : public CGRAobject{
public:
  CGRACompound(CGRAobject &Base);
  ~CGRACompound();
  void DrawTree(glm::mat4 V, glm::mat4 P);
  void PushChild(CGRACompound * Child, glm::mat4 connection);
  void setModelTransformation(glm::mat4 &modeltransf) = delete;
  void SetTransformFromMother(glm::mat4 &modeltransf);
  void PropagateModelTransformation(glm::mat4 &modeltransf);


  CGRAobject * Object;
  glm::mat4 TransformFromMother;
  std::vector<CGRACompound *> Children;
};


class CGRAExtrusion : public CGRAobject{
public:
  CGRAExtrusion(std::vector<glm::vec3> pontos);
  ~CGRAExtrusion();
  void drawIt(glm::mat4 V, glm::mat4 P) override;
};


class CGRARevolution : public CGRAobject{
public:
  CGRARevolution(std::vector<glm::vec3> pontos);
  ~CGRARevolution();
  void drawIt(glm::mat4 V, glm::mat4 P) override;
};

class CGRASquare : public CGRAobject {
public:
  
  CGRASquare();
  ~CGRASquare();
  void drawIt(glm::mat4 V, glm::mat4 P) override;
};

class CGRABackground : public CGRAobject {
public:

  GLuint videotexture_ID;
  CGRABackground();
  ~CGRABackground();
  void drawIt(glm::mat4 V, glm::mat4 P) override;
  void SetVideoTexture(GLuint vt_id);
};

class CGRACube : public CGRAobject {
public:
  
  CGRACube();
  ~CGRACube();
  void drawIt(glm::mat4 V, glm::mat4 P) override;
};

class CGRASphere: public CGRAobject{
public:
  CGRASphere();
  ~CGRASphere();
  void drawIt(glm::mat4 V, glm::mat4 P) override;
};

class CGRACylinder: public CGRAobject{
public:
  CGRACylinder();
  ~CGRACylinder();
  void drawIt(glm::mat4 V, glm::mat4 P) override;
};

class CGRACone: public CGRAobject{
public:
  CGRACone();
  ~CGRACone();
  void drawIt(glm::mat4 V, glm::mat4 P) override;
};