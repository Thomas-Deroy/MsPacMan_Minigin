#pragma once
#include <vector>
#include "Scene.h"
#include "GameObject.h"

namespace dae
{
    struct Node {
        glm::vec2 position;
        std::vector<Node*> neighbors;
        bool isTraversable = true;  
    };


    class Scene;

    class LevelBuilder
    {
    public:
        ~LevelBuilder();
        void LoadLevelFromFile(const std::string& filePath, Scene* scene, float offsetX = 0.f, float offsetY = 0.f);
        void RemovePellet(GameObject* pellet);

        Node* FindClosestNode(const glm::vec2& position);

    private:
        void SpawnPellet(int x, int y);
        void SpawnPowerPellet(int x, int y);
        void SpawnWall(int x, int y);
        Node* CreateNode(float x, float y);
        void ConnectNodes();
        bool IsWall(int x, int y);
        glm::vec3 GridToWorldPosition(int x, int y) const;

        std::vector<GameObject*> m_Pellets;
        std::vector<std::vector<Node*>> m_Nodes;
        Scene* m_Scene{ nullptr };
        float m_TileSize{ 16.0f };
        float m_BaseY{ 40.0f };
    };
}