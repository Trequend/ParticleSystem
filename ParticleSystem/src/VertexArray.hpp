#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

class VertexArray
{
private:
	size_t vao;
	size_t attribArrayIndex = 0;

	size_t BufferElementTypeToOpenGLType(BufferElement::Type type);
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void Unbind();

	void AddVertexBuffer(const VertexBuffer& buffer);
	void SetIndexBuffer(const IndexBuffer& buffer);
};
