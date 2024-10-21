#pragma once

#include "NodeBase.h"
namespace AIBehavior
{
    class SuccessNode : public NodeBase
    {
    public:
        SuccessNode() : NodeBase("Success Node"){}
        NodeResult Update(float deltaTime);
        void Enter() override;
        void Exit() override;
    };
};