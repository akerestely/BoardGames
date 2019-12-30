#pragma once
#include "BoardGame\IRenderable.h"

class Piece : public IRenderable
{
public:
	enum class Color
	{
		White,
		Red
	};

public:
	Piece();
	virtual void Init(const std::shared_ptr<Engine::GLSLProgram> &program) override;
	virtual void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2()) override;

	void SetColor(Color color);

private:
	void buildModel();

private:
	glm::vec4 m_color;

	std::shared_ptr<Engine::GLSLProgram> m_program;
	uint32_t m_vboId, m_iboId;
	uint32_t m_iboSize;
};
