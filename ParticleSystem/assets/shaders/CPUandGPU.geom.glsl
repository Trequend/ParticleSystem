#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 QuadRotation[];
in vec4 QuadColor[];
in float QuadSize[];

uniform mat4 ModelMatrix;
uniform mat4 ViewProjectionMatrix;

out vec4 Color;

mat4 Translation(vec4 t)
{
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		t.x, t.y, t.z, 1
	);
}

mat4 RotationX(float a)
{
	float sina = sin(a);
	float cosa = cos(a);
	return mat4(
		1, 0, 0, 0,
		0, cosa, sina, 0,
		0, -sina, cosa, 0,
		0, 0, 0, 1
	);
}

mat4 RotationY(float a)
{
	float sina = sin(a);
	float cosa = cos(a);
	return mat4(
		cosa, 0, -sina, 0,
		0, 1, 0, 0,
		sina, 0, cosa, 0,
		0, 0, 0, 1
	);
}

mat4 RotationZ(float a)
{
	float sina = sin(a);
	float cosa = cos(a);
	return mat4(
		cosa, sina, 0, 0,
		-sina, cosa, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

mat4 Rotation(vec3 r)
{
	return RotationX(r.x) * RotationY(r.y) * RotationZ(r.z);
}

mat4 Scale(float s)
{
	return mat4(
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1
	);
}

void main()
{
	float size2 = 0.5;
	mat4 m = ViewProjectionMatrix
	       * Translation(gl_in[0].gl_Position)
	       * Rotation(QuadRotation[0])
	       * Scale(QuadSize[0])
	       * ModelMatrix;

	gl_Position = m * vec4(-size2, -size2, 0.0, 1.0);
	Color = QuadColor[0];
	EmitVertex();

	gl_Position = m * vec4(size2, -size2, 0.0, 1.0);
	Color = QuadColor[0];
	EmitVertex();

	gl_Position = m * vec4(-size2, size2, 0.0, 1.0);
	Color = QuadColor[0];
	EmitVertex();

	gl_Position = m * vec4(size2, size2, 0.0, 1.0);
	Color = QuadColor[0];
	EmitVertex();

	EndPrimitive();
}
