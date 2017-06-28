#pragma once
#include<vector>

#include "glm.hpp"

#include "IRenderable.h"

class Graph : public IRenderable
{
public:
	virtual ~Graph();

	virtual void Init(const std::shared_ptr<Engine::GLSLProgram> &program) override;
	virtual void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2()) override;

	void SetValues(const std::vector<float> &values1, const std::vector<float> &values2);

private:
	std::shared_ptr<Engine::GLSLProgram> m_program;
	uint m_nVboId;
	uint m_nIboId;
	uint m_nIboSize;
	uint m_nIboSize1;
	uint m_nIboSize2;
};
