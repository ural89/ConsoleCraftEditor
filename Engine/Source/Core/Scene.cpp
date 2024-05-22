#include "Scene.h"
#include "UIHandler.h"
#include "Input.h"
#include "Scripting/SceneDataDeserializer.h"
#include "Scripting/ScriptRegistry.h"
#include <filesystem>
Scene::Scene()
{
	camera = new Camera();
	uiHandler = new UIHandler();
	collisionResolver = new CollisionResolver(*this);

	debugUIPtr = std::make_shared<UIData>();
	debugUIPtr->position = Vector2(5, 29);
	uiHandler->AddString(debugUIPtr);

	CreateBoxBorder();
}
void Scene::CreateBoxBorder()
{
	// Create ground body
	b2BodyDef borderDef;
	borderDef.position.Set(0, SCREENHEIGHT);

	b2Body *groundBody = World->CreateBody(&borderDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(SCREENWIDTH, 1.f);
	groundBody->CreateFixture(&groundBox, 0);

	borderDef.position.Set(0, 0);
	b2Body *topBody = World->CreateBody(&borderDef);
	b2PolygonShape topBox;
	topBox.SetAsBox(SCREENWIDTH, 1.f);
	topBody->CreateFixture(&topBox, 0);

	borderDef.position.Set(SCREENWIDTH, 0);
	b2Body *rightBody = World->CreateBody(&borderDef);
	b2PolygonShape rightBox;
	rightBox.SetAsBox(1, SCREENHEIGHT);
	rightBody->CreateFixture(&rightBox, 0);

	borderDef.position.Set(0, 0);
	b2Body *leftBody = World->CreateBody(&borderDef);
	b2PolygonShape leftBox;
	leftBox.SetAsBox(1, SCREENHEIGHT);
	leftBody->CreateFixture(&leftBox, 0);
}
Scene::~Scene()
{
	Input::Cleanup();
	for (auto &go : GetGameObjects())
	{
		if (go != nullptr)
			delete go;
	}
	for (auto &p : polygons)
	{
		delete p;
	}
	std::cout << "Destructed  scene" << '\n';
	delete camera;
	delete uiHandler;
	delete collisionResolver;
	delete raycaster;
	delete World;
	World = nullptr;
}

void Scene::InitializeGameObject(GameObject *go)
{
	GameObjects.push_back(go);
	NameToGameObjectMap.insert({go->name, go});
	go->Init();

	if (go->hasCollider)
		collision.AddGameObject(go);
}

void Scene::InitializePolygon(Polygon *p)
{
	polygons.push_back(p);
	// p->Init();
	polygonCollision.AddPolygon(p);
}

GameObject *Scene::CreateGameObjectFromTemplate(std::string gameObjectName)
{
	GameObjectFactory factory;
	ComponentFactory componentFactory;
	// Register creators for game objects
	factory.RegisterAllObjects(*this);

	std::string projectsFolder = std::filesystem::current_path().parent_path().string();

	SceneDataDeserializer sceneDataDeserializer;
	if (sceneDataDeserializer.Deserialize(projectsFolder + "/SceneData/" + currentSceneName,
										  sceneData))
	{
		for (auto &gameObjectData : sceneData.GameObjectDatas)
		{
			if ((!gameObjectData->isTemplate || gameObjectData->name != gameObjectName))
				continue;

			GameObject *gameObject = factory.Create(gameObjectData->script);
			if (gameObject == nullptr) // if there is no script for gameobject, it will be base gameobject class
			{
				gameObject = new GameObject("", *this);
				gameObject->name = gameObjectData->name;
			}
			// std::cout << gameObjectData->script << '\n';
			// std::cin.get();
			if (gameObject)
			{
				gameObject->gameObjectId = gameObjectData->id;
				componentFactory.RegisterAllComponents(*gameObject);
				for (size_t i = 0; i < gameObjectData->components.size(); i++)
				{
					gameObject->AddComponent(componentFactory.Create(gameObjectData->components[i].name));
				}
				Vector2 position = Vector2(gameObjectData->position.X, gameObjectData->position.Y);

				std::vector<std::vector<int>> sprite = SceneDataDeserializer::convertTo2D(gameObjectData->sprite, gameObjectData->spriteWidth); // convert from vector<vector> to 1d vector
				gameObject->sprite = sprite;
				AddGameObject(gameObject, position);
				
				return gameObject;
			}
		}
	}
	return nullptr;
}

void Scene::CreateGameObjectsFromSceneData(const std::string &sceneName) // DONT FORGET TO RENEW PROJECT AFTER CHANGE HERE!!
{
	std::vector<GameObject *> createdGameObjects;
	GameObjectFactory factory;
	ComponentFactory componentFactory;
	currentSceneName = sceneName; //TODO: create a setter for this
	// Register creators for game objects
	factory.RegisterAllObjects(*this);

	std::string projectsFolder = std::filesystem::current_path().parent_path().string();

	SceneDataDeserializer sceneDataDeserializer;
	if (sceneDataDeserializer.Deserialize(projectsFolder + "/SceneData/" + sceneName,
										  sceneData))
	{
		for (auto &gameObjectData : sceneData.GameObjectDatas)
		{
			if (gameObjectData->isTemplate)
				continue;; // if this is a template it wont be in scene

			GameObject *gameObject = factory.Create(gameObjectData->script);
			if (gameObject == nullptr)
			{
				gameObject = new GameObject("", *this);
				gameObject->name = gameObjectData->name;
			}
			// std::cout << gameObjectData->script << '\n';
			// std::cin.get();
			if (gameObject)
			{
				gameObject->gameObjectId = gameObjectData->id;
				componentFactory.RegisterAllComponents(*gameObject);
				for (size_t i = 0; i < gameObjectData->components.size(); i++)
				{
					gameObject->AddComponent(componentFactory.Create(gameObjectData->components[i].name));
				}
				Vector2 position = Vector2(gameObjectData->position.X, gameObjectData->position.Y);

				std::vector<std::vector<int>> sprite = SceneDataDeserializer::convertTo2D(gameObjectData->sprite, gameObjectData->spriteWidth); // convert from vector<vector> to 1d vector
				gameObject->sprite = sprite;
				AddGameObject(gameObject, position);
				createdGameObjects.push_back(gameObject);
			}
		}
		AttachParentsFromSceneData(sceneData, createdGameObjects);
	}
}
void Scene::AttachParentsFromSceneData(SceneData &sceneData, std::vector<GameObject *> &createdGameObjects)
{

	for (auto &gameObjectData : sceneData.GameObjectDatas)
	{
		if (gameObjectData->parentId != -1) // if object has a parent
		{
			int parentId = gameObjectData->parentId;

			GameObject *parentGameObject = nullptr;
			for (auto &gameObject : createdGameObjects)
			{
				if (gameObject->gameObjectId == parentId)
				{
					parentGameObject = gameObject;
					break;
				}
			}

			if (parentGameObject != nullptr)
			{
				for (auto &gameObject : createdGameObjects)
				{
					if (gameObject->gameObjectId == gameObjectData->id)
					{
						gameObject->transform.SetParent(parentGameObject->transform);

						break;
					}
				}
			}
		}
	}
}