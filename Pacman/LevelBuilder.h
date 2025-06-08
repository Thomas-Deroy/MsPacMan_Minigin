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
        bool isMerged = false;
		bool isTunnel = false;

        bool preferNot = false;
    };

    struct TunnelPair
    {
        Node* left = nullptr;
        Node* right = nullptr;
    };

    struct PelletColors
    {
        float r = 1.f;
        float g = 1.f;
        float b = 1.f;
        float alpha = 1.f;
	};

    class Scene;

    class LevelBuilder
    {
    public:
        ~LevelBuilder();
        void LoadLevelFromFile(const std::string& filePath, Scene* scene, float offsetX = 0.f, float offsetY = 0.f);
        void UnloadLevel();

		void SetPelletColor(float r, float g, float b, float alpha = 1.f);
        void ResetPellets();
        void RemovePellet(GameObject* pellet);

        Node* FindClosestNode(const glm::vec2& position);
        Node* GetCenterNode() const { return m_CenterNode; }

        GameObject* GetGhostHouse() const { return m_GhostHousePtr; };

        struct TunnelPair
        {
            Node* left = nullptr;
            Node* right = nullptr;
        };

        const std::vector<TunnelPair>& GetTunnelPairs() const { return m_TunnelPairs; }

    private:
        void SpawnPellet(int x, int y);
        void SpawnPowerPellet(int x, int y);
		void SpawnGhostHouse();

		GameObject* m_GhostHousePtr;

        Node* CreateNode(float x, float y);
        void ConnectNodes();
        bool IsWall(int x, int y);
        glm::vec3 GridToWorldPosition(int x, int y) const;

        std::vector<GameObject*> m_Pellets;
        std::vector<std::vector<Node*>> m_Nodes;
        Scene* m_Scene{ nullptr };
        std::string m_LevelFilePath{};

        float m_TileSize{ 16.0f };
        float m_OffsetX{ 0.f };
        float m_OffsetY{ 0.f };
        float m_BaseY{ 40.0f };
        
		float m_NodeCenterOffsetX{ -0.4f };
		float m_NodeCenterOffsetY{ -0.4f };

        Node* m_CenterNode{ nullptr };
        std::vector<TunnelPair> m_TunnelPairs;

		PelletColors m_PelletColors{ };
    };
}