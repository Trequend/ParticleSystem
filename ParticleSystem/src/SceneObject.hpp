#pragma once

#include <string>

#include "Transform.hpp"

class SceneObject abstract
{
public:
	Transform transform;
	const std::string name;

	SceneObject(const std::string& name, Transform transform) : name(name), transform(transform) { }
	SceneObject(const std::string& name) : name(name) { }

	virtual void Update();
	virtual void UI();
	virtual void Render();
};
