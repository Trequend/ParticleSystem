#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>

class Scene
{
private:
	static bool changeBlocked;
	static Scene* currentScene;
	static std::map<std::string, std::function<Scene* ()>> sceneConstructors;

	bool isDestroyed;
public:
	template<class T>
	static void Register(const std::string& name);
	static bool IsRegistered(const std::string& name);
	static std::vector<std::string> GetNamesOfRegistered();
	static void Load(const std::string& name);
	static Scene& GetCurrent();
	static void CloseCurrent();
	static bool CurrentExists();

	virtual void Initialize() = 0;
	void Update();
	void Render();
	virtual void UI();
	virtual void OnDestroy();
	bool IsDestroyed() const;
};

template<class T>
void Scene::Register(const std::string& name)
{
	static_assert(std::is_base_of<Scene, T>::value);

	if (IsRegistered(name))
	{
		return;
	}

	sceneConstructors.insert(make_pair(name, []() { return (Scene*)new T(); }));
}
