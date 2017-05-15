#include "Engine/BaseTypes.h"
#include "Board.h"

#include <vector>
#include "glm/glm.hpp"
#include "gl/glew.h"

#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

Board::Board()
{
}

Board::~Board()
{
}

void Board::Init(const std::shared_ptr<Engine::GLSLProgram> &program)
{
	this->program = program;
	buildModel();
}

void Board::Render(Engine::Camera2D &camera)
{
	program->Use();
	//update uniforms
	glUniform4f(program->GetUniformLocation("color"), 0.75, 0.9, 0.9, 1);
	glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &camera.GetCameraMatrix()[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glDrawElements(GL_TRIANGLES, iboSize, GL_UNSIGNED_INT, 0);
	program->UnUse();
}

void Board::buildModel()
{
	//construct vertex data
	std::vector<glm::vec2> vertices;
	// grid
// 	vertices.emplace_back(-100, -100);
// 	vertices.emplace_back( 100, -100);
// 	vertices.emplace_back( 100,  100);
// 	vertices.emplace_back(-100,  100);
// 	vertices.emplace_back(-95, -95);
// 	vertices.emplace_back( 95, -95);
// 	vertices.emplace_back( 95,  95);
// 	vertices.emplace_back(-95,  95);

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
	std::vector<uint> indices;
	// grid
	indices.insert(indices.begin(), {
// 		0, 1, 5, 0, 5, 4,
// 		1, 2, 6, 1, 6, 5,
// 		2, 3, 7, 2, 7, 6,
// 		3, 0, 4, 3, 4, 7,

		 8,  9, 10,  8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	});
	for (auto &i : indices)
		i -= 8;
	iboSize = indices.size();

	//upload to GPU
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * iboSize, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
