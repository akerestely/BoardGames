#pragma once
#include <memory>

// forward declarations
namespace Engine
{
	class GLSLProgram;
	class Camera2D;
}

struct IRenderable
{
	virtual ~IRenderable() = default;
	virtual void Init(const std::shared_ptr<Engine::GLSLProgram> &program) = 0;
	virtual void Render(const Engine::Camera2D &camera, const glm::vec2 &position = glm::vec2()) = 0;
};
