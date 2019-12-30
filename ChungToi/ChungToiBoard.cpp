#include "glm.hpp"
#include "ChungToiBoard.h"

#include "glew.h"
#include "gtc/matrix_transform.hpp"
#include <vector>

#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

void ChungToiBoard::Init(const std::shared_ptr<Engine::GLSLProgram> &program)
{
	m_program = program;
	buildModel();
}

void ChungToiBoard::Render(const Engine::Camera2D &camera, const glm::vec2 &position /*= glm::vec2()*/)
{
	m_program->Use();
	//update uniforms
	glUniform4f(m_program->GetUniformLocation("color"), 222.f/255.f, 184.f/255.f, 135.f/255.f, 1.0f);
	glm::mat4 mpv = glm::translate(camera.GetCameraMatrix(), glm::vec3(position.x, position.y, 0));
	glUniformMatrix4fv(m_program->GetUniformLocation("MVP"), 1, GL_FALSE, &mpv[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glDrawElements(GL_TRIANGLES, m_iboSize, GL_UNSIGNED_INT, 0);
	m_program->UnUse();
}

void ChungToiBoard::buildModel()
{
	//construct vertex data
	std::vector<glm::vec2> vertices;
	// grid
	vertices.emplace_back(-100, -100);
	vertices.emplace_back( 100, -100);
	vertices.emplace_back( 100,  100);
	vertices.emplace_back(-100,  100);
	vertices.emplace_back(-95, -95);
	vertices.emplace_back( 95, -95);
	vertices.emplace_back( 95,  95);
	vertices.emplace_back(-95,  95);

	vertices.emplace_back(-30, -95);
	vertices.emplace_back(-35, -95);
	vertices.emplace_back(-35, 95);
	vertices.emplace_back(-30, 95);

	vertices.emplace_back(30, -95);
	vertices.emplace_back(35, -95);
	vertices.emplace_back(35, 95);
	vertices.emplace_back(30, 95);

	vertices.emplace_back(-95, -30);
	vertices.emplace_back(95, -30);
	vertices.emplace_back(95, -35);
	vertices.emplace_back(-95, -35);

	vertices.emplace_back(-95, 30);
	vertices.emplace_back(95, 30);
	vertices.emplace_back(95, 35);
	vertices.emplace_back(-95, 35);

	//assign triangle indices
	std::vector<uint32_t> indices;
	indices.insert(indices.begin(), {
		0, 1, 5, 0, 5, 4,
		1, 2, 6, 1, 6, 5,
		2, 3, 7, 2, 7, 6,
		3, 0, 4, 3, 4, 7,

		8,  9, 10,  8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	});
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
