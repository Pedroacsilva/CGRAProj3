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

Camera camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f,
              0.0f);
float AmbientColor[3] = {1.0f, 1.0f,
                         1.0f}; // Por norma, luz branca (mudar eventualmente
                                // para cenario nighttime).

DEECShader *basicShader;
GLFWwindow *window;
bool wire = false;
float deltaTime = 0.0f, lastFrame = 0.0f;
float cameraSpeed = 4.0f;
float yaw = -90.0f;
const float displacement = 0.05f;

glm::vec3 viewerPosition(0.0f, 1.0f, 5.0f);
const glm::vec3 PointOfInterest (0.0f);
const glm::vec3 upVector (0.0f, 1.0f, 0.0f);
glm::vec3 viewerRight = glm::normalize(glm::cross(- (viewerPosition - PointOfInterest), upVector));

Camera viewerPositionCameraL(viewerPosition - glm::vec3(displacement, 0.0f, 0.0f), upVector, 0.0f,
                            0.0f);
Camera viewerPositionCameraR(viewerPosition + glm::vec3(displacement, 0.0f, 0.0f), upVector, 0.0f,
                            0.0f);
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
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_TRUE) {
    wire = !wire;
    (wire) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
           : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
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
}

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
  window = glfwCreateWindow(640, 480, "12.1 - Pedro Silva", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  if (glewInit() != GLEW_OK) {
    std::cout << "Error initializing GLEW.\n";
    return -1;
  }
  glfwSetKeyCallback(window, KeyCallback);

  // Carregar & linkar shaders

  basicShader = new DEECShader;
  std::cout << "loading shaders.\n";
  if (basicShader->loadShaders("texture.vert", "texture.frag") == GL_FALSE) {
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

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glm::mat4 proj = glm::perspective(glm::radians(80.0f), 1.0f, 0.5f, 30.0f);
  glm::mat4 ortho_proj = glm::ortho(-0.50f, 0.50f, 0.50f, -0.50f, 0.0f, 1.0f);

/*  // FBO para background image
  FrameBuffer fbo;
  TextureBuffer tbo;
  RenderBuffer rbo;
  tbo.AttachToFrameBuffer(fbo);
  rbo.AttachToFrameBuffer(fbo);
  fbo.Validate();*/

  CGRACube cubo;
  CGRASphere esfera;
  CGRABackground cartaz;
  glm::mat4 cubePosition(1.0f), spherePosition(1.0f), cartazPosition(1.0f);
  cubo.setShader(basicShader);
  esfera.setShader(basicShader);
  cartaz.setShader(basicShader);

  spherePosition = glm::translate(spherePosition, glm::vec3(-2.0f, 0.5f, 0.0f));

  cubo.setModelTransformation(cubePosition);
  esfera.setModelTransformation(spherePosition);
  cartaz.setModelTransformation(cartazPosition);

  cartaz.setTexture("grass.ppm");



  glClearColor(0.00f, 0.00f, 0.00f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cartaz.drawIt(camera.GetViewMatrix(), proj);

    // Draw Blue Lens
    glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);


    cubo.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);
    esfera.drawIt(viewerPositionCameraL.GetViewMatrix(), proj);

    glClear(GL_DEPTH_BUFFER_BIT);
    // Draw Red Lens
    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);

    cubo.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);
    esfera.drawIt(viewerPositionCameraR.GetViewMatrix(), proj);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
