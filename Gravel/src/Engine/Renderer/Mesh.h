#pragma once

#include "Engine/Core/Core.h"
#include "Shader.h"

#include <assimp/types.h>

#include <vector>

namespace Gravel {


    class Mesh
    {
    public:

        struct Vertex
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 TexCoords;
        };

        struct Texture
        {
            uint32_t id;
            std::string type;
            aiString path;
        };

        virtual ~Mesh() = default;
        virtual void Draw(Shared<Shader> shader) = 0;

        static Shared<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures);
    };

}