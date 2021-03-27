#pragma once

#include <vector>
#include <string>

#include "Scene.hpp"

class SceneSelector
{
private:
	bool isOpen = true;
	int selectedScene;
	std::vector<std::string> scenes;

	static bool VectorGetter(void* data, int index, const char** outText);
public:
	SceneSelector() : scenes(Scene::GetNamesOfRegistered()) { }

	void Render();
	void Toggle();
};

