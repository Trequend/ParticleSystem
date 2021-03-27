#include "SceneVariables.hpp"

#include <climits>

#include <imgui.h>

#include "../Scene.hpp"

void SceneVariables::Render()
{
	if (!isOpened)
	{
		return;
	}

	ImGui::SetNextWindowSizeConstraints(ImVec2(250, 150), ImVec2(INT_MAX, INT_MAX));
	ImGui::Begin(name.c_str(), &isOpened);
	if (!Scene::CurrentExists())
	{
		ImGui::End();
		return;
	}

	Scene& scene = Scene::GetCurrent();
	scene.UI();
	ImGui::End();
}
