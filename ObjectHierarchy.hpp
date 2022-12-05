#pragma once
#include <vector>
#include "GLObject.hpp"
//#include "Renderer.cpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL

#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif
/*---------------------------------------------------------------------------------------------------------------
Object Hierarchy: I'm basically describing a tree with each node having N branches? Ie an expansion of  binary tree.
	- Constructor accepts one base object. This object will be central to the hierarchy -- all objects will be 
	relative to this one.
	- Each object will have N connections describing the transform between the base object and	the connected object.
	- These connections will be stored in a std::vector. To push an object, you must specify the object to connect
	 to with vector coordinates + a GLObject object + a glm::vec3 describing the rigid transform.
	- To draw, traverse the whole data structure (recursively? I guess.) and accumulate transform matrices described
	by the connections between objects. 
	- If an object has no connections to other objects, it is a leaf object.
----------------------------------------------------------------------------------------------------------------*/


class ObjectTree {
public:
	ObjectTree(const GLObject &Object);
	ObjectTree(const GLObject &Object, glm::mat4 &Transform);
	~ObjectTree();

	GLObject ObjectData;
	glm::mat4 TransformFromMother;
	std::vector<ObjectTree> Children;
	std::string name;

	//void PushObject(const GLObject Object, const glm::mat4 TransformToChild);
	void PushObject(const ObjectTree & obj);
	void Draw(Renderer &renderer, Shaders &shader, glm::mat4 model);
	void DrawTree(Renderer &renderer, Shaders &shaders, glm::mat4 model);	// Could do it like this or I could have the Renderer go through the tree recursively
//	void DrawTree(Renderer &renderer, Shaders &shaders, glm::mat4 model);	// Could do it like this or I could have the Renderer go through the tree recursively
};


