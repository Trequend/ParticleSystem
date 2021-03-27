#pragma once

#include "Transform.hpp"

class SceneObject abstract
{
public:
	Transform transform;

	SceneObject(Transform transform) : transform(transform) { }
	SceneObject() { }

	virtual void Update();
	virtual void UI();
	virtual void Render();
};
