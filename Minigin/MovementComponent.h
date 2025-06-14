#pragma once

#include "Component.h"
#include "GameObject.h"
#include "LevelBuilder.h" 
#include "ColliderComponent.h"
#include "LevelBuilder.h"

namespace dae
{
    struct Node;
    class GameObject;
	class LevelBuilder;
    class SpriteComponent;

    enum class AnimationType 
    {
        None,
        Pacman,
		Ghost
	};

    class MovementComponent : public Component
    {
    public:
        MovementComponent(GameObject* owner, float speed, AnimationType type, LevelBuilder* builder);

        void Update(float deltaTime) override;

        // Speed
        void SetSpeedMultiplier(float multiplier) { m_SpeedMultiplier = multiplier; }
        float GetSpeedMultiplier() const { return m_SpeedMultiplier; }

        void SetBaseSpeed(float speed) { m_BaseSpeed = speed; }
        float GetBaseSpeed() const { return m_BaseSpeed; }

		// Direction
        void SetNextDirection(const glm::vec2& dir);
        void ReverseDirection();

        const glm::vec2& GetCurrentDirection() const;
        const glm::vec2& GetNextDirection() const;

        // Nodes
        void SetTargetNode(Node* targetNode);
		Node* GetTargetNode() const { return m_TargetNode; }
        void SetTargetDestination(Node* destination);
        Node* GetTargetDestination() const { return m_FinalTargetNode; }

        void SetCurrentNode(Node* node);
        Node* GetCurrentNode() const { return m_CurrentNode; }

        void FindAndSetNode(LevelBuilder* builder);

        void SetIgnoreNodePreferences(bool ignore) { m_IgnoreNodePreferences = ignore; }
		void SetIgnoreTunnels(bool ignore) { m_IgnoreTunnels = ignore; }

        glm::vec2 SafeNormalize(const glm::vec2& vec) const;
        
        void Reset();
        void Reset(LevelBuilder* builder);

    private:
        bool CanMove(const glm::vec2& dir) const;
        void MoveToNode(Node* node);

        void UpdateAnimationRow();

		float m_BaseSpeed{ 1.0f };
        float m_SpeedMultiplier{ 1.0f };
		float m_Speed{ 0.0f };

        GameObject* m_Owner{ nullptr };
        SpriteComponent* m_Sprite{ nullptr };
		LevelBuilder* m_LevelBuilder{ nullptr };

        Node* m_CurrentNode{ nullptr };
        Node* m_TargetNode{ nullptr };

		bool m_IgnoreTunnels{ false };
        bool m_IgnoreNodePreferences{ false };

		AnimationType m_AnimationType;

        glm::vec2 m_CurrentDirection{ 0, 0 };
        glm::vec2 m_NextDirection{ 0, 0 };

        std::vector<Node*> m_Path;
        size_t m_CurrentPathIndex = 0;
        Node* m_FinalTargetNode = nullptr;
    };
}
