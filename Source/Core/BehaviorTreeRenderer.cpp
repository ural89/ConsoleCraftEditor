#include "BehaviorTreeRenderer.h"
#include "imgui.h"
#include <vector>
#include <iostream>
void BehaviorTreeRenderer::Render(std::vector<BehaviorTreeNodeData *> behaviorTreeNodeDatas)
{
    ImGui::Begin("BehaviorTreeDesigner");
    ImVec2 gameViewPosition = ImGui::GetCursorScreenPos();
    if (behaviorTreeNodeDatas.size() > 0)
    {
        for (auto it = behaviorTreeNodeDatas.begin(); it != behaviorTreeNodeDatas.end(); ++it)
        {
            if ((*it) == nullptr)
            {
                continue;
            }

            int boxSize = 75;
            ImVec2 topLeft((*it)->position.X + gameViewPosition.x, (*it)->position.Y + gameViewPosition.y);//(*it)->position.X, (*it)->position.Y);
            ImVec2 bottomRight((*it)->position.X + gameViewPosition.x + boxSize, (*it)->position.Y + gameViewPosition.y + boxSize);//(*it)->position.X + boxSize, (*it)->position.Y + boxSize);
            //  std::cout << "Rendering " << topLeft.y << '\n';
            ImGui::GetWindowDrawList()->AddRect(topLeft, bottomRight, ImColor(1.0f, 1.0f, 1.0f, 0.5f), 0.0f, 0, 2.0f);
        }
    }
    ImGui::End();
}
