#include <cstdlib>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "scenes/Scene.hpp"
#include "windows/WindowsManager.hpp"
#include "windows/SceneSelector.hpp"
#include "windows/SceneVariables.hpp"
#include "windows/Environment.hpp"
#include "windows/Performance.hpp"
#include "scenes/Sandbox.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/RendererError.hpp"

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

void InitWindowsManager(WindowsManager& windowsManager)
{
	windowsManager.AddWindow(new SceneSelector());
	windowsManager.AddWindow(new SceneVariables());
	windowsManager.AddWindow(new Environment());
	windowsManager.AddWindow(new Performance());
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

	Renderer* renderer = nullptr;
	try
	{
		renderer = new Renderer(5000);
	}
	catch (RendererError error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << "Cannot create renderer" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	Performance::Data* performanceData = new Performance::Data();

	unsigned int fps = 60;
	double lastFrameTime = 0.0;
	double deltaTime = 0.0;
	double step = 1.0 / fps;
	while (!glfwWindowShouldClose(window))
	{
		double currentFrameTime = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		std::chrono::milliseconds updateTime;
		std::chrono::milliseconds renderTimeCPU;
		std::chrono::milliseconds renderTimeGPU;
		bool dataCollected = false;

		if (Scene::CurrentExists())
		{
			Scene& scene = Scene::GetCurrent();
			deltaTime += std::min(0.1, currentFrameTime - lastFrameTime);

			auto start = std::chrono::high_resolution_clock::now();
			while (deltaTime > step)
			{
				deltaTime -= step;
				scene.Update(float(step));
				if (scene.IsDestroyed())
				{
					break;
				}
			}

			auto stop = std::chrono::high_resolution_clock::now();
			updateTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

			if (!scene.IsDestroyed())
			{
				start = std::chrono::high_resolution_clock::now();
				renderer->BeginScene();
				scene.Render();
				renderer->EndScene();
				auto stop = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
				renderTimeGPU = renderer->GetStatistics().renderTime;
				renderTimeCPU = duration - renderTimeGPU;

				if (!scene.IsDestroyed())
				{
					dataCollected = true;
				}
			}
		}

		ImGuiNewFrame();
		windowsManager.Render();
		RenderImGuiFrame();

		if (Scene::CurrentExists() && dataCollected)
		{
			performanceData->Add(
				updateTime,
				renderTimeCPU,
				renderTimeGPU,
				1.0 / (currentFrameTime - lastFrameTime)
			);
		}

		glfwSwapBuffers(window);
		lastFrameTime = currentFrameTime;
	}

	delete renderer;
	Scene::CloseCurrent();
	glfwTerminate();
	return EXIT_SUCCESS;
}
