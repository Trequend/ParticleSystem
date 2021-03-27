#pragma once

class Window
{
public:
	Window(bool isOpened = true) : isOpened(isOpened) { }

	bool isOpened;
	virtual void Render() = 0;
};
