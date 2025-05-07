#pragma once
#include "Component.h"
#include "GameObject.h"

namespace dae
{
    class MovementComponent;
    class GameObject;
    struct Node;

    enum class GhostType { Blinky, Pinky, Inky, Clyde };

    class GhostAIComponent : public Component
    {
    public:
        enum class GhostState { Chase, Scatter, Frightened, Eaten };

        GhostAIComponent(GameObject* owner, GameObject* pacman, GhostType type);

        void Update(float deltaTime) override;
        void SetState(GhostState newState);
        void SetFrightened(bool frightened);

        GhostType GetType() const { return m_Type; }

    private:
        glm::vec2 ChooseDirectionToward(const glm::vec2& targetPos);
        glm::vec2 GetTargetPosition() const;

        glm::vec2 CalculateChaseTarget() const;
        glm::vec2 CalculateScatterTarget() const;
        glm::vec2 GetScatterCorner() const;

        glm::vec2 BlinkyChase() const;    // Red - direct chase
        glm::vec2 PinkyChase() const;     // Pink - ambush (4 tiles ahead)
        glm::vec2 InkyChase() const;      // Cyan - unpredictable
        glm::vec2 ClydeChase() const;     // Orange - runs away when close

        MovementComponent* m_MovementComponent;
        GameObject* m_Pacman;

        GhostState m_CurrentState = GhostState::Scatter;
        GhostType m_Type;

        float m_StateTimer = 0.f;
        const float m_ScatterDuration = 7.f;
        const float m_ChaseDuration = 20.f;
        const float m_FrightenedDuration = 6.f;

        glm::vec2 m_LastDirection = { 0, 0 };
        float m_DirectionChangeCooldown = 0.f;
    };
}