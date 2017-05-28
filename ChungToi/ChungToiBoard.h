#pragma once
#include "BoardGame/IRenderable.h"

class ChungToiBoard : public IRenderable
{
public:
	virtual void Init(const std::shared_ptr<Engine::GLSLProgram> &program) override;
	virtual void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2()) override;

private:
	void buildModel();

private:
	std::shared_ptr<Engine::GLSLProgram> m_program;
	uint m_vboId, m_iboId;
	uint m_iboSize;
};
