#pragma once

#include "NodeBase.h"
#include <vector>

namespace AIBehavior
{
    class SelectorNode : public NodeBase
    {
    public:
        SelectorNode(std::string &&nodeName) : NodeBase(nodeName){}
        SelectorNode() : NodeBase("Selector Node"), m_CurrentChildIndex(0) {}

        void Enter() override;
        void Exit() override;
        NodeResult Update(float deltaTime) override;

    private:
        size_t m_CurrentChildIndex;
    };
}
