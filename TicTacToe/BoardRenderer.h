#pragma once
#include <memory>

// forward declarations
namespace Engine
{
	class GLSLProgram;
	class Camera2D;
}

class BoardRenderer
{
public:
	BoardRenderer();
	~BoardRenderer();

	void Init(const std::shared_ptr<Engine::GLSLProgram> &program);
	void Render(Engine::Camera2D &camera);

private:
	void buildModel();

private:
	std::shared_ptr<Engine::GLSLProgram> program;
	uint vboId, iboId;
	uint iboSize;
};

