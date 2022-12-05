#include "compoundObjects.hpp"

compoundObjects::compoundObjects(CGRAobject &CGRAobject) {
  Object = &CGRAobject;
  TransformFromMother =
      glm::mat4(1.0f); // Set initial transform to identity matrix
}

compoundObjects::~compoundObjects() {}

void compoundObjects::PushChild(compoundObjects *Child, glm::mat4 connection) {
  //	Child->TransformFromMother = connection;
  Child->Object->modeltr = connection * Child->Object->modeltr;
  Children.push_back(Child);
}

void compoundObjects::DrawTree(glm::mat4 V, glm::mat4 P,
                               glm::mat4 previousModel = glm::mat4(1.0f)) {
  Object->drawIt(V, P);
  for (const auto &elemt : Children) {
    elemt->Object->drawIt(V, P);
  }
}

void compoundObjects::SetTreeTransformation(glm::mat4 &Transformation) {
  Object->setModelTransformation(Transformation);
  for (const auto &elemt : Children) {
  	glm::mat4 newModeltr = Transformation * elemt->Object->modeltr;
//  	elemt->Object->setModelTransformation(newModeltr);
  }
}
