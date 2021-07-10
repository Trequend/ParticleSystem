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
#include "renderer/Renderer.hpp"
#include "renderer/RendererError.hpp"
#include "scenes/Sandbox.hpp"
#include "scenes/cpu/EasyTestCPU.hpp"
#include "scenes/cpu/NormalTestCPU.hpp"
#include "scenes/cpu/HardTestCPU.hpp"
#include "scenes/cpu_and_gpu/EasyTestCPUandGPU.hpp"
#include "scenes/cpu_and_gpu/NormalTestCPUandGPU.hpp"
#include "scenes/cpu_and_gpu/HardTestCPUandGPU.hpp"

#include <Windows.h>

void RegisterScenes()
{
	Scene::Register<Sandbox>("Sandbox");

	Scene::Register<EasyTestCPU>("Easy test (CPU)");
	Scene::Register<NormalTestCPU>("Normal test (CPU)");
	Scene::Register<HardTestCPU>("Hard test (CPU)");

	Scene::Register<EasyTestCPUandGPU>("Easy test (CPU and GPU)");
	Scene::Register<NormalTestCPUandGPU>("Normal test (CPU and GPU)");
	Scene::Register<HardTestCPUandGPU>("Hard test (CPU and GPU)");

	Scene::BlockRegistration();
}

void ErrorCallbackGLFW(int errorCode, const char* description)
{
	std::cerr << "GLFW (" << errorCode << "): " << description << std::endl;
}

bool InitGLFW()
{
#if _DEBUG
	glfwSetErrorCallback(ErrorCallbackGLFW);
#endif
	if (!glfwInit())
	{
		std::cerr << "(GLFW) failed to initialize" << std::endl;
		return false;
	}
	
	return true;
}

GLFWwindow* CreateGLFWWindow(int width, int height, const char* title)
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

#ifdef _DEBUG
	glDebugMessageCallback(DebugMessageCallbackGL, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif
	return true;
}

ImGuiIO& InitImGui(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 18.0f);
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Dark theme. Thanks Cherno. https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp

	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	return io;
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

#if _DEBUG
int main()
#else
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#endif
{
	RegisterScenes();

	if (!InitGLFW())
	{
		return EXIT_FAILURE;
	}

	GLFWwindow* window = CreateGLFWWindow(1280, 720, "Particle system");
	if (!window)
	{
		return EXIT_FAILURE;
	}

	if (!InitGLEW())
	{
		return EXIT_FAILURE;
	}

	InitImGui(window);

	WindowsManager windowsManager(window);
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
		std::chrono::nanoseconds renderTimeCPU;
		std::chrono::nanoseconds renderTimeGPU;
		bool dataCollected = false;

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		if (Scene::CurrentExists() && width != 0 && height != 0)
		{
			Scene& scene = Scene::GetCurrent();
			deltaTime += min(0.1, currentFrameTime - lastFrameTime);

			auto start = std::chrono::high_resolution_clock::now();
			while (deltaTime >= step)
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
				scene.Render(float(deltaTime), float(step));
				renderer->EndScene();
				auto stop = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
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
				std::chrono::duration_cast<std::chrono::milliseconds>(renderTimeCPU),
				std::chrono::duration_cast<std::chrono::milliseconds>(renderTimeGPU),
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
