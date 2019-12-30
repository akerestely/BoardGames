#include "glm.hpp"
#include "TicTacToeBoard.h"

#include <vector>
#include "gtc/matrix_transform.hpp"
#include "glew.h"

#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

void TicTacToeBoard::Init(const std::shared_ptr<Engine::GLSLProgram> &program)
{
	this->program = program;
	buildModel();
}

void TicTacToeBoard::Render(const Engine::Camera2D &camera, const glm::vec2 &position/* = glm::vec2()*/)
{
	program->Use();
	//update uniforms
	glUniform4f(program->GetUniformLocation("color"), 0.75f, 0.9f, 0.9f, 1.0f);
	glm::mat4 mpv = glm::translate(camera.GetCameraMatrix(), glm::vec3(position.x, position.y, 0));
	glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &mpv[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glDrawElements(GL_TRIANGLES, iboSize, GL_UNSIGNED_INT, 0);
	program->UnUse();
}

void TicTacToeBoard::buildModel()
{
	//construct vertex data
	std::vector<glm::vec2> vertices;
	// grid
	vertices.emplace_back(-30, -95);
	vertices.emplace_back(-35, -95);
	vertices.emplace_back(-35,  95);
	vertices.emplace_back(-30,  95);

	vertices.emplace_back(30, -95);
	vertices.emplace_back(35, -95);
	vertices.emplace_back(35,  95);
	vertices.emplace_back(30,  95);

	vertices.emplace_back(-95, -30);
	vertices.emplace_back( 95, -30);
	vertices.emplace_back( 95, -35);
	vertices.emplace_back(-95, -35);

	vertices.emplace_back(-95, 30);
	vertices.emplace_back( 95, 30);
	vertices.emplace_back( 95, 35);
	vertices.emplace_back(-95, 35);

	//assign triangle indices
	std::vector<uint32_t> indices;
	indices.insert(indices.begin(), {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
	});
	iboSize = (uint32_t)indices.size();

	//upload to GPU
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * iboSize, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
