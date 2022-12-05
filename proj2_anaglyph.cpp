#include "Camera.cpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include <cmath>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL

#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

/*#ifndef STB_I
#include "vendor/stb_image/stb_image.cpp"
#include "vendor/stb_image/stb_image_print.cpp"
#define STB_I
#endif

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"
#include "vendor/imgui/imgui_impl_opengl3.h"*/

#include <iostream>
#include <string>

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

#ifndef TBO
#include "TextureBuffer.cpp"
#define TBO
#endif

#ifndef RBO
#include "RenderBuffer.cpp"
#define RBO
#endif

#ifndef FBO
#include "FrameBuffer.cpp"
#define FBO
#endif

#include "cgraobject.cpp"
#include "deecshader.h"

#define GLEW_STATIC

Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f,
              0.0f);
float AmbientColor[3] = {1.0f, 1.0f,
                         1.0f}; // Por norma, luz branca (mudar eventualmente
                                // para cenario nighttime).

DEECShader *basicShader;
GLFWwindow *window;
bool wire = false, visitorPOV = true;
float yaw = -90.0f;
// float lastX = 400, lastY = 300, yaw = -90.0f, pitch = 0.0f;
float deltaTime = 0.0f, lastFrame = 0.0f;
bool firstMouse = true;
float cameraSpeed = 4.0f;

const float displacement = 0.05f;

glm::vec3 viewerPosition(0.0f, 1.0f, 5.0f);
const glm::vec3 PointOfInterest(0.0f);
const glm::vec3 upVector(0.0f, 1.0f, 0.0f);
glm::vec3 viewerRight =
    glm::normalize(glm::cross(-(viewerPosition - PointOfInterest), upVector));

Camera viewerPositionCameraL(viewerPosition -
                                 glm::vec3(displacement, 0.0f, 0.0f),
                             upVector, 0.0f, 0.0f);
Camera viewerPositionCameraR(viewerPosition +
                                 glm::vec3(displacement, 0.0f, 0.0f),
                             upVector, 0.0f, 0.0f);

glm::vec4 GetTranslationFromMat4(const glm::mat4 &modeltr) {
  glm::vec4 outVector;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == 3)
        outVector[j] = modeltr[i][j];
    }
  }
  return outVector;
}

void PrintVec4(glm::vec4 Matrix) {
  std::cout << Matrix[0] << "\t" << Matrix[1] << "\t" << Matrix[2] << "\t"
            << Matrix[3] << "\n";
}

void SetUniform3f(float f0, float f1, float f2, std::string uniformName,
                  DEECShader *shader) {
  int data_location =
      glGetUniformLocation(shader->shaderprogram, uniformName.c_str());
  glUniform3f(data_location, f0, f1, f2);
}
void SetUniform1f(float f0, std::string uniformName, DEECShader *shader) {
  int data_location =
      glGetUniformLocation(shader->shaderprogram, uniformName.c_str());
  glUniform1f(data_location, f0);
}

glm::vec4 lampadaColoridaColor(0.0f);

