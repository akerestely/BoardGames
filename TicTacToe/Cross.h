#pragma once
#include <memory>

// forward declarations
namespace Engine
{
	class GLSLProgram;
	class Camera2D;
}

class Cross
{
public:
	Cross();
	~Cross();

	void Init(const std::shared_ptr<Engine::GLSLProgram> &program);
	void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2());

private:
	void buildModel();

private:
	std::shared_ptr<Engine::GLSLProgram> program;
	uint vboId, iboId;
	uint iboSize;
};

