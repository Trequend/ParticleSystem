#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Scene.hpp"

void RegisterScenes()
{

}

void HandleErrorGLFW(int errorCode, const char* description)
{
	std::cerr << "GLFW (" << errorCode << "): " << description << std::endl;
}

bool InitGLFW()
{
	glfwSetErrorCallback(HandleErrorGLFW);
	if (!glfwInit())
	{
		std::cerr << "(GLFW) failed to initialize" << std::endl;
		return false;
	}
	
	return true;
}

GLFWwindow* CreateWindow(int width, int height, const char* title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
	{
		std::cerr << "(GLFW) failed to create window" << std::endl;
		return nullptr;
	}
	
	glfwMakeContextCurrent(window);
	return window;
}

bool InitGLEW()
{
	unsigned int code = glewInit();
	if (code != GLEW_OK)
	{
		std::cerr << "GLEW: " << glewGetErrorString(code) << std::endl;
		std::cerr << "(GLEW) failed to initialize" << std::endl;
		return false;
	}

	return true;
}

void InitImGui(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void ImGuiNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderImGuiFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ProcessCurrentScene()
{
	if (!Scene::CurrentExists())
	{
		return;
	}

	Scene& scene = Scene::GetCurrent();
	
	scene.Update();
	if (scene.IsDestroyed())
	{
		return;
	}

	scene.Render();
	if (scene.IsDestroyed())
	{
		return;
	}
}

void ProcessCurrentSceneUI()
{
	if (!Scene::CurrentExists())
	{
		return;
	}

	Scene& scene = Scene::GetCurrent();
	scene.UI();
}

int main()
{
	RegisterScenes();

	if (!InitGLFW())
	{
		return EXIT_FAILURE;
	}

	GLFWwindow* window = CreateWindow(1280, 720, "Particle system");
	if (!window)
	{
		return EXIT_FAILURE;
	}

	if (!InitGLEW())
	{
		return EXIT_FAILURE;
	}

	InitImGui(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		ProcessCurrentScene();

		ImGuiNewFrame();
		ProcessCurrentSceneUI();
		RenderImGuiFrame();

		glfwSwapBuffers(window);
	}

	Scene::CloseCurrent();
	glfwTerminate();
	return EXIT_SUCCESS;
}
