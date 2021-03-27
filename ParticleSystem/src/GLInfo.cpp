#include "GLInfo.hpp"

#include <GL/glew.h>

bool GLInfo::isInitialized;
const unsigned char* GLInfo::vendor;
const unsigned char* GLInfo::renderer;
const unsigned char* GLInfo::version;
const unsigned char* GLInfo::glslVersion;
int GLInfo::major;
int GLInfo::minor;

void GLInfo::Initialize()
{
	if (isInitialized)
	{
		return;
	}

	isInitialized = true;
	vendor = glGetString(GL_VENDOR);
	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);
	glGetString(GL_SHADING_LANGUAGE_VERSION);

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
}

const unsigned char* GLInfo::Vendor()
{
	Initialize();
	return vendor;
}

const unsigned char* GLInfo::Renderer()
{
	Initialize();
	return renderer;
}

const unsigned char* GLInfo::Version()
{
	Initialize();
	return version;
}

const unsigned char* GLInfo::GLSLVersion()
{
	Initialize();
	return glslVersion;
}

int GLInfo::Major()
{
	Initialize();
	return major;
}

int GLInfo::Minor()
{
	Initialize();
	return minor;
}
