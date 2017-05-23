#pragma once
#include "IRenderable.h"

class Cross : public IRenderable
{
public:
	Cross();
	~Cross();

	virtual void Init(const std::shared_ptr<Engine::GLSLProgram> &program) override;
	virtual void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2()) override;

private:
	void buildModel();

private:
	std::shared_ptr<Engine::GLSLProgram> program;
	uint vboId, iboId;
	uint iboSize;
};

