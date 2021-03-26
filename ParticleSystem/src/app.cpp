#include <cstdlib>
#include <iostream>

#include <GLFW/glfw3.h>

void HandleErrorGLFW(int errorCode, const char* description)
{
	std::cerr << "GLFW (" << errorCode << "): " << description << std::endl;
}

int InitGLFW()
{
	glfwSetErrorCallback(HandleErrorGLFW);
	return glfwInit();
}

GLFWwindow* CreateWindow(int width, int height, const char* title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	return glfwCreateWindow(width, height, title, NULL, NULL);
}

int main()
{
	if (!InitGLFW())
	{
		std::cerr << "(GLFW) failed to initialize" << std::endl;
		return EXIT_FAILURE;
	}

	GLFWwindow* window = CreateWindow(1280, 720, "Particle system");
	if (!window)
	{
		std::cerr << "(GLFW) failed to create window" << std::endl;
		return EXIT_FAILURE;
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}