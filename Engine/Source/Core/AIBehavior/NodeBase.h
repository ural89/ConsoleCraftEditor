#pragma once
#include <vector>
#include <memory>
#include <string>

enum class NodeStatus
{

    Inactive,
    Running,
    Success,
    Failure
};

class BehaviorTreeNode
{
public:
    BehaviorTreeNode() {}
    // BehaviorTreeNode() {}

protected:
    NodeStatus status;
    std::string name;

public:
    virtual ~BehaviorTreeNode() = default;

    virtual NodeStatus Update() {return NodeStatus::Inactive;}

    NodeStatus getStatus() const { return status; }

    virtual std::string getName() const { return name; }
};
