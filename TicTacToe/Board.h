#pragma once
#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"

class Board
{
public:
	Board();
	~Board();

	void Init();
	void Render(Engine::Camera2D &camera);

private:
	void initShader();
	void buildModel();

private:
	Engine::GLSLProgram program;
	uint vboId, iboId;
	uint iboSize;
};

