#include "ObjectHierarchy.hpp"

ObjectTree::ObjectTree(const GLObject &Object) {
  ObjectData = Object;
  TransformFromMother =
      glm::mat4(1.0f); // Set initial transform to identity matrix
}

ObjectTree::ObjectTree(const GLObject &Object, glm::mat4 &Transform) {
  ObjectData = Object;
  TransformFromMother = Transform;
}

ObjectTree::~ObjectTree() {}

void ObjectTree::PushObject(const ObjectTree &son) { Children.push_back(son); }

void ObjectTree::Draw(Renderer &renderer, Shaders &shaders, glm::mat4 model){
  //shaders.SetUniformMat4f("u_MVP", mvp);
  shaders.SetUniformMat4f("u_Model", TransformFromMother);
  renderer.Draw(ObjectData.m_VA, ObjectData.m_IB, shaders);
}

void ObjectTree::DrawTree(Renderer &renderer, Shaders &shaders, glm::mat4 model) {
  for (auto &elemt : Children) {
  //  model = TransformFromMother * model;
//    shaders.SetUniformMat4f("u_Model", model);
    elemt.DrawTree(renderer, shaders, model);
  }
  //mvp = mvp * TransformFromMother;
  model = TransformFromMother * model;
  shaders.SetUniformMat4f("u_Model", model);
  renderer.Draw(ObjectData.m_VA, ObjectData.m_IB, shaders);
}