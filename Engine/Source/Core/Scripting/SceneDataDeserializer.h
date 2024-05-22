#pragma once
#include <string>
#include <fstream>
#include "../DataStructs.h"

class SceneDataDeserializer
{
public:
    bool Deserialize(const std::string &filePath, SceneData &sceneData);
    static std::vector<std::vector<int>> convertTo2D(const std::vector<int> &sprite1D, int spriteWidth)
    {
        std::vector<std::vector<int>> sprite;
        int spriteHeight = sprite1D.size() / spriteWidth;

        sprite.resize(spriteHeight);

        // Fill the 2D sprite by iterating over the 1D vector
        for (int i = 0; i < spriteHeight; ++i)
        {
            // Calculate the starting index for this row
            int startIndex = i * spriteWidth;
            // Copy elements from sprite1D to this row of sprite
            sprite[i].assign(sprite1D.begin() + startIndex, sprite1D.begin() + startIndex + spriteWidth);
        }

        return sprite;
    }
};