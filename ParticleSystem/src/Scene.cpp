#include "Scene.hpp"

#include <stdexcept>

bool Scene::registrationIsAvailable = true;
bool Scene::changeBlocked;
Scene* Scene::currentScene;
std::string Scene::currentSceneName;
std::map<std::string, std::function<Scene* ()>> Scene::sceneConstructors;

void Scene::BlockRegistration()
{
	registrationIsAvailable = false;
}

bool Scene::IsRegistered(const std::string& name)
{
	auto it = sceneConstructors.find(name);
	return it != sceneConstructors.end();
}

std::vector<std::string> Scene::GetNamesOfRegistered()
{
	std::vector<std::string> registered;
	for (auto const& pair : sceneConstructors)
	{
		registered.push_back(pair.first);
	}

	return registered;
}

void Scene::Load(const std::string& name)
{
	if (changeBlocked)
	{
		return;
	}

	auto it = sceneConstructors.find(name);
	if (it == sceneConstructors.end())
	{
		return;
	}

	CloseCurrent();
	currentScene = it->second();
	currentSceneName = name;
	currentScene->Initialize();
}

Scene& Scene::GetCurrent()
{
	if (currentScene == nullptr)
	{
		throw std::runtime_error("No scene");
	}

	return *currentScene;
}

const std::string& Scene::GetCurrentSceneName()
{
	if (currentScene == nullptr)
	{
		throw std::runtime_error("No scene");
	}

	return currentSceneName;
}

void Scene::CloseCurrent()
{
	if (changeBlocked || currentScene == nullptr)
	{
		return;
	}

	changeBlocked = true;
	currentScene->OnDestroy();
	changeBlocked = false;
	delete currentScene;
	currentScene = nullptr;
}

bool Scene::CurrentExists()
{
	return currentScene != nullptr;
}

void Scene::Update()
{
	for (objectIndex = 0; objectIndex < objects.size(); objectIndex++)
	{
		objects[objectIndex]->Update();
	}
}

void Scene::Render()
{
	for (objectIndex = 0; objectIndex < objects.size(); objectIndex++)
	{
		objects[objectIndex]->Render();
	}
}

void Scene::UI() { }

void Scene::OnDestroy() { }

bool Scene::IsDestroyed() const
{
	return isDestroyed;
}

void Scene::RemoveObjectAt(size_t index)
{
	if (index >= objects.size())
	{
		return;
	}

	objects.erase(objects.begin() + index);
	if (objectIndex >= index)
	{
		objectIndex--;
	}
}

const std::vector<std::shared_ptr<SceneObject>>& Scene::GetObjects()
{
	return objects;
}
