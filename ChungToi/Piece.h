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
	
	enum class Orientation
	{
		Cardinal,
		Diagonal
	};

public:
	Piece();
	virtual void Init(const std::shared_ptr<Engine::GLSLProgram> &program) override;
	virtual void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2()) override;

	void SetOrientation(Orientation orientation);
	void SetColor(Color color);

private:
	void buildModel();

private:
	float m_rotation;
	glm::vec4 m_color;

	std::shared_ptr<Engine::GLSLProgram> m_program;
	uint32_t m_vboId, m_iboId;
	uint32_t m_iboSize;

	uint32_t m_vboIdCross, m_iboIdCross;
	uint32_t m_iboSizeCross;
};
