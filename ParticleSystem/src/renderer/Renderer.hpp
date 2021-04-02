#pragma once

#include <memory>
#include <chrono>
#include <glm/glm.hpp>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

class Renderer
{
private:
	class Data
	{
	private:
		float* vertices;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		VertexArray vertexArray;

		size_t maxQuadCount;
		size_t maxVertexCount;
		size_t maxIndexCount;
		size_t stride;

		size_t quadIndex = 0;
		size_t vertexIndex = 0;
		size_t offset = 0;

		static const glm::vec4 positions[4];
	public:
		Data(size_t quadCount);
		~Data();

		void SetPosition(const glm::mat4& modelMatrix);
		void SetColor(const glm::vec4& color);
		void NextVertex();

		bool CanDraw();
		bool IsFull();

		void Draw();
		void Reset();
	};

	struct Statistics
	{
		unsigned int drawCalls = 0;
		std::chrono::milliseconds renderTime;
	};

	Renderer::Data* data = nullptr;
	bool isScene = false;
	Shader shader;
	glm::mat4 viewProjectionMatrix;
	Statistics statistics;

	void Flush();

	static Renderer* renderer;
public:
	static const int quadCountLimit = 25000;

	Renderer(size_t maxQuadCount);
	~Renderer();

	void BeginScene();
	void EndScene();

	static void Setup(size_t maxQuadCount);

	static const Statistics& GetStatistics();

	static void DrawQuad(const glm::mat4 modelMatrix, const glm::vec4& color);
};
