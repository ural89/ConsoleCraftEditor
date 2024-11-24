#pragma once
#include "NodeBase.h"
class CompositeNode : public BehaviorTreeNode
{
protected:
public:
    CompositeNode() {}
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    void addChild(std::shared_ptr<BehaviorTreeNode> child)
    {
        children.push_back(child);
    }
};

class SequenceNode : public CompositeNode
{
public:
    SequenceNode() : CompositeNode() { name = "SequenceNode";}
    NodeStatus Update() override
    {
        status = NodeStatus::Running;
        for (auto &child : children)
        {
            NodeStatus childStatus = child->Update();
            if (childStatus != NodeStatus::Success)
            {
                status = childStatus;
                return status;
            }
        }
        status = NodeStatus::Success;
        return status;
    }
};

class SelectorNode : public CompositeNode
{
public:
    SelectorNode() : CompositeNode(){name = "SelectorNode";}
    NodeStatus Update() override
    {
        status = NodeStatus::Running;
        for (auto &child : children)
        {
            NodeStatus childStatus = child->Update();
            if (childStatus != NodeStatus::Failure)
            {
                status = childStatus;
                return status;
            }
        }
        status = NodeStatus::Failure;
        return status;
    }
};
