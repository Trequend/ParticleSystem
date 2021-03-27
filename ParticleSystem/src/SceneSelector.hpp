#pragma once

#include <vector>
#include <string>

#include "Scene.hpp"
#include "Window.hpp"

class SceneSelector : public Window
{
private:
	int selectedScene = 0;
	std::vector<std::string> scenes = Scene::GetNamesOfRegistered();

	static bool VectorGetter(void* data, int index, const char** outText);
public:
	void Render();
};

