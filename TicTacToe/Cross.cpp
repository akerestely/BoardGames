#include "Engine/BaseTypes.h"
#include "glm/glm.hpp"
#include "Cross.h"

#include <vector>
#include "gl/glew.h"

#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

Cross::Cross()
{
}

Cross::~Cross()
{
}

void Cross::Init(const std::shared_ptr<Engine::GLSLProgram>& program)
{
	this->program = program;
	buildModel();
}

void Cross::Render(const Engine::Camera2D &camera, const glm::vec2 &position /*= glm::vec2()*/)
{
	program->Use();
	//update uniforms
	glUniform4f(program->GetUniformLocation("color"), 0.2, 0.3, 0.3, 1);
	glm::mat4 mpv = glm::translate(camera.GetCameraMatrix(), glm::vec3(position.x, position.y, 0));
	glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &mpv[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glDrawElements(GL_TRIANGLES, iboSize, GL_UNSIGNED_INT, 0);
	program->UnUse();
}

void Cross::buildModel()
{
	//construct vertex data
	std::vector<glm::vec2> vertices;
	vertices.emplace_back(-16, -19);
	vertices.emplace_back(19, 16);
	vertices.emplace_back(16, 19);
	vertices.emplace_back(-19, -16);

	vertices.emplace_back(-19, 16);
	vertices.emplace_back(16, -19);
	vertices.emplace_back(19, -16);
	vertices.emplace_back(-16, 19);

	//assign triangle indices
	std::vector<uint> indices;
	indices.insert(indices.begin(), {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7
	});
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
