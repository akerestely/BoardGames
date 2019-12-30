#include "Graph.h"

#include <algorithm>

#include "glew.h"
#include "gtc/matrix_transform.hpp"

#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

Graph::~Graph()
{
	glDeleteBuffers(1, &m_nVboId);
	glDeleteBuffers(1, &m_nIboId);
}

void Graph::Init(const std::shared_ptr<Engine::GLSLProgram> &program)
{
	m_program = program;
	glGenBuffers(1, &m_nVboId);
	glGenBuffers(1, &m_nIboId);
}

void Graph::SetValues(const std::vector<float> &values1, const std::vector<float> &values2)
{
	//construct vertex data
	std::vector<glm::vec2> vertices;
	vertices.emplace_back(-0.8f, 0.4f);
	vertices.emplace_back(-0.8f, -0.45f);
	vertices.emplace_back(-0.85f, -0.4f);
	vertices.emplace_back(-0.1f, -0.4f);

	float sizeX = 0.6f / (values1.size() - 1);
	for (uint32_t i = 0; i < values1.size(); ++i)
	{
		vertices.emplace_back(-0.75f + i * sizeX, values1[i] * 0.78f - 0.39f);
	}
	for (uint32_t i = 0; i < values2.size(); ++i)
	{
		vertices.emplace_back(-0.75f + i * sizeX, values2[i] * 0.78f - 0.39f);
	}

	//assign triangle indices
	std::vector<uint32_t> indices;
	indices.insert(indices.end(), {
		0, 1,
		2, 3
	});
	m_nIboSize = (uint32_t)indices.size();

	uint32_t lastVal = indices.back() + 1;
	for (uint32_t i = 1; i < values1.size(); ++i)
		indices.insert(indices.end(), { lastVal, ++lastVal });
	m_nIboSize1 = (uint32_t)indices.size();

	lastVal = indices.back() + 1;
	for (uint32_t i = 1; i < values2.size(); ++i)
		indices.insert(indices.end(), { lastVal, ++lastVal });
	m_nIboSize2 = (uint32_t)indices.size();

	//upload to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_nVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_nIboSize2, &indices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Graph::Render(const Engine::Camera2D &camera, const glm::vec2 &position /*= glm::vec2()*/)
{
	m_program->Use();
	//update uniforms
	glUniform4f(m_program->GetUniformLocation("color"), 0.5f, 0.5f, 0.7f, 1.0f);
	glm::mat4 mpv = glm::translate(camera.GetCameraMatrix(), glm::vec3(position.x, position.y, 0));
	m_program->UploadUniform("MVP", mpv);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIboId);

	glLineWidth(5);
	glDrawElements(GL_LINES, m_nIboSize, GL_UNSIGNED_INT, 0);

	glUniform4f(m_program->GetUniformLocation("color"), 34 / 255.f, 139 / 255.f, 34 / 255.f, 1.0f);
	glLineWidth(3);
	glDrawElements(GL_LINES, m_nIboSize1 - m_nIboSize, GL_UNSIGNED_INT, (void*)(m_nIboSize * sizeof(uint32_t)));

	glUniform4f(m_program->GetUniformLocation("color"), 255 / 255.f, 69 / 255.f, 0 / 255.f, 1.0f);
	glLineWidth(3);
	glDrawElements(GL_LINES, m_nIboSize2 - m_nIboSize1, GL_UNSIGNED_INT, (void*)(m_nIboSize1 * sizeof(uint32_t)));
	m_program->UnUse();
}
