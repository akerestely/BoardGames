#include "Engine/BaseTypes.h"
#include "glm.hpp"
#include "Piece.h"

#include "glew.h"
#include "gtc/matrix_transform.hpp"
#include <vector>

#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

const glm::vec4 kWhite(0.7f, 0.8f, 0.9f, 1.f);
const glm::vec4 kRed(0.9f, 0.2f, 0.1f, 1.f);
const glm::vec4 kCrossColor(0.9f, 0.99f, 0.99f, 1.f);

Piece::Piece() : 
	m_rotation(0),
	m_color(kWhite)
{
}

void Piece::Init(const std::shared_ptr<Engine::GLSLProgram> &program)
{
	m_program = program;
	buildModel();
}

void Piece::Render(const Engine::Camera2D &camera, const glm::vec2 &position /*= glm::vec2()*/)
{
	m_program->Use();
	//update uniforms
	glUniform4fv(m_program->GetUniformLocation("color"), 1, &m_color[0]);
	glm::mat4 mpv = glm::translate(camera.GetCameraMatrix(), glm::vec3(position.x, position.y, 0));
	mpv = glm::rotate(mpv, m_rotation, glm::vec3(0.f, 0.f, 1.f));
	glUniformMatrix4fv(m_program->GetUniformLocation("MVP"), 1, GL_FALSE, &mpv[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glDrawElements(GL_TRIANGLES, m_iboSize, GL_UNSIGNED_INT, 0);

	glUniform4fv(m_program->GetUniformLocation("color"), 1, &kCrossColor[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIdCross);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboIdCross);
	glDrawElements(GL_TRIANGLES, m_iboSizeCross, GL_UNSIGNED_INT, 0);

	m_program->UnUse();
}

void Piece::SetOrientation(Orientation orientation)
{
	if (orientation == Orientation::Cardinal)
		m_rotation = 0.f;
	else
		m_rotation = 45.f;
}

void Piece::SetColor(Color color)
{
	if (color == Color::White)
		m_color = kWhite;
	else
		m_color = kRed;
}

void Piece::buildModel()
{
	const float PI = 3.141592653589793f;
	const uint slices = 8;
	float alpha0 = glm::radians(22.5f);
	float step = 2 * PI / (slices);
	const float radius = 20;
	const float width = 4;

	std::vector<glm::vec2> vertices;
	vertices.reserve(slices * 2);
	for (uint i = 0; i < slices; ++i)
		vertices.emplace_back(radius * cos(alpha0 + i * step), radius * sin(alpha0 + i * step));

	//assign triangle indices
	std::vector<uint> indices;
	for (uint i = 0; i < slices; ++i)
	{
		indices.push_back(i);
		indices.push_back((i + 1) % slices);
		indices.push_back(0);
	}

	m_iboSize = (uint)indices.size();

	//upload to GPU
	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_iboSize, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertices.clear();
	indices.clear();

	vertices.emplace_back(-10,   2);
	vertices.emplace_back(-10,  -2);
	vertices.emplace_back( -2,  -2);
	vertices.emplace_back( -2, -10);
	vertices.emplace_back(  2, -10);
	vertices.emplace_back(  2,  -2);
	vertices.emplace_back( 10,  -2);
	vertices.emplace_back( 10,   2);
	vertices.emplace_back(  2,   2);
	vertices.emplace_back(  2,  10);
	vertices.emplace_back( -2,  10);
	vertices.emplace_back( -2,   2);
	
	indices.insert(indices.begin(), {
		0, 1, 6, 0, 6, 7,
		2, 3, 4, 2, 4, 5,
		8, 9, 10, 8, 10, 11
	});

	m_iboSizeCross = (uint)indices.size();

	//upload to GPU
	glGenBuffers(1, &m_vboIdCross);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIdCross);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_iboIdCross);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboIdCross);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_iboSizeCross, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
