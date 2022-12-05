#pragma once

#ifndef GL_SETUP
#include <GL/glew.h>
#include <GL/gl.h>
#define GL_SETUP
#endif

class IndexBuffer {
	unsigned int m_RendererID;
	unsigned int m_Count;
public:

	IndexBuffer();
	IndexBuffer(const void * data, unsigned int count);
	~IndexBuffer();

	void Push(const void * data, unsigned int count);
	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }
	unsigned int GetID() const {return m_RendererID;}
	
};