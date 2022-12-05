#include "Camera.hpp"

#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

Camera::~Camera() {}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
