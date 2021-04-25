#pragma once

#include <vector>
#include <memory>
#include <imgui.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"

class WindowsManager : public Window
{
private:
	bool toggleButtonPressed = false;
	GLFWwindow* window;
	bool isFullscreen = false;
	int savedPosition[2] = { 0, 0 };
	int savedSize[2] = { 0, 0 };
	std::vector<std::shared_ptr<Window>> windows;

	void SwitchFullscreen();
public:
	WindowsManager(GLFWwindow* window) : Window("Windows"), window(window) { }

	void Render();
	void AddWindow(Window* window);
};

