#pragma once
#include "../Core.h"
#include "../CoreStructs/Transform.h"
#include <vector>
#include "Component.h"

class GE_API GameObject
{

public:

	int gameObjectId = 0;
	GameObject(std::string&& name, class Scene &scene) : name(name), scene(scene){};
	virtual ~GameObject()
	{
		for (auto &component : components)
		{
			delete component;
		}
		components.clear();
		sprite.clear();
	}

	Transform transform = Transform();

	std::string name = "Gameobject";
	std::vector<std::vector<int>> sprite = {{1, 1}, {1, 1}};

	virtual int GetWidth() const
	{
		return sprite[0].size();
	}
	virtual int GetHeight() const
	{
		return sprite.size();
	}
	void Destroy()
	{
		OnDestroy();
		isDestroyedFlag = true;
		transform.HasOwnerDestroyed = true;
	}
	void AddComponent(Component *component)
	{
		components.push_back(component);
		component->Init();
	}
	void SetRenderable(bool isRenderable)
	{
		this->isRenderable = isRenderable;
		if(!isRenderable)
		{
			forceClearFromScreen = true;
		}
	}

	template <typename T>
	T *GetComponent()
	{
		for (auto &component : components)
		{

			if (typeid(*component) == typeid(T))
			{
				return dynamic_cast<T *>(component);
			}
		}
		return nullptr;
	}

	//overrides
public:
	virtual void Init(){};
	virtual void Update(float deltaTime){};
	virtual void OnCollided(GameObject &other){};
	virtual void OnCollisionExit(GameObject &other){};
	virtual void OnCollidedBorder(int border){};
	virtual void OnDestroy(){};

	void UpdateComponents(float deltaTime)
	{
		for (auto &component : components)
		{
			component->Update(deltaTime);
		}
	}
	std::string symbol = "*";
	int overrideColor = -1;

	bool isDestroyedFlag = false;
	bool hasClearedFromScreen = false;
	bool forceClearFromScreen = false;
	bool isRenderable = true;
	bool hasCollider = true;
	bool canFindable = true;
	bool isNavIgnore = false;
	Scene &GetCurrentScene()
	{
		return scene;
	}
	bool operator!=(const GameObject &other) const
	{
		return this != &other; 
	}
	protected:
		std::vector<Component *> components;
		Scene & scene;
#ifdef __GNUC__
		int GREEN = 2;
		int RED = 1;
		int YELLOW = 3;
#else
		int GREEN = 2;
		int RED = 4;
		int YELLOW = 1;
#endif
	};
