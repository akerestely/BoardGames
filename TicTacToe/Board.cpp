#include "Engine/BaseTypes.h"
#include "Board.h"

#include <vector>
#include "glm/glm.hpp"
#include "gl/glew.h"

Board::Board()
{
}

Board::~Board()
{
}

void Board::Init()
{
	initShader();
	buildModel();
}

void Board::Render(Engine::Camera2D &camera)
{
	program.Use();
	//update uniforms
	glUniformMatrix4fv(program.GetUniformLocation("MVP"), 1, GL_FALSE, &camera.GetCameraMatrix()[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glDrawElements(GL_TRIANGLES, iboSize, GL_UNSIGNED_INT, 0);
	program.UnUse();
}

void Board::initShader()
{
	program.CompileShaders("Shaders/simpleShading.vert", "Shaders/simpleShading.frag");
	program.AddAttribute("vertexPosition");
	program.LinkShader();

	program.Use();
	glUniform4f(program.GetUniformLocation("color"), 1, 0, 0, 1);
	program.UnUse();
}

void Board::buildModel()
{
	//const uint kRows = 3, kCols = 3;
	//construct vertex data
	std::vector<glm::vec2> vertices;
	//vertices.reserve(kRows * kCols);
	vertices.emplace_back(-100, -100);
	vertices.emplace_back( 100, -100);
	vertices.emplace_back( 100,  100);
	vertices.emplace_back(-100,  100);

	//assign triangle indices. Two triangles at once, that form a rectangle.
	std::vector<uint> indices;
	indices.insert(indices.begin(), { 0, 1, 2, 0, 2, 3 });
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
