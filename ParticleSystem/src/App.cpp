#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Scene.hpp"
#include "windows/WindowsManager.hpp"
#include "windows/SceneSelector.hpp"
#include "windows/SceneVariables.hpp"
#include "windows/Environment.hpp"
#include "scenes/Sandbox.hpp"

void RegisterScenes()
{
	Scene::Register<Sandbox>("Sandbox");

	Scene::BlockRegistration();
}

void ErrorCallbackGLFW(int errorCode, const char* description)
{
	std::cerr << "GLFW (" << errorCode << "): " << description << std::endl;
}

bool InitGLFW()
{
	glfwSetErrorCallback(ErrorCallbackGLFW);
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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
	{
		std::cerr << "(GLFW) failed to create window" << std::endl;
		return nullptr;
	}
	
	glfwMakeContextCurrent(window);
	return window;
}

std::string DebugSourceToStringGL(unsigned int source)
{
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		return "OpenGL";
	case GL_DEBUG_SOURCE_APPLICATION:
		return "Application";
	case GL_DEBUG_SOURCE_OTHER:
		return "Other";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "Shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "3rd party";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "Windows system";
	default:
		return "Unknown";
	}
}

std::string DebugTypeToStringGL(unsigned int type)
{
	switch (type)
	{
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "Deprecated";
	case GL_DEBUG_TYPE_ERROR:
		return "Error";
	case GL_DEBUG_TYPE_MARKER:
		return "Marker";
	case GL_DEBUG_TYPE_OTHER:
		return "Other";
	case GL_DEBUG_TYPE_PERFORMANCE:
		return "Performance";
	case GL_DEBUG_TYPE_POP_GROUP:
		return "Pop group";
	case GL_DEBUG_TYPE_PORTABILITY:
		return "Portability";
	case GL_DEBUG_TYPE_PUSH_GROUP:
		return "Push group";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "Undefined";
	default:
		return "Unknown";
	}
}

std::string DebugSeverityToStringGL(unsigned int severity)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		return "HIGH";
	case GL_DEBUG_SEVERITY_LOW:
		return "LOW";
	case GL_DEBUG_SEVERITY_MEDIUM:
		return "MEDIUM";
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "NOTIFICATION";
	default:
		return "UNKNOWN";
	}
}

void GLAPIENTRY DebugMessageCallbackGL(
	unsigned int source,
	unsigned int type,
	unsigned int id,
	unsigned int severity,
	int length,
	const char* message,
	const void* userParam
)
{
	std::ostream& stream = type == GL_DEBUG_TYPE_ERROR ? std::cerr : std::cout;

	stream << "[" << DebugSeverityToStringGL(severity) << "]";
	stream << "(id:" << id << ") ";
	stream << DebugSourceToStringGL(source) << " -> ";
	stream << DebugTypeToStringGL(type) << ": ";
	stream << message;

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

	glDebugMessageCallback(DebugMessageCallbackGL, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
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

void InitWindowsManager(WindowsManager& windowsManager)
{
	windowsManager.AddWindow(new SceneSelector());
	windowsManager.AddWindow(new SceneVariables());
	windowsManager.AddWindow(new Environment());
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

	WindowsManager windowsManager;
	InitWindowsManager(windowsManager);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		ProcessCurrentScene();

		ImGuiNewFrame();
		windowsManager.Render();
		RenderImGuiFrame();

		glfwSwapBuffers(window);
	}

	Scene::CloseCurrent();
	glfwTerminate();
	return EXIT_SUCCESS;
}
