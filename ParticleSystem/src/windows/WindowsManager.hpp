#pragma once

#include <vector>
#include <memory>
#include <imgui.h>

#include "Window.hpp"

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

