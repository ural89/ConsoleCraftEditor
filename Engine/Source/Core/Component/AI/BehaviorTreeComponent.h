#pragma once
#include "../../Component.h"
class BehaviorTreeComponent : public Component
{
public:
    BehaviorTreeComponent(class GameObject &owner) : Component(owner) {}
    void Init() override;
private:
    
 //TODO: add behavior tree asset here
};