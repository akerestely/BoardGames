#include "Engine/BaseTypes.h"
#include "glm/glm.hpp"
#include "Nought.h"

#include <vector>
#include "gl/glew.h"

#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

Nought::Nought()
{
}

Nought::~Nought()
{
}

void Nought::Init(const std::shared_ptr<Engine::GLSLProgram>& program)
{
	this->program = program;
	buildModel();
}

void Nought::Render(const Engine::Camera2D &camera, const glm::vec2 &position /*= glm::vec2()*/)
{
	program->Use();
	//update uniforms
	glUniform4f(program->GetUniformLocation("color"), 0.5, 0.5, 0.7, 1);
	glm::mat4 mpv = glm::translate(camera.GetCameraMatrix(), glm::vec3(position.x, position.y, 0));
	glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &mpv[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glDrawElements(GL_TRIANGLES, iboSize, GL_UNSIGNED_INT, 0);
	program->UnUse();
}

void Nought::buildModel()
{
	//construct vertex data
	const float PI = 3.141592653589793;
	const uint slices = 40;
	float step = 2 * PI / (slices);
	const float radius = 20;
	const float width = 4;

	std::vector<glm::vec2> vertices;
	vertices.reserve(slices * 2);
	for (float alpha = 0; alpha <= 2 * PI; alpha += step)
		vertices.emplace_back(radius * cos(alpha), radius * sin(alpha));
	for (float alpha = 0; alpha <= 2 * PI; alpha += step)
		vertices.emplace_back((radius - width) * cos(alpha), (radius - width) * sin(alpha));

	//assign triangle indices
	std::vector<uint> indices;
	for (uint i = 0; i < slices; ++i)
	{
		indices.push_back(i);
		indices.push_back((i + 1) % slices);
		indices.push_back(slices + (i + 1) % slices);
		indices.push_back(i);
		indices.push_back(slices + (i + 1) % slices);
		indices.push_back(slices + i);
	}

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
