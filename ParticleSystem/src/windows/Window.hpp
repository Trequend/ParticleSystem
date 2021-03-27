#pragma once

#include <string>

class Window
{
public:
	Window(std::string name, bool isOpened = true) : name(name), isOpened(isOpened) { }

	const std::string name;
	bool isOpened;
	virtual void Render() = 0;
};
