#include "LevelBuilder.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include <fstream>
#include <iostream>
#include "PowerPelletComponent.h"
#include "GameEvents.h"
#include "../3rdParty/nlohmann/json.hpp"
using json = nlohmann::json;

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

    void LevelBuilder::LoadLevelFromFile(int levelNumber, Scene* scene, float offsetX, float offsetY)
    {
        m_Scene = scene;
        m_OffsetX = offsetX;
        m_OffsetY = offsetY;
		std::string fullPath = "../Data/Levels.json"; 
        m_LevelFilePath = fullPath;
		m_CurrentLevelNumber = levelNumber;

        // Clear previous nodes
        for (auto& row : m_Nodes)
        {
            for (auto& node : row)
            {
                delete node;
            }
        }
        m_Nodes.clear();

        // Load JSON
        std::ifstream file(fullPath);
        if (!file.is_open())
        {
            std::cerr << "Failed to open level JSON file: " << fullPath << "\n";
            return;
        }

        json levelJson;
        file >> levelJson;

        std::string levelKey = std::to_string(levelNumber);

        const auto& levelData = levelJson["levels"][levelKey];

        int y = 0;
        for (const auto& rowStr : levelData)
        {
            std::vector<Node*> row;
            std::string line = rowStr.get<std::string>();
            for (int x = 0; x < static_cast<int>(line.size()); ++x)
            {
                Node* newNode = nullptr;
                // Merge nodes
                if ((line[x] == 'm' && x + 1 < line.size() && line[x + 1] == 'm') ||
                    (line[x] == 'M' && x + 1 < line.size() && line[x + 1] == 'M'))
                {
                    float midX = x + 0.5f;
                    newNode = CreateNode(midX + offsetX, y + offsetY);
                    newNode->isMerged = true;
                    if (line[x] == 'M')
                        newNode->preferNot = true;

                    row.emplace_back(newNode);
                    row.emplace_back(nullptr);
                    ++x;
                }
                else
                {
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
                        // Wall
                        break;
                    case 'T':
                        newNode = CreateNode(x + offsetX, y + offsetY);
                        newNode->isTunnel = true;
                        break;
                    case '-':
                        newNode = CreateNode(x + offsetX, y + offsetY);
                        break;
                    case ' ':
                        break;
                    default:
                        break;
                    }

                    row.emplace_back(newNode);
                }
            }

            m_Nodes.emplace_back(std::move(row));
            ++y;
        }

        SpawnGhostHouse();
        SpawnFruitTriggerAreas();
        ConnectNodes();
        m_CenterNode = FindClosestNode(glm::vec2(200.f, 250.f));
    }

    void LevelBuilder::UnloadLevel()
    {
        for (auto* pellet : m_Pellets)
        {
            if (!pellet) continue;

            if (auto* collider = pellet->GetComponent<ColliderComponent>())
                CollisionSystem::GetInstance().UnregisterCollider(collider);

            if (m_Scene)
            {
                pellet->SetParent(nullptr, false);
                m_Scene->Remove(pellet);
            }
        }
        m_Pellets.clear();

        for (auto* powerPellets : m_PowerPellets)
        {
            if (!powerPellets) continue;

            if (auto* collider = powerPellets->GetComponent<ColliderComponent>())
                CollisionSystem::GetInstance().UnregisterCollider(collider);

            if (m_Scene)
            {
                powerPellets->SetParent(nullptr, false);
                m_Scene->Remove(powerPellets);
            }
        }
        m_PowerPellets.clear();

        for (auto& row : m_Nodes)
        {
            for (auto& node : row)
            {
                delete node;
                node = nullptr;
            }
        }
        m_Nodes.clear();

        m_TunnelPairs.clear();
        m_CenterNode = nullptr;
    }

    Node* LevelBuilder::CreateNode(float x, float y)
    {
        auto node = new Node{};
        node->position = glm::vec2((x + m_NodeCenterOffsetX) * m_TileSize, m_BaseY + (y + m_NodeCenterOffsetY) * m_TileSize);
        return node;
    }

    void LevelBuilder::ConnectNodes()
    {
        for (size_t y = 0; y < m_Nodes.size(); ++y)
        {
            Node* tunnelLeft = nullptr;
            Node* tunnelRight = nullptr;

            if (!m_Nodes[y].empty()) {
                if (m_Nodes[y][0] && m_Nodes[y][0]->isTunnel)
                    tunnelLeft = m_Nodes[y][0];
                if (m_Nodes[y].back() && m_Nodes[y].back()->isTunnel)
                    tunnelRight = m_Nodes[y].back();
            }

            if (tunnelLeft && tunnelRight) {
                m_TunnelPairs.emplace_back(TunnelPair{ tunnelLeft, tunnelRight });

                tunnelLeft->neighbors.emplace_back(tunnelRight);
                tunnelRight->neighbors.emplace_back(tunnelLeft);
            }

            for (size_t x = 0; x < m_Nodes[y].size(); ++x)
            {
                Node* node = m_Nodes[y][x];
                if (!node) continue;

                node->isTraversable = !IsWall(static_cast<int>(x), static_cast<int>(y));

                // UP 
                if (y > 0 && m_Nodes[y - 1][x])
                {
                    Node* above = m_Nodes[y - 1][x];
                    if (above->isTraversable && (!node->isMerged || above->isMerged))
                    {
                        node->neighbors.emplace_back(above);
                    }
                }

                // DOWN 
                if (y + 1 < m_Nodes.size() && m_Nodes[y + 1][x])
                {
                    Node* below = m_Nodes[y + 1][x];
                    if (below->isTraversable && (!node->isMerged || below->isMerged))
                    {
                        node->neighbors.emplace_back(below);
                    }
                }

                // LEFT
                if (x > 0)
                {
                    Node* left = m_Nodes[y][x - 1];
                    if (!left && x > 1)
                        left = m_Nodes[y][x - 2];

                    if (left && left->isTraversable && left != node)
                        node->neighbors.emplace_back(left);
                }

                // RIGHT 
                if (x + 1 < m_Nodes[y].size())
                {
                    Node* right = m_Nodes[y][x + 1];
                    if (!right && x + 2 < m_Nodes[y].size())
                        right = m_Nodes[y][x + 2];

                    if (right && right->isTraversable && right != node)
                        node->neighbors.emplace_back(right);
                }
            }
        }
    }



    void LevelBuilder::ClearPellets()
    {
        for (auto* pellet : m_Pellets)
        {
            if (!pellet) continue;

            if (auto* collider = pellet->GetComponent<ColliderComponent>())
                CollisionSystem::GetInstance().UnregisterCollider(collider);

            if (m_Scene)
            {
                pellet->SetParent(nullptr, false);
                m_Scene->Remove(pellet);
            }
        }
        m_Pellets.clear();

        for (auto* powerPellets : m_PowerPellets)
        {
            if (!powerPellets) continue;

            if (auto* collider = powerPellets->GetComponent<ColliderComponent>())
                CollisionSystem::GetInstance().UnregisterCollider(collider);

            if (m_Scene)
            {
                powerPellets->SetParent(nullptr, false);
                m_Scene->Remove(powerPellets);
            }
        }
        m_PowerPellets.clear();
    }

    void LevelBuilder::ResetPellets()
    {
        ClearPellets(); // Clear existing pellets

        std::ifstream file(m_LevelFilePath);
        if (!file.is_open())
        {
            std::cerr << "Failed to open level JSON file: " << m_LevelFilePath << "\n";
            return;
        }

        json levelJson;
        file >> levelJson;

        std::string levelKey = std::to_string(m_CurrentLevelNumber); 
        const auto& levelData = levelJson["levels"][levelKey];

        int y = 0;
        for (const auto& rowStr : levelData)
        {
            std::string line = rowStr.get<std::string>();
            for (int x = 0; x < static_cast<int>(line.size()); ++x)
            {
                switch (line[x])
                {
                case '.':
                    SpawnPellet(x, y);
                    break;
                case 'o':
                    SpawnPowerPellet(x, y);
                    break;
                default:
                    break;
                }
            }
            ++y;
        }
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
        pellet->GetComponent<RenderComponent>()->SetColor(m_PelletColors.r, m_PelletColors.g, m_PelletColors.b, m_PelletColors.alpha);
        pellet->AddComponent<ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(0.f, 0.f), CollisionLayer::Object, false);

        const auto pos = GridToWorldPosition(x, y);
        pellet->SetPosition(pos.x, pos.y, pos.z);
        pellet->SetRenderLayer(0);

        GameObject* pelletPtr = pellet.get();
        m_Pellets.emplace_back(pelletPtr);

        m_Scene->Add(pellet);
    }

    void LevelBuilder::SpawnPowerPellet(int x, int y)
    {
        auto powerPellet = std::make_unique<GameObject>();
        powerPellet->AddComponent<RenderComponent>("MSPC_Power_Pellet.tga", 2.0f);
        powerPellet->GetComponent<RenderComponent>()->SetColor(m_PowerPelletColors.r, m_PowerPelletColors.g, m_PowerPelletColors.b, m_PowerPelletColors.alpha);
        powerPellet->AddComponent<ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(0.f, 0.f), CollisionLayer::Object, false);
		powerPellet->AddComponent<PowerPelletComponent>();

        const auto pos = GridToWorldPosition(x, y);
        powerPellet->SetPosition(pos.x, pos.y, pos.z);
        powerPellet->SetRenderLayer(0);

        GameObject* powerPelletPtr = powerPellet.get();
        m_PowerPellets.emplace_back(powerPelletPtr);

        m_Scene->Add(powerPellet);
    }

    void LevelBuilder::SpawnGhostHouse()
    {
        const float COLLIDER_SIZEX = 80.f;
        const float COLLIDER_SIZEY = 30.f;

        auto ghostHouse = std::make_unique<GameObject>();
		m_GhostHousePtr = ghostHouse.get();
        ghostHouse->AddComponent<ColliderComponent>(glm::vec2(COLLIDER_SIZEX, COLLIDER_SIZEY), glm::vec2(0.f, 0.f), CollisionLayer::Object, false);
        ghostHouse->SetPosition(224.f - COLLIDER_SIZEX/2, 288.f - COLLIDER_SIZEY);
        m_Scene->Add(ghostHouse);
	}

    void LevelBuilder::SpawnFruitTriggerAreas()
    {
        const float COLLIDER_SIZEX = 40.f;
        const float COLLIDER_SIZEY = 500.f;

        auto fruitLeftTriggerArea = std::make_unique<GameObject>();
        m_LeftFruitTriggerAreaPtr = fruitLeftTriggerArea.get();
        fruitLeftTriggerArea->AddComponent<ColliderComponent>(glm::vec2(COLLIDER_SIZEX, COLLIDER_SIZEY), glm::vec2(0.f, 0.f), CollisionLayer::Object, false);
        fruitLeftTriggerArea->SetPosition(-50.f, 40.f);
        m_Scene->Add(fruitLeftTriggerArea);

        auto fruitRightTriggerArea = std::make_unique<GameObject>();
        m_RightFruitTriggerAreaPtr = fruitRightTriggerArea.get();
        fruitRightTriggerArea->AddComponent<ColliderComponent>(glm::vec2(COLLIDER_SIZEX, COLLIDER_SIZEY), glm::vec2(0.f, 0.f), CollisionLayer::Object, false);
        fruitRightTriggerArea->SetPosition(458.f, 40.f);
        m_Scene->Add(fruitRightTriggerArea);
    }

    void LevelBuilder::SetPelletColor(float r, float g, float b, float alpha)
    {
        m_PelletColors.r = r;
        m_PelletColors.g = g;
        m_PelletColors.b = b;
        m_PelletColors.alpha = alpha;
	}

    void LevelBuilder::SetPowerPelletColor(float r, float g, float b, float alpha)
    {
        m_PowerPelletColors.r = r;
        m_PowerPelletColors.g = g;
        m_PowerPelletColors.b = b;
		m_PowerPelletColors.alpha = alpha;
    }


    void LevelBuilder::RemovePellet(GameObject* pellet)
    {
        if (!pellet) return;

        m_Pellets.erase(
            std::remove(m_Pellets.begin(), m_Pellets.end(), pellet),
            m_Pellets.end()
        );

        m_PowerPellets.erase(
            std::remove(m_PowerPellets.begin(), m_PowerPellets.end(), pellet),
            m_PowerPellets.end()
        );

        if (auto* collider = pellet->GetComponent<ColliderComponent>())
        {
            CollisionSystem::GetInstance().UnregisterCollider(collider);
        }

        if (m_Scene)
        {
            pellet->SetParent(nullptr, false);
            pellet->MarkForDestroy(); 
        }

        if (m_Pellets.empty())
        {
            EventSystem::GetInstance().Notify(Event{ AllPelletsEaten });
        }
    }

    glm::vec3 LevelBuilder::GridToWorldPosition(int x, int y) const
    {
        return glm::vec3{ (x + m_OffsetX) * m_TileSize, m_BaseY + (y + m_OffsetY) * m_TileSize, 0.0f };
    }

    bool LevelBuilder::IsWall(int x, int y)
    {
        if (x >= 0 && y >= 0 && y < m_Nodes.size() && x < m_Nodes[y].size())
        {
            return m_Nodes[y][x] == nullptr;
        }
        return false;
    }
}