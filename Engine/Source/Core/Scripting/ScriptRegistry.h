#pragma once
#include "../GameObject.h"
#include "Core/AIBehavior/BehaviorTree.h"
#include <map>
#include <functional>
class GameObjectFactory
{
public:
    GameObject *Create(const std::string &gameObjectName);
    void Register(const std::string &gameObjectName, std::function<GameObject *()> creator);
    void RegisterAllObjects(class Scene &scene);

private:
    std::map<std::string, std::function<GameObject *()>> creatorMap; // map game object name to creator function
};

class ComponentFactory
{
public:
    Component *Create(const std::string &componentName);
    void Register(const std::string &componentName, std::function<Component *()> creator);
    void RegisterAllComponents(class GameObject &ownerGameObject);

private:
    std::map<std::string, std::function<Component *()>> creatorMap;
};

class BehaviorTreeNodeFactory
{
public:
    BehaviorTreeNode *Create(const std::string &nodeName);
    void Register(const std::string &nodeName, std::function<BehaviorTreeNode *()> creator);
    void RegisterAllNodes();
private:
    std::map<std::string, std::function<BehaviorTreeNode *()>> creatorMap;
};