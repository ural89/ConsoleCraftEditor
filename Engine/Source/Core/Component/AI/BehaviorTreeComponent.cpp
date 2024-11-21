#include "BehaviorTreeComponent.h"
#include "Core/AIBehavior/BehaviorTree.h"
#include "Core/AIBehavior/ActionNode.h"
void BehaviorTreeComponent::Init()
{
    auto root = std::make_shared<SelectorNode>("Selector 1");

    auto sequence1 = std::make_shared<SequenceNode>("Sequence 1");
    sequence1->addChild(std::make_shared<ConditionNode>("Condition A"));
    sequence1->addChild(std::make_shared<ActionNode>("Action A"));
    sequence1->addChild(std::make_shared<FailNode>("Failure node"));

    auto sequence2 = std::make_shared<SequenceNode>("Sequence 2");
    sequence2->addChild(std::make_shared<ConditionNode>("Condition C"));
    sequence2->addChild(std::make_shared<ActionNode>("Action D"));

    root->addChild(sequence1);
    root->addChild(sequence2);

    BehaviorTree tree(root);
    tree.Update();
    std::cin.get();
    tree.Update();
}