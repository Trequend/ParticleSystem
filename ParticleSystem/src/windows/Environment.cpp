#include "Environment.hpp"

#include <imgui.h>

#include "../GLInfo.hpp"

void Environment::Render()
{
	if (!isOpened)
	{
		return;
	}

	ImGui::SetNextWindowSizeConstraints(ImVec2(320, 100), ImVec2(INT_MAX, INT_MAX));
	ImGui::Begin(name.c_str(), &isOpened, ImGuiWindowFlags_NoCollapse);
	ImGui::Text("GL Vendor   : %s", GLInfo::Vendor());
	ImGui::Text("GL Renderer : %s", GLInfo::Renderer());
	ImGui::Text("GL Version  : %s", GLInfo::Version());
	ImGui::Text("GLSL Version: %s", GLInfo::GLSLVersion());
	ImGui::End();
}