void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_TRUE)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_TRUE){
    viewerPositionCameraL.m_Position += viewerPositionCameraL.m_Front * cameraSpeed * deltaTime;
    viewerPositionCameraR.m_Position += viewerPositionCameraR.m_Front * cameraSpeed * deltaTime;
  }
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_TRUE){
    viewerPositionCameraL.m_Position -= viewerPositionCameraL.m_Front * cameraSpeed * deltaTime;
    viewerPositionCameraR.m_Position -= viewerPositionCameraR.m_Front * cameraSpeed * deltaTime;  }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_TRUE) {
    yaw -= cameraSpeed * deltaTime * 40;
    glm::vec3 direction(1.0f);
    direction[0] = cos(glm::radians(yaw));
    direction[1] = 0.0f;
    direction[2] = sin(glm::radians(yaw));
    viewerPositionCameraL.m_Front = glm::normalize(direction);
    viewerPositionCameraL.m_Right =
        glm::normalize(glm::cross(viewerPositionCameraL.m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
    viewerPositionCameraR.m_Front = glm::normalize(direction);
    viewerPositionCameraR.m_Right =
        glm::normalize(glm::cross(viewerPositionCameraR.m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_TRUE) {
    yaw += cameraSpeed * deltaTime * 40;
    glm::vec3 direction(1.0f);
    direction[0] = cos(glm::radians(yaw));
    direction[1] = 0.0f;
    direction[2] = sin(glm::radians(yaw));
    viewerPositionCameraL.m_Front = glm::normalize(direction);
    viewerPositionCameraL.m_Right =
        glm::normalize(glm::cross(viewerPositionCameraL.m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
    viewerPositionCameraR.m_Front = glm::normalize(direction);
    viewerPositionCameraR.m_Right =
        glm::normalize(glm::cross(viewerPositionCameraR.m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
  }
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_TRUE) {
    wire = !wire;
    (wire) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
           : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_TRUE)
    visitorPOV = !visitorPOV;
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_TRUE) {
//    glClearColor(0.0f, 0.f, 0.56f, 1.0f);
    SetUniform1f(0.0f, "u_AmbientStrength", basicShader);
    SetUniform1f(0.0f, "u_DirectionalDiffStrength", basicShader);
    SetUniform1f(0.0f, "u_SpecularStrength", basicShader);
    SetUniform1f(0.05f, "u_WhiteLampAmbientStrength", basicShader);
    SetUniform1f(0.1f, "u_WhiteLampDiffuseStrength", basicShader);
    SetUniform1f(0.1f, "u_WhiteLampSpecularStrength", basicShader);
    SetUniform1f(0.05f, "u_ColoredLampAmbientStrength", basicShader);
    SetUniform1f(0.1f, "u_ColoredLampDiffuseStrength", basicShader);
    SetUniform1f(0.1f, "u_ColoredLampSpecularStrength", basicShader);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_TRUE) {
//    glClearColor(0.53f, 0.8f, 0.92f, 1.0f);
    SetUniform1f(0.8f, "u_AmbientStrength", basicShader);
    SetUniform1f(0.8f, "u_DirectionalDiffStrength", basicShader);
    SetUniform1f(0.8f, "u_SpecularStrength", basicShader);
    SetUniform1f(0.0f, "u_WhiteLampAmbientStrength", basicShader);
    SetUniform1f(0.0f, "u_WhiteLampDiffuseStrength", basicShader);
    SetUniform1f(0.0f, "u_WhiteLampSpecularStrength", basicShader);
    SetUniform1f(0.0f, "u_ColoredLampAmbientStrength", basicShader);
    SetUniform1f(0.0f, "u_ColoredLampDiffuseStrength", basicShader);
    SetUniform1f(0.0f, "u_ColoredLampSpecularStrength", basicShader);
  }
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_TRUE) {
    SetUniform3f(1.0f, 0.0f, 0.0f, "u_ColoredLampColor", basicShader);
    lampadaColoridaColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_TRUE) {
    SetUniform3f(0.0f, 1.0f, 0.0f, "u_ColoredLampColor", basicShader);
    lampadaColoridaColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_TRUE) {
    SetUniform3f(0.0f, 0.0f, 1.0f, "u_ColoredLampColor", basicShader);
    lampadaColoridaColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_TRUE) {
    SetUniform3f(1.0f, 1.0f, 1.0f, "u_ColoredLampColor", basicShader);
    lampadaColoridaColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_TRUE) {
    SetUniform3f(0.0f, 0.0f, 0.0f, "u_ColoredLampColor", basicShader);
    lampadaColoridaColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }
}

int main(int argc, char const *argv[]) {

  // Inicialização de Glfw e Glew

  if (!glfwInit()) {
    std::cout << "Error initializing GLFW.\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Projecto 2 - Pedro Silva", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, KeyCallback);

  if (glewInit() != GLEW_OK) {
    std::cout << "Error initializing GLEW.\n";
    return -1;
  }

  // Carregar & linkar shaders

  basicShader = new DEECShader;
  std::cout << "loading shaders.\n";
  if (basicShader->loadShaders("texturelight.vert", "texturelight.frag") ==
      GL_FALSE) {
    printf("ERROR LOADING SHADERS.\n");
    exit(EXIT_FAILURE);
  }
  std::cout << "linking shaders.\n";
  if (basicShader->linkShaderProgram() == GL_FALSE) {
    printf("ERROR LINKING SHADERS.\n");
    exit(EXIT_FAILURE);
  }
  basicShader->startUsing();
  // Setup de uniformes para iluminação. Começar no cenário de dia.
  // SetUniform3f(0.0f, 0.0f, 0.0f, "u_AmbientLightColor", basicShader);
  SetUniform3f(AmbientColor[0], AmbientColor[1], AmbientColor[2],
               "u_AmbientLightColor", basicShader);
  SetUniform1f(0.8f, "u_AmbientStrength", basicShader);
  SetUniform1f(0.8f, "u_DirectionalDiffStrength", basicShader);
  SetUniform1f(0.8f, "u_SpecularStrength", basicShader);
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  // Ligar teste de profundidade. O fragmento com menor profundidade será
  // desenhado.

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Cor do céu. Aproveitar para o cenário
//  glClearColor(0.53f, 0.8f, 0.92f, 1.0f); Nao funciona por causa do anaglifo oop
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Matriz de projecção: perspectiva com near plane = 0.5f e far plane = 30.0f.
  // FOV = 80º
  glm::mat4 proj = glm::perspective(glm::radians(80.0f), 1.0f, 0.5f, 30.0f);

  // Declarar objectos
  CGRASquare chao;
  CGRACube carroCorpo, carro2Corpo;
  CGRASquare cartaz;
  CGRASphere sol, esferaDeco;
  CGRASphere bolaNatal;
  CGRASphere lampadaBranca, lampadaColorida;
  CGRACylinder pneu;
  CGRACylinder carroPneu, carroPneu2, carroPneu3, carroPneu4, arvoreTronco;
  CGRACone cone, arvoreFolhas;
  CGRACompound carroCorpoObj(carroCorpo);
  CGRACompound carroPneuObj(carroPneu);
  CGRACompound carroPneu2Obj(carroPneu2);
  CGRACompound carro2CorpoObj(carro2Corpo);
  CGRACompound carro2PneuObj(carroPneu3);
  CGRACompound carro2Pneu2Obj(carroPneu4);
  CGRACompound arvoreTroncoObj(arvoreTronco);
  CGRACompound arvoreFolhasObj(arvoreFolhas);

  // FBO para POV do condutor
  FrameBuffer fbo;
  TextureBuffer tbo;
  RenderBuffer rbo;
  tbo.AttachToFrameBuffer(fbo);
  rbo.AttachToFrameBuffer(fbo);
  fbo.Validate();

  std::vector<glm::vec3> revoPontos;
  revoPontos.emplace_back(glm::vec3(1.2f, 0.0f, 0.0f));
  revoPontos.emplace_back(glm::vec3(0.5f, 0.0f, 0.0f));

  std::vector<glm::vec3> extrPontos;
  extrPontos.emplace_back(glm::vec3(-1.0f, 2.0f, 0.0f));
  extrPontos.emplace_back(glm::vec3(1.0f, 1.0f, 0.0f));
  extrPontos.emplace_back(glm::vec3(1.0f, -1.0f, 0.0f));
  extrPontos.emplace_back(glm::vec3(-1.0f, -1.0f, 0.0f));

  std::vector<glm::vec3> conePontos;
  conePontos.emplace_back(1.0f, 0.0f, -1.0f);
  conePontos.emplace_back(0.0f, 1.0f, 0.0f);

  CGRARevolution track(revoPontos);
  CGRAExtrusion trophy(extrPontos);
  CGRARevolution cone2(conePontos);

  carroCorpo.setTexture("carrotexture.ppm");
  carro2Corpo.setTexture("carrotexture.ppm");
  sol.setTexture("suntexture.ppm");
  arvoreFolhas.setTexture("leavestexture.ppm");
  //  arvoreFolhas.setChessTexture(true);
  arvoreTronco.setTexture("trunktexture.ppm");
  //  arvoreFolhas.setTexture("treetexture.ppm");
  //  arvoreTronco.setChessTexture(true);
  //  sol.setChessTexture(true);
  pneu.setTexture("pneutexture.ppm");
  //  pneu.setChessTexture(true);
  carroPneu.setTexture("pneutexture.ppm");
  carroPneu2.setTexture("pneutexture.ppm");
  carroPneu3.setTexture("pneutexture.ppm");
  carroPneu4.setTexture("pneutexture.ppm");
  esferaDeco.setTexture("tattoo.ppm");
  cone.setTexture("conetexture.ppm");
  chao.setTexture("grass.ppm");
  track.setTexture("tracktexture.ppm");
  cone2.setTexture("conetexture.ppm");
  // track.setChessTexture(true);
  // chao.setChessTexture(true);
  //  cartaz.setTexture("tattoo.ppm");
  trophy.setTexture("trophytexture.ppm");
  lampadaBranca.setTexture("lamptexture.ppm");
  lampadaColorida.setTexture("lamptexture.ppm");
  bolaNatal.setTexture("trophytexture.ppm");

  track.setShader(basicShader);
  cartaz.setShader(basicShader);
  trophy.setShader(basicShader);
  cone.setShader(basicShader);
  cone2.setShader(basicShader);
  pneu.setShader(basicShader);
  sol.setShader(basicShader);
  chao.setShader(basicShader);
  esferaDeco.setShader(basicShader);
  carroCorpo.setShader(basicShader);
  carro2Corpo.setShader(basicShader);
  carroPneu.setShader(basicShader);
  carroPneu2.setShader(basicShader);
  carroPneu3.setShader(basicShader);
  carroPneu4.setShader(basicShader);
  arvoreFolhas.setShader(basicShader);
  arvoreTronco.setShader(basicShader);
  lampadaColorida.setShader(basicShader);
  lampadaBranca.setShader(basicShader);
  bolaNatal.setShader(basicShader);

  glm::mat4 chaoPosition(1.0f);
  glm::mat4 cartazPosition(1.0f);
  glm::mat4 trackPosition(1.0f);
  glm::mat4 solPosition(1.0f);
  glm::mat4 esferaPosition(1.0f);
  glm::mat4 pneuPosition(1.0f);
  glm::mat4 conePosition(1.0f);
  glm::mat4 cone2Position(1.0f);
  glm::mat4 trophyPosition(1.0f);
  glm::mat4 carroCorpoPosition(1.0f);
  glm::mat4 carro2CorpoPosition(1.0f);
  glm::mat4 carroPneuPosition(1.0f);
  glm::mat4 arvoreFolhasPosition(1.0f);
  glm::mat4 arvoreTroncoPosition(1.0f);
  glm::mat4 lampadaBrancaPosition(1.0f);
  glm::mat4 lampadaColoridaPosition(1.0f);
  glm::mat4 bolaNatalPosition(1.0f);

  // Deslocar objectos na cena.

  cartazPosition = glm::scale(cartazPosition, glm::vec3(5.0f, 2.5f, 3.0f));
  cartazPosition = glm::translate(cartazPosition, glm::vec3(0.0f, 2.0f, .0f));
  cartazPosition = glm::rotate(cartazPosition, glm::degrees(3.14f / 10.0f),
                               glm::vec3(1.0f, 0.0f, 0.0f));
  //  cartazPosition = glm::translate(cartazPosition, glm::vec3(1.5f, 0.0f,
  //  -2.0f));
  /*  cartazPosition = glm::rotate(cartazPosition, glm::degrees(120.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));*/

  chaoPosition = glm::scale(chaoPosition, glm::vec3(25.0f, 25.0f, 25.0f));
  /*  chaoPosition = glm::rotate(chaoPosition, 3.5f,
                               glm::vec3(1.0f, 0.0f, 0.0f));*/
  chaoPosition = glm::translate(chaoPosition, glm::vec3(0.0f, 0.0f, -0.001f));

  solPosition = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 8.0f, -5.0f));

  std::vector<glm::vec3> pneuLocations = {glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(1.0f, 0.0f, 0.0f),
                                          glm::vec3(1.0f, 1.0f, 0.0f)};

  std::vector<glm::vec3> coneLocations = {
      glm::vec3(5.9f, 1.0f, -3.0f), glm::vec3(5.9f, 1.0f, -1.5f),
      glm::vec3(5.9f, 1.0f, 0.0f), glm::vec3(5.9f, 1.0f, 1.5f),
      glm::vec3(5.9f, 1.0f, 3.0f)};

  std::vector<glm::vec3> bolaNatalLocations = {
      glm::vec3(-70.0f, 16.0f, -47.0f), glm::vec3(-75.0f, 16.0f, -51.0f),
      glm::vec3(-65.0f, 13.0f, -51.0f)};

  pneuPosition = glm::scale(pneuPosition, glm::vec3(0.6f, 0.1f, 0.2f));
  pneuPosition = glm::translate(pneuPosition, glm::vec3(-1.0f, 1.5f, 0.0f));

  conePosition = glm::scale(conePosition, glm::vec3(0.3f, 0.7f, 0.3f));
  conePosition = glm::translate(conePosition, glm::vec3(2.0f, 0.0f, 0.0f));

  cone2Position = glm::scale(cone2Position, glm::vec3(0.3f, 0.7f, 0.3f));
  cone2Position = glm::translate(cone2Position, glm::vec3(-5.0f, 0.7f, 0.0f));
  /*  cone2Position = glm::rotate(cone2Position, glm::degrees(90.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f));*/

  trackPosition = glm::translate(trackPosition, glm::vec3(0.0f, 0.01f, 0.0f));
  /*  trackPosition = glm::rotate(trackPosition, glm::degrees(90.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f));*/
  trackPosition = glm::scale(trackPosition, glm::vec3(5.0f, 5.0f, 5.0f));

  trophyPosition = glm::translate(trophyPosition, glm::vec3(0.0f, 1.1f, -1.5f));

  carroCorpoPosition =
      glm::translate(carroCorpoPosition, glm::vec3(3.0f, 1.0f, 0.0f));
  carro2CorpoPosition =
      glm::translate(carro2CorpoPosition, glm::vec3(5.0f, 1.0f, -2.0f));

  // Pneus do carro

  glm::mat4 pneu2corpo(1.0f);
  pneu2corpo = glm::scale(pneu2corpo, glm::vec3(0.1f, 0.5f, 0.1f));
  pneu2corpo = glm::translate(pneu2corpo, glm::vec3(1.0f, -1.5f, 2.5f));
  pneu2corpo =
      glm::rotate(pneu2corpo, glm::degrees(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 pneu22corpo = pneu2corpo;
  pneu22corpo = glm::translate(pneu22corpo, glm::vec3(3.0f, 0.0f, 0.0f));

  // Conectar pneus do carro aos corpos respectivos.

  carroCorpoObj.PushChild(&carroPneuObj, pneu2corpo);
  carroCorpoObj.PushChild(&carroPneu2Obj, pneu22corpo);

  carro2CorpoObj.PushChild(&carro2PneuObj, pneu2corpo);
  carro2CorpoObj.PushChild(&carro2Pneu2Obj, pneu22corpo);

  arvoreFolhasPosition =
      glm::scale(arvoreFolhasPosition, glm::vec3(4.5f, 1.5f, 1.0f));
  arvoreFolhasPosition =
      glm::translate(arvoreFolhasPosition, glm::vec3(0.0f, 0.7f, 0.0f));

  // Conectar folhas da árvore ao tronco.
  arvoreTroncoObj.PushChild(&arvoreFolhasObj, arvoreFolhasPosition);

  arvoreTroncoPosition =
      glm::translate(arvoreTroncoPosition, glm::vec3(-7.0f, 0.6f, -5.0f));
  arvoreTroncoPosition =
      glm::scale(arvoreTroncoPosition, glm::vec3(0.3f, 1.0f, 1.0f));

  lampadaBrancaPosition = glm::scale(lampadaBrancaPosition, glm::vec3(0.5f));
  lampadaBrancaPosition =
      glm::translate(lampadaBrancaPosition, glm::vec3(3.0f, 8.5f, 0.0f));

  lampadaColoridaPosition =
      glm::scale(lampadaColoridaPosition, glm::vec3(0.5f));
  lampadaColoridaPosition =
      glm::translate(lampadaColoridaPosition, glm::vec3(-3.0f, 8.5f, 0.0f));

  esferaPosition = glm::translate(esferaPosition, glm::vec3(0.0f, 1.0f, -1.0f));

  bolaNatalPosition = glm::scale(bolaNatalPosition, glm::vec3(0.1f));
  bolaNatalPosition =
      glm::translate(bolaNatalPosition, glm::vec3(-65.0f, 13.0f, -51.0f));

  // Definir transformadas iniciais.
  chao.setModelTransformation(chaoPosition);
  cartaz.setModelTransformation(cartazPosition);
  sol.setModelTransformation(solPosition);
  esferaDeco.setModelTransformation(esferaPosition);
  pneu.setModelTransformation(pneuPosition);
  cone.setModelTransformation(conePosition);
  cone2.setModelTransformation(cone2Position);
  track.setModelTransformation(trackPosition);
  trophy.setModelTransformation(trophyPosition);
  lampadaBranca.setModelTransformation(lampadaBrancaPosition);
  lampadaColorida.setModelTransformation(lampadaColoridaPosition);
  bolaNatal.setModelTransformation(bolaNatalPosition);
  carroCorpoObj.PropagateModelTransformation(carroCorpoPosition);
  carro2CorpoObj.PropagateModelTransformation(carro2CorpoPosition);
  arvoreTroncoObj.PropagateModelTransformation(arvoreTroncoPosition);

  // Definição de cores.
  glm::vec4 grassColor = glm::vec4(0.3f, 0.5f, 0.27f, 1.0f);
  glm::vec4 treeColor = glm::vec4(0.2f, 0.8f, 0.27f, 1.0f);
  glm::vec4 trunkColor = glm::vec4(0.39f, 0.26f, 0.13f, 1.0f);
  glm::vec4 blackColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  //  glm::vec4 trackColor = glm::vec4(0.8f);
  glm::vec4 trophyColor = glm::vec4(0.97f, 0.65f, 0.01f, 1.0f);
  //  glm::vec4 pneuColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  glm::vec4 solColor = glm::vec4(0.98f, 0.99f, 0.06f, 1.0f);
  glm::vec4 coneColor = glm::vec4(1.0f, 0.49f, 0.0f, 1.0f);
  glm::vec4 carroColor = glm::vec4(0.9f, 0.3f, 0.2f, 1.0f);
  glm::vec4 carro2Color = glm::vec4(0.5f, 0.1f, 0.9f, 1.0f);
  glm::vec4 whiteColor = glm::vec4(0.9f);

  // Definir cores dos objectos
  trophy.SetColor(trophyColor);
  chao.SetColor(grassColor);
  carroCorpo.SetColor(carroColor);
  carro2Corpo.SetColor(carro2Color);
  sol.SetColor(solColor);
  cone2.SetColor(coneColor);
  chao.SetColor(carroColor);
  cartaz.SetColor(whiteColor);
  arvoreFolhas.SetColor(treeColor);
  arvoreTronco.SetColor(trunkColor);
  lampadaBranca.SetColor(whiteColor);
  lampadaColorida.SetColor(blackColor);
  esferaDeco.SetColor(whiteColor);
  bolaNatal.SetColor(whiteColor);

  carroCorpo.SetSpecularReaction(2.0f);
  carro2Corpo.SetSpecularReaction(2.0f);
  bolaNatal.SetSpecularReaction(2.0f);

  carroCorpo.SetShininess(64.0f);
  carro2Corpo.SetShininess(64.0f);
  bolaNatal.SetShininess(128.0f);

  // Preparar câmara para rastrear posição de um condutor.
  glm::vec3 cameraCarroPosition(1.0f);
  glm::vec4 cameraPosition(1.0f);

  float carroTheta = 0.0f;

  glm::mat4 visitorView(1.0f);
  glm::mat4 driverView(1.0f);
  glm::mat4 activeView(1.0f);
  glm::vec4 driverPosition(1.0f);
  glm::vec3 driverPositionVec3(1.0f);
  glm::vec3 driverFront(1.0f);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 carrosPos(1.0f);
    // Incrementar rotação dos carros
    carrosPos = glm::rotate(carrosPos, glm::degrees(0.0001f),
                            glm::vec3(0.0f, 1.0f, 0.0f));

    // Obter posição do piloto (e incrementar y um bocado)
    driverPosition = GetTranslationFromMat4(carroCorpoObj.Object->modeltr);
    driverPositionVec3[0] = driverPosition[0];
    driverPositionVec3[1] =
        driverPosition[1] + 1.0f; // Acrescentar alguma altura ao condutor para
                                  // nao estar dentro do carro
    driverPositionVec3[2] = driverPosition[2];

    // Frente do condutor = up vector x posição condutor
    driverFront = glm::normalize(
        glm::cross(glm::vec3(0.0, 1.0f, 0.0f), driverPositionVec3));

    driverView =
        glm::lookAt(driverPositionVec3, driverPositionVec3 + driverFront,
                    // driverPositionVec3 + glm::vec3(0.0f, 0.0f, -1.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

    // void SetUniform3f(float f0, float f1, float f2, std::string uniformName,
    // DEECShader * shader)
    SetUniform3f(camera.m_Position[0], camera.m_Position[1],
                 camera.m_Position[2], "u_CameraPosition", basicShader);

    // Desenhar no FBO para o ecra.

    fbo.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    activeView = driverView;

    // Desenhar chao
    chao.drawIt(activeView, proj);

    // Desenhar cartaz
    cartaz.drawIt(activeView, proj);

    // Desenhar pista
    track.drawIt(activeView, proj);

    // Desenhar trofeu
    trophy.drawIt(activeView, proj);

    // Desenhar pneus
    for (const auto &elemt : pneuLocations) {
      glm::mat4 location = pneuPosition;
      location = glm::translate(location, elemt);
      pneu.setModelTransformation(location);
      pneu.drawIt(activeView, proj);
    }

    // Desenhar cones
    for (const auto &elemt : coneLocations) {
      glm::mat4 location(1.0f);
      location = glm::translate(location, elemt);
      cone.setModelTransformation(location);
      cone.drawIt(activeView, proj);
    }

    // Desenhar sol
    sol.drawIt(activeView, proj);

    // Desenhar esfera decorativa
    esferaDeco.drawIt(activeView, proj);

    // Desenhar cone (revolucao)
    cone2.drawIt(activeView, proj);

    // Desenhar árvore
    arvoreTroncoObj.DrawTree(activeView, proj);

    // Desenhar bolas de natal (se tiver tempo, refazer objectos hierarquicos
    // para isto estar melhor)
    for (const auto &elemt : bolaNatalLocations) {
      glm::mat4 location(1.0f);
      location = glm::scale(location, glm::vec3(0.1f));
      location = glm::translate(location, elemt);
      bolaNatal.setModelTransformation(location);
      bolaNatal.drawIt(activeView, proj);
    }
    // bolaNatal.drawIt(activeView, proj);

    // Desenhar carros (Ligar componente especular)
    carroCorpoObj.PropagateModelTransformation(carrosPos);
    carro2CorpoObj.PropagateModelTransformation(carrosPos);

    carroCorpoObj.DrawTree(activeView, proj);
    carro2CorpoObj.DrawTree(activeView, proj);

    // Desenhar lampadas
    lampadaBranca.drawIt(activeView, proj);

    // Cor da lampada colorida pode mudar e não dá muito jeito este objecto ser
    // global (tem de ser construído depois do contexto OpenGL), portanto tenho
    // de fazer assim.

    lampadaColorida.SetColor(lampadaColoridaColor);
    lampadaColorida.drawIt(activeView, proj);

    // Desenhar no framebuffer para o ecrã.
    fbo.Unbind();

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw Blue Lens
    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
    /*    if (visitorPOV)
          // Obter matriz de vista do visitor.
          activeView = camera.GetViewMatrix();
        else
          // Definir matriz de vista como a do condutor.
          activeView = driverView;*/

    tbo.Bind();

    // Desenhar cartaz
    cartaz.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    //    tbo.Unbind();

    // Desenhar chao
    chao.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    // Desenhar pista
    track.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    // Desenhar trofeu
    trophy.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    // Desenhar pneus
    for (const auto &elemt : pneuLocations) {
      glm::mat4 location = pneuPosition;
      location = glm::translate(location, elemt);
      pneu.setModelTransformation(location);
      pneu.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);
    }

    // Desenhar cones
    for (const auto &elemt : coneLocations) {
      glm::mat4 location(1.0f);
      location = glm::translate(location, elemt);
      cone.setModelTransformation(location);
      cone.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);
    }

    // Desenhar sol
    sol.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    // Desenhar esfera decorativa
    esferaDeco.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    // Desenhar cone (revolucao)
    cone2.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    // Desenhar árvore
    arvoreTroncoObj.DrawTree(viewerPositionCameraL.GetViewMatrix(), proj);

    // Desenhar bolas de natal (se tiver tempo, refazer objectos hierarquicos
    // para isto estar melhor)
    for (const auto &elemt : bolaNatalLocations) {
      glm::mat4 location(1.0f);
      location = glm::scale(location, glm::vec3(0.1f));
      location = glm::translate(location, elemt);
      bolaNatal.setModelTransformation(location);
      bolaNatal.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);
    }
    // bolaNatal.drawIt(activeView, proj);

    // Desenhar carros (Ligar componente especular)
    carroCorpoObj.PropagateModelTransformation(carrosPos);
    carro2CorpoObj.PropagateModelTransformation(carrosPos);

    carroCorpoObj.DrawTree(viewerPositionCameraL.GetViewMatrix(), proj);
    carro2CorpoObj.DrawTree(viewerPositionCameraL.GetViewMatrix(), proj);

    // Desenhar lampadas
    lampadaBranca.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    // Cor da lampada colorida pode mudar e não dá muito jeito este objecto ser
    // global (tem de ser construído depois do contexto OpenGL), portanto tenho
    // de fazer assim.

    lampadaColorida.SetColor(lampadaColoridaColor);
    lampadaColorida.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    //------------- RED LENS--------------------
    glClear(GL_DEPTH_BUFFER_BIT);

    // Draw Red Lens
    glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
    /*    if (visitorPOV)
          // Obter matriz de vista do visitor.
          activeView = camera.GetViewMatrix();
        else
          // Definir matriz de vista como a do condutor.
          activeView = driverView;*/

    tbo.Bind();

    // Desenhar cartaz
    cartaz.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    //    tbo.Unbind();

    // Desenhar chao
    chao.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    // Desenhar pista
    track.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    // Desenhar trofeu
    trophy.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    // Desenhar pneus
    for (const auto &elemt : pneuLocations) {
      glm::mat4 location = pneuPosition;
      location = glm::translate(location, elemt);
      pneu.setModelTransformation(location);
      pneu.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);
    }

    // Desenhar cones
    for (const auto &elemt : coneLocations) {
      glm::mat4 location(1.0f);
      location = glm::translate(location, elemt);
      cone.setModelTransformation(location);
      cone.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);
    }

    // Desenhar sol
    sol.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    // Desenhar esfera decorativa
    esferaDeco.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    // Desenhar cone (revolucao)
    cone2.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    // Desenhar árvore
    arvoreTroncoObj.DrawTree(viewerPositionCameraR.GetViewMatrix(), proj);

    // Desenhar bolas de natal (se tiver tempo, refazer objectos hierarquicos
    // para isto estar melhor)
    for (const auto &elemt : bolaNatalLocations) {
      glm::mat4 location(1.0f);
      location = glm::scale(location, glm::vec3(0.1f));
      location = glm::translate(location, elemt);
      bolaNatal.setModelTransformation(location);
      bolaNatal.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);
    }
    // bolaNatal.drawIt(activeView, proj);

    // Desenhar carros (Ligar componente especular)
    carroCorpoObj.PropagateModelTransformation(carrosPos);
    carro2CorpoObj.PropagateModelTransformation(carrosPos);

    carroCorpoObj.DrawTree(viewerPositionCameraR.GetViewMatrix(), proj);
    carro2CorpoObj.DrawTree(viewerPositionCameraR.GetViewMatrix(), proj);

    // Desenhar lampadas
    lampadaBranca.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    // Cor da lampada colorida pode mudar e não dá muito jeito este objecto ser
    // global (tem de ser construído depois do contexto OpenGL), portanto tenho
    // de fazer assim.

    lampadaColorida.SetColor(lampadaColoridaColor);
    lampadaColorida.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    glfwSwapBuffers(window);
    glfwPollEvents();
    carroTheta += deltaTime * 0.02f;
  }

  glfwTerminate();
  return 0;
}
