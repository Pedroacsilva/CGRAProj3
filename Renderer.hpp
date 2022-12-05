#pragma once

#include "IndexBuffer.hpp"
#include "ObjectHierarchy.hpp"
#include "Shaders.hpp"
#include "VertexArray.hpp"


class Renderer {
public:

	Renderer();
	~Renderer();

	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shaders& shader) const;
	//void Draw(const ObjectTree& ObjStruct, const Shaders& shader) const;
};