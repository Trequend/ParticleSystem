#include "WindowsManager.hpp"

#include <iostream>

void WindowsManager::SwitchFullscreen()
{
	isFullscreen = !isFullscreen;
	if (isFullscreen)
	{
		glfwGetWindowPos(window, &savedPosition[0], &savedPosition[1]);
		glfwGetWindowSize(window, &savedSize[0], &savedSize[1]);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(window, nullptr, savedPosition[0], savedPosition[1], savedSize[0], savedSize[1], 0);
	}
}

void WindowsManager::Render()
{
	if (ImGui::IsKeyPressed('Q', false))
	{
		SwitchFullscreen();
	}

	if (ImGui::IsKeyPressed('W', false))
	{
		isOpened = !isOpened;
	}

	if (!isOpened)
	{
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(250, 150), ImGuiCond_FirstUseEver);
	ImGui::Begin(name.c_str(), &isOpened, ImGuiWindowFlags_NoResize);
	for (size_t i = 0; i < windows.size(); i++)
	{
		ImGui::Checkbox(windows[i]->name.c_str(), &windows[i]->isOpened);
	}
	ImGui::End();

	for (size_t i = 0; i < windows.size(); i++)
	{
		windows[i]->Render();
	}
}

void WindowsManager::AddWindow(Window* window)
{
	windows.push_back(std::shared_ptr<Window>(window));
}
