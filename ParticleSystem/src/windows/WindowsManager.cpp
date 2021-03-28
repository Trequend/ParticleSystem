#include "WindowsManager.hpp"

#include <iostream>

void WindowsManager::Render()
{
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
