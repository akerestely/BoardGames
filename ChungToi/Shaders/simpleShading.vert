#version 120

attribute vec2 vertexPosition;

uniform mat4 MVP;

void main()
{
	gl_Position.xy = (MVP * vec4(vertexPosition, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
}