#pragma once
#include "Component.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include <memory>
#include "GhostFrightenedState.h"
#include "GhostEatenState.h"
#include "GhostExitHouseState.h"

namespace dae
{
    class MovementComponent;
    class GameObject;
    struct Node;
    class IGhostState;

    enum class GhostType { Blinky, Pinky, Inky, Clyde };

    class GhostAIComponent : public Component
    {
    public:
        GhostAIComponent(GameObject* owner, GameObject* player, GhostType type, bool isPlayer = false);

        void Update(float deltaTime) override;
        void QueueStateChange(std::unique_ptr<IGhostState> newState);
        void ChangeState(std::unique_ptr<IGhostState> newState);
        void SetFrightened(bool frightened);
        bool GetFrightened() { return typeid(*m_CurrentState) == typeid(GhostFrightenedState); }
        bool GetEaten() { return typeid(*m_CurrentState) == typeid(GhostEatenState); }
        bool GetExitState() { return typeid(*m_CurrentState) == typeid(GhostExitHouseState); }

        GhostType GetType() const { return m_Type; }
        MovementComponent* GetMovement() const { return m_MovementComponent; }
		SpriteComponent* GetSprite() const { return m_Owner->GetComponent<SpriteComponent>(); }

        glm::vec2 ChooseDirectionToward(const glm::vec2& targetPos);
        glm::vec2 GetTargetPosition() const;
        glm::vec2 CalculateChaseTarget() const;
        glm::vec2 CalculateScatterTarget() const;
        glm::vec2 GetScatterCorner() const;

        glm::vec2 BlinkyChase() const;
        glm::vec2 PinkyChase() const;
        glm::vec2 InkyChase() const;
        glm::vec2 ClydeChase() const;

        float GetStateTimer() const { return m_StateTimer; }
        void ResetStateTimer() { m_StateTimer = 0.f; }
        void SetDirectionCooldown(float time) { m_DirectionChangeCooldown = time; }
        float GetDirectionCooldown() const { return m_DirectionChangeCooldown; }
        glm::vec2& GetLastDirection() { return m_LastDirection; }

        void SetStartDelay(float delay);

    private:
        MovementComponent* m_MovementComponent;
        GameObject* m_Player;
        GhostType m_Type;

        std::unique_ptr<IGhostState> m_CurrentState;
        std::unique_ptr<IGhostState> m_QueuedState;

        float m_StateTimer = 0.f;
        glm::vec2 m_LastDirection = { 0, 0 };
        float m_DirectionChangeCooldown = 0.f;
		bool m_IsPlayer = false;

        bool m_ReadyToExit{ false };
        float m_ExitDelayTimer{ 0.f };
        float m_StartDelay{ 0.f };
		bool m_GhostExited{ true };
    };
}