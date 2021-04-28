#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexRotation;
layout (location=2) in vec4 VertexColor;
layout (location=3) in float VertexSize;

out vec3 QuadRotation;
out vec4 QuadColor;
out float QuadSize;

void main()
{
	QuadRotation = VertexRotation;
	QuadColor = VertexColor;
	QuadSize = VertexSize;

	gl_Position = vec4(VertexPosition, 1.0);
}