#include "SceneObject.hpp"

#include <imgui.h>
#include <glm/glm.hpp>

void SceneObject::Update(float deltaTime) { }

void SceneObject::UI()
{
	ImGui::Text(name.c_str());

	glm::vec3 translation = transform.GetTranslation();
	if (ImGui::InputFloat3("Translation", &translation[0]))
	{
		transform.SetTranslation(translation);
	}

	glm::vec3 rotation = transform.GetRotation();
	if (ImGui::InputFloat3("Rotation", &rotation[0]))
	{
		transform.SetRotation(rotation);
	}

	glm::vec3 scale = transform.GetScale();
	if (ImGui::InputFloat3("Scale", &scale[0]))
	{
		transform.SetScale(scale);
	}
}

void SceneObject::Render() { }
