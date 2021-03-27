#pragma once

#include "Window.hpp"

#include <imgui.h>
#include <vector>
#include <memory>

class WindowsManager : public Window
{
private:
	bool toggleButtonPressed = false;
	std::vector<std::shared_ptr<Window>> windows;
public:
	WindowsManager() : Window("Windows") { }

	void Render();
	void AddWindow(Window* window);
};

