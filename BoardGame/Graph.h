#pragma once
#include<vector>

#include "glm.hpp"

#include "IRenderable.h"

class Graph : public IRenderable
{
public:
	Graph() = default;
	virtual ~Graph();

	virtual void Init(const std::shared_ptr<Engine::GLSLProgram> &program) override;
	virtual void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2()) override;

	void SetValues(const std::vector<float> &values1, const std::vector<float> &values2);

private:
	std::shared_ptr<Engine::GLSLProgram> m_program;
	uint32_t m_nVboId;
	uint32_t m_nIboId;
	uint32_t m_nIboSize;
	uint32_t m_nIboSize1;
	uint32_t m_nIboSize2;
};
