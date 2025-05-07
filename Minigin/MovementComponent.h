#pragma once

#include "Component.h"
#include "GameObject.h"
#include "LevelBuilder.h" 
#include "ColliderComponent.h"

namespace dae
{
    struct Node;
    class GameObject;
	class LevelBuilder;
    class SpriteComponent;

    enum class AnimationType 
    {
        Pacman,
		Ghost
	};

    class MovementComponent : public Component
    {
    public:
        MovementComponent(GameObject* owner, float speed, AnimationType type);

        void Update(float deltaTime) override;

        void SetNextDirection(const glm::vec2& dir);
        void SetCurrentNode(Node* node);
        bool CanMove(const glm::vec2& dir) const;
        void MoveToNode(Node* node);

        const glm::vec2& GetCurrentDirection() const;
        const glm::vec2& GetNextDirection() const;

        void FindAndSetStartNode(LevelBuilder* builder);

        void SetSpeedMultiplier(float multiplier) { m_SpeedMultiplier = multiplier; }
        float GetSpeedMultiplier() const { return m_SpeedMultiplier; }

        void ReverseDirection();

        void SetBaseSpeed(float speed) { m_BaseSpeed = speed; }
        float GetBaseSpeed() const { return m_BaseSpeed; }

        Node* GetCurrentNode() const { return m_CurrentNode; }

    private:
        void UpdateAnimationRow();

        float m_BaseSpeed;
        float m_SpeedMultiplier{ 1.0f };
        float m_Speed; 

        GameObject* m_Owner;
        SpriteComponent* m_Sprite;

        Node* m_CurrentNode{ nullptr };
        Node* m_TargetNode{ nullptr };

		AnimationType m_AnimationType;

        glm::vec2 m_CurrentDirection{ 0, 0 };
        glm::vec2 m_NextDirection{ 0, 0 };
    };
}
