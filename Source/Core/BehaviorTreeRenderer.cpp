#include "BehaviorTreeRenderer.h"
#include "imgui.h"
#include <vector>
#include <iostream>
void BehaviorTreeRenderer::Render(std::vector<BehaviorTreeNodeData *> behaviorTreeNodeDatas)
{
    ImGui::Begin("BehaviorTreeDesigner");
    if (behaviorTreeNodeDatas.size() > 0)
    {
        for (auto it = behaviorTreeNodeDatas.begin(); it != behaviorTreeNodeDatas.end(); ++it)
        {
            if ((*it) == nullptr)
            {
                continue;
            }
            std::cout << "Rendering " << (*it)->position << '\n';

            int boxSize = 10;
            ImVec2 topLeft(10, 10);//(*it)->position.X, (*it)->position.Y);
            ImVec2 bottomRight(100, 100);//(*it)->position.X + boxSize, (*it)->position.Y + boxSize);
            ImGui::GetWindowDrawList()->AddRect(topLeft, bottomRight, ImColor(1.0f, 1.0f, 1.0f, 0.5f), 0.0f, 0, 22.0f);
        }
    }
    ImGui::End();
}
