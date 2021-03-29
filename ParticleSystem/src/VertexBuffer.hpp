#pragma once

#include "BufferLayout.h"

class VertexBuffer
{
private:
	size_t buffer;
	BufferLayout layout;
public:
	VertexBuffer(void* data, size_t size, const BufferLayout& layout);
	VertexBuffer(size_t size, const BufferLayout& layout);
	~VertexBuffer();

	void Bind();
	void Unbind();

	const BufferLayout& GetLayout();
};
