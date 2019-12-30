#include "glm.hpp"
#include "MillBoard.h"

#include "glew.h"
#include "gtc/matrix_transform.hpp"
#include <vector>

#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

void MillBoard::Init(const std::shared_ptr<Engine::GLSLProgram> &program)
{
	m_program = program;
	buildModel();
}

void MillBoard::Render(const Engine::Camera2D &camera, const glm::vec2 &position /*= glm::vec2()*/)
{
	m_program->Use();
	//update uniforms
	glUniform4f(m_program->GetUniformLocation("color"), 139.f / 255.f, 69.f / 255.f, 19.f / 255.f, 1.0f);
	glm::mat4 mpv = glm::translate(camera.GetCameraMatrix(), glm::vec3(position.x, position.y, 0));
	glUniformMatrix4fv(m_program->GetUniformLocation("MVP"), 1, GL_FALSE, &mpv[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glDrawElements(GL_TRIANGLES, m_iboSize, GL_UNSIGNED_INT, 0);
	m_program->UnUse();
}

void MillBoard::buildModel()
{
	//construct vertex data
	std::vector<glm::vec2> vertices;
	// outer
	vertices.emplace_back(-183,  183);
	vertices.emplace_back(-183, -183);
	vertices.emplace_back(-177, -183);
	vertices.emplace_back(-177,  183);
	vertices.emplace_back(-183, -183);
	vertices.emplace_back( 183, -183);
	vertices.emplace_back( 183, -177);
	vertices.emplace_back(-183, -177);
	vertices.emplace_back(183,  183);
	vertices.emplace_back(183, -183);
	vertices.emplace_back(177, -183);
	vertices.emplace_back(177,  183);
	vertices.emplace_back(-183, 183);
	vertices.emplace_back( 183, 183);
	vertices.emplace_back( 183, 177);
	vertices.emplace_back(-183, 177);
	// middle, and inner
	uint32_t nVertPerLayer = (uint32_t)vertices.size();
	const uint32_t kWidth = 60;
	for(uint32_t i = 1; i <= 2; ++i)
		for (uint32_t j = 0; j < nVertPerLayer; ++j)
		{
			auto &vert = vertices[j];
			vertices.emplace_back(vert.x - glm::sign(vert.x) * kWidth * i, vert.y - glm::sign(vert.y) * kWidth * i);
		}
	// 'cross'
	vertices.emplace_back(-183, 3);
	vertices.emplace_back(-183, -3);
	vertices.emplace_back(-63, -3);
	vertices.emplace_back(-63, 3);
	vertices.emplace_back(183, 3);
	vertices.emplace_back(183, -3);
	vertices.emplace_back(63, -3);
	vertices.emplace_back(63, 3);
	vertices.emplace_back(3, 183);
	vertices.emplace_back(-3, 183);
	vertices.emplace_back(-3, 63);
	vertices.emplace_back(3, 63);
	vertices.emplace_back(3, -183);
	vertices.emplace_back(-3, -183);
	vertices.emplace_back(-3, -63);
	vertices.emplace_back(3, -63);

	//assign triangle indices
	std::vector<uint32_t> indices;	
	for (uint32_t i = 0, sIndex = 0; i < 16; ++i, sIndex += 4)
		indices.insert(indices.end(), { sIndex, sIndex + 1, sIndex + 2, sIndex, sIndex + 2, sIndex + 3 });

	m_iboSize = (uint32_t)indices.size();

	//upload to GPU
	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_iboSize, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
