#include "LevelBuilder.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include <fstream>
#include <iostream>

namespace dae
{
    LevelBuilder::~LevelBuilder()
    {
        for (auto& row : m_Nodes)
        {
            for (auto& node : row)
            {
                delete node;
                node = nullptr;
            }
        }
        m_Nodes.clear();
    }

    void LevelBuilder::LoadLevelFromFile(const std::string& filePath, Scene* scene, float offsetX, float offsetY)
    {
        m_Scene = scene;

        // Clean up old nodes
        for (auto& row : m_Nodes)
        {
            for (auto& node : row)
            {
                delete node;
            }
        }
        m_Nodes.clear();

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cerr << "Failed to open level file: " << filePath << "\n";
            return;
        }

        std::string line;
        int y = 0;

        while (std::getline(file, line))
        {
            std::vector<Node*> row;
            for (int x = 0; x < static_cast<int>(line.size()); ++x)
            {
                Node* newNode = nullptr;

                // No offset for spawning objects
                switch (line[x])
                {
                case '.':
                    SpawnPellet(x, y);
                    newNode = CreateNode(x + offsetX, y + offsetY);
                    break;
                case 'o':
                    SpawnPowerPellet(x, y);
                    newNode = CreateNode(x + offsetX, y + offsetY);
                    break;
                case '#':
                    SpawnWall(x, y);
                    break;
                case '-':
                    newNode = CreateNode(x + offsetX, y + offsetY);
                    break;
                case ' ':
                    // empty space, no node
                    break;
                default:
                    break;
                }

                row.emplace_back(newNode);
            }
            m_Nodes.emplace_back(std::move(row));
            ++y;
        }

        ConnectNodes();
    }


    Node* LevelBuilder::CreateNode(float x, float y)
    {
        auto node = new Node{};
        node->position = glm::vec2(x * m_TileSize, m_BaseY + y * m_TileSize);
        return node;
    }

    void LevelBuilder::ConnectNodes()
    {
        for (size_t y = 0; y < m_Nodes.size(); ++y)
        {
            for (size_t x = 0; x < m_Nodes[y].size(); ++x)
            {
                Node* node = m_Nodes[y][x];
                if (!node) continue;

                // Check if the current tile is a wall and set traversability
                if (IsWall(static_cast<int>(x), static_cast<int>(y)))
                    node->isTraversable = false;
                else node->isTraversable = true;   

                // Up
                if (y > 0 && m_Nodes[y - 1][x] && m_Nodes[y - 1][x]->isTraversable)
                    node->neighbors.emplace_back(m_Nodes[y - 1][x]);
                // Down
                if (y + 1 < m_Nodes.size() && m_Nodes[y + 1][x] && m_Nodes[y + 1][x]->isTraversable)
                    node->neighbors.emplace_back(m_Nodes[y + 1][x]);
                // Left
                if (x > 0 && m_Nodes[y][x - 1] && m_Nodes[y][x - 1]->isTraversable)
                    node->neighbors.emplace_back(m_Nodes[y][x - 1]);
                // Right
                if (x + 1 < m_Nodes[y].size() && m_Nodes[y][x + 1] && m_Nodes[y][x + 1]->isTraversable)
                    node->neighbors.emplace_back(m_Nodes[y][x + 1]);
            }
        }
    }

    bool LevelBuilder::IsWall(int x, int y)
    {
        if (x >= 0 && y >= 0 && y < m_Nodes.size() && x < m_Nodes[y].size())
        {
            return m_Nodes[y][x] == nullptr;
        }
        return false;
    }


    Node* LevelBuilder::FindClosestNode(const glm::vec2& position)
    {
        Node* closestNode = nullptr;
        float closestDistanceSq = std::numeric_limits<float>::max();

        for (auto& row : m_Nodes)
        {
            for (auto* node : row)
            {
                if (!node) continue;

                float distSq = glm::distance(node->position, position);
                distSq *= distSq;
                if (distSq < closestDistanceSq)
                {
                    closestDistanceSq = distSq;
                    closestNode = node;
                }
            }
        }

        return closestNode;
    }

    void LevelBuilder::SpawnPellet(int x, int y)
    {
        auto pellet = std::make_unique<GameObject>();
        pellet->AddComponent<RenderComponent>("MSPC_Pellet.tga", 2.0f);
        pellet->AddComponent<ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(0.f, 0.f), CollisionLayer::Object, false, false);

        const auto pos = GridToWorldPosition(x, y);
        pellet->SetPosition(pos.x, pos.y, pos.z);

        GameObject* pelletRawPtr = pellet.get();
        m_Pellets.emplace_back(pelletRawPtr);

        m_Scene->Add(pellet);
    }

    void LevelBuilder::SpawnPowerPellet(int x, int y)
    {
        auto powerPellet = std::make_unique<GameObject>();
        powerPellet->AddComponent<RenderComponent>("MSPC_Power_Pellet.tga", 2.0f);

        const auto pos = GridToWorldPosition(x, y);
        powerPellet->SetPosition(pos.x, pos.y, pos.z);

        m_Scene->Add(powerPellet);
    }

    void LevelBuilder::SpawnWall(int x, int y)
    {
        auto wall = std::make_unique<GameObject>();
        //wall->AddComponent<RenderComponent>("test.tga", 2.f);
        wall->AddComponent<ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(0.f, 0.f), CollisionLayer::Wall, true, false);

        const auto pos = GridToWorldPosition(x, y);
        wall->SetPosition(pos.x, pos.y, pos.z);

        m_Scene->Add(wall);
    }

    void LevelBuilder::RemovePellet(GameObject* pellet)
    {
        if (!pellet) return;

        m_Pellets.erase(
            std::remove(m_Pellets.begin(), m_Pellets.end(), pellet),
            m_Pellets.end()
        );

        auto* collider = pellet->GetComponent<ColliderComponent>();
        if (collider)
            CollisionSystem::GetInstance().UnregisterCollider(collider);

        if (m_Scene) {
            pellet->SetParent(nullptr, false);
            m_Scene->Remove(pellet);
        }
    }

    glm::vec3 LevelBuilder::GridToWorldPosition(int x, int y) const
    {
        return glm::vec3{ x * m_TileSize, m_BaseY + y * m_TileSize, 0.0f };
    }
}