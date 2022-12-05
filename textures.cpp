// Textures
#include "Camera.cpp"
/*#include "compoundObjects.cpp"
#include "compoundObjects.hpp"*/
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

#include "cgraobject.cpp"
#include "deecshader.h"

#define GLEW_STATIC

Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f,
              0.0f);

/*Camera cameraCarro(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f),
   0.0f, 0.0f);*/

DEECShader *basicShader;
GLFWwindow *window;
bool wire = false, visitorPOV = true;
float yaw = -90.0f;
// float lastX = 400, lastY = 300, yaw = -90.0f, pitch = 0.0f;
float deltaTime = 0.0f, lastFrame = 0.0f;
bool firstMouse = true;
float cameraSpeed = 1.0f;

/*void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_TRUE)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_TRUE)
    camera.m_Position += camera.m_Front * cameraSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_TRUE)
    camera.m_Position -= camera.m_Front * cameraSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_TRUE)
    camera.m_Position += camera.m_Right * cameraSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_TRUE)
    camera.m_Position -= camera.m_Right * cameraSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_TRUE) {
    wire = !wire;
    (wire) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
           : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}*/

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

void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_TRUE)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_TRUE)
    camera.m_Position += camera.m_Front * cameraSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_TRUE)
    camera.m_Position -= camera.m_Front * cameraSpeed * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_TRUE) {
    yaw -= cameraSpeed * deltaTime * 80;
    glm::vec3 direction(1.0f);
    direction[0] = cos(glm::radians(yaw));
    direction[1] = 0.0f;
    direction[2] = sin(glm::radians(yaw));
    camera.m_Front = glm::normalize(direction);
    camera.m_Right =
        glm::normalize(glm::cross(camera.m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
    /*    std::cout << "Yaw: " << yaw << "\n";
        std::cout << "CameraUp: " << camera.m_Up[0] <<", " << camera.m_Up[1] <<
       ", " << camera.m_Up[2] <<".\n";*/
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_TRUE) {
    yaw += cameraSpeed * deltaTime * 80;
    glm::vec3 direction(1.0f);
    direction[0] = std::cos(glm::radians(yaw));
    direction[1] = 0.0f;
    direction[2] = std::sin(glm::radians(yaw));
    camera.m_Front = glm::normalize(direction);
    camera.m_Right =
        glm::normalize(glm::cross(camera.m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
    /*    std::cout << "Yaw: " << yaw << "\n";
        std::cout << "CameraUp: " << camera.m_Up[0] <<", " << camera.m_Up[1] <<
       ", " << camera.m_Up[2] <<".\n";*/
  }
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_TRUE) {
    wire = !wire;
    (wire) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
           : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_TRUE)
    visitorPOV = !visitorPOV;
}

/*void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  camera.m_Front = glm::normalize(direction);
  camera.m_Right =
      glm::normalize(glm::cross(camera.m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
}*/

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
  window = glfwCreateWindow(640, 480, "Projecto 1 - Pedro Silva", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, KeyCallback);

//  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  //  glfwSetCursorPosCallback(window, mouse_callback);

  if (glewInit() != GLEW_OK) {
    std::cout << "Error initializing GLEW.\n";
    return -1;
  }

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

  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  // Ligar teste de profundidade. O fragmento com menor profundidade será
  // desenhado.

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Cor do céu. Aproveitar para o cenário
  glClearColor(0.53f, 0.8f, 0.92f, 1.0f);

  // Matriz de projecção: perspectiva com near plane = 0.5f e far plane = 30.0f. FOV = 80º
  glm::mat4 proj = glm::perspective(glm::radians(80.0f), 1.0f, 0.5f, 30.0f);

  // Declarar objectos
  CGRACone cubo;
  


  // Definir transformadas iniciais.
  glm::mat4 cubePosition(1.0f);
//  cubePosition = glm::translate(cubePosition, glm::vec3(0.0f, 0.0f, 3.0f));
//  cubo.setModelTransformation(cubePosition);
  // Definição de cores.
  glm::vec4 grassColor = glm::vec4(0.3f, 0.5f, 0.27f, 1.0f);


  // Transmitir cores como variáveis uniformes
  cubo.SetColor(grassColor);
  cubo.setTexture("chessboard.ppm");
//  cubo.setChessTexture(true);
  cubo.setShader(basicShader);


  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cubePosition = glm::rotate(cubePosition, glm::degrees(0.0001f), glm::vec3(0.0f, 1.0f, 0.0f));
    cubo.setModelTransformation(cubePosition);

    cubo.drawIt(camera.GetViewMatrix(), proj);


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}