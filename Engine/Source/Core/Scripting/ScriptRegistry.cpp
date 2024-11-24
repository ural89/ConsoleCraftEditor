#include "ScriptRegistry.h"
#include "../Scene.h"
#include "IncludeGameObjects.h"
#include "IncludeComponents.h"
#include "../Component/RigidbodyComponent.h"
#include "../Component/PlayerController.h"
#include "Core/Component/AI/BehaviorTreeComponent.h"
#include "../ParticleSystem/ParticleSource.h"
GameObject *GameObjectFactory::Create(const std::string &gameObjectName)
{
    auto it = creatorMap.find(gameObjectName);
    if (it != creatorMap.end())
    {
        return it->second();
    }
    return nullptr;
}

void GameObjectFactory::Register(const std::string &gameObjectName, std::function<GameObject *()> creator)
{
    creatorMap[gameObjectName] = creator;
}

void GameObjectFactory::RegisterAllObjects(Scene &scene)
{
#include "IncludeRegisterObjectList.h"
}

Component *ComponentFactory::Create(const std::string &componentName)
{
    auto it = creatorMap.find(componentName);
    if (it != creatorMap.end())
    {
        return it->second();
    }
    return nullptr;
}

void ComponentFactory::Register(const std::string &componentName, std::function<Component *()> componentCreator)
{
    creatorMap[componentName] = componentCreator;
}

void ComponentFactory::RegisterAllComponents(GameObject &ownerGameObject)
{
    Register("RigidbodyComponent", [&ownerGameObject]() //default engine components
             { return new Rigidbody(ownerGameObject); });
    Register("PlayerController", [&ownerGameObject]()
             { return new PlayerController(ownerGameObject); });
    Register("ParticleSource", [&ownerGameObject]()
             { return new ParticleSource(ownerGameObject); });
    Register("BehaviorTreeComponent", [&ownerGameObject]()
             { return new BehaviorTreeComponent(ownerGameObject); });
             //this file is modified after added component
#include "IncludeRegisterComponentList.h" 
}

std::shared_ptr<BehaviorTreeNode> BehaviorTreeNodeFactory::Create(const std::string &nodeName)
{
    auto it = creatorMap.find(nodeName);
    if (it != creatorMap.end())
    {
        return it->second();
    }
    return nullptr;
}

void BehaviorTreeNodeFactory::Register(const std::string &nodeName, std::function<std::shared_ptr<BehaviorTreeNode> ()> creator)
{
    creatorMap[nodeName] = creator;
}

void BehaviorTreeNodeFactory::RegisterAllNodes()
{
    Register("Selector", []() 
    {
        return  std::make_shared<SelectorNode>();
    });
}
