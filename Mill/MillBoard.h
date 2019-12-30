#pragma once

#include "BoardGame/IRenderable.h"

#include <cstdint>

class MillBoard : public IRenderable
{
public:
	MillBoard() = default;
	virtual void Init(const std::shared_ptr<Engine::GLSLProgram> &program) override;
	virtual void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2()) override;

private:
	void buildModel();

private:
	std::shared_ptr<Engine::GLSLProgram> m_program;
	uint32_t m_vboId, m_iboId;
	uint32_t m_iboSize;
};
