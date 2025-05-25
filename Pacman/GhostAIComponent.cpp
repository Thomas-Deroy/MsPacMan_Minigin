#include "GhostAIComponent.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include "GhostChaseState.h"
#include "GhostScatterState.h"
#include "GhostFrightenedState.h"
#include "GhostEatenState.h"
#include <algorithm>
#include <random>

namespace dae
{
    GhostAIComponent::GhostAIComponent(GameObject* owner, GameObject* player, GhostType type, bool isPlayer)
        : Component(owner), m_Player(player), m_Type(type)
    {
        m_MovementComponent = owner->GetComponent<MovementComponent>();
		m_IsPlayer = isPlayer;
        ChangeState(std::make_unique<GhostScatterState>());
    }

    void GhostAIComponent::Update(float deltaTime)
    {
        if (!m_MovementComponent || !m_Player || !m_CurrentState) return;

        m_StateTimer += deltaTime;
        m_DirectionChangeCooldown -= deltaTime;

        m_CurrentState->Update(*this, deltaTime);

        if (m_QueuedState)
        {
            ChangeState(std::move(m_QueuedState));
            m_QueuedState = nullptr;
        }
    }

    void GhostAIComponent::QueueStateChange(std::unique_ptr<IGhostState> newState)
    {
        m_QueuedState = std::move(newState);
    }


    void GhostAIComponent::ChangeState(std::unique_ptr<IGhostState> newState)
    {
        if (m_CurrentState) m_CurrentState->Exit(*this);
        m_CurrentState = std::move(newState);
        m_StateTimer = 0.f;
        if (m_CurrentState) m_CurrentState->Enter(*this);
    }

    glm::vec2 GhostAIComponent::ChooseDirectionToward(const glm::vec2& targetPos)
    {
        if (!m_MovementComponent || m_IsPlayer) return { 0, 0 };

        Node* currentNode = m_MovementComponent->GetCurrentNode();
        if (!currentNode || currentNode->neighbors.empty())
            return { 0, 0 };

        glm::vec2 bestDir = m_LastDirection;
        float bestDistance = std::numeric_limits<float>::max();

        glm::vec2 oppositeDir = -m_LastDirection;

        for (auto neighbor : currentNode->neighbors)
        {
            if (!neighbor || !neighbor->isTraversable) continue;

            glm::vec2 dir = glm::normalize(neighbor->position - currentNode->position);

            if (dir == oppositeDir) continue;

            if (neighbor->isMerged)
            {
                glm::vec2 delta = neighbor->position - currentNode->position;
                if (delta.y > 0.1f)
                    continue;
            }

            float distance = glm::distance(targetPos, neighbor->position);
            if (distance < bestDistance)
            {
                bestDistance = distance;
                bestDir = dir;
            }
        }

        return bestDir;
    }

    glm::vec2 GhostAIComponent::GetTargetPosition() const
    {
        if (m_CurrentState)
            return m_CurrentState->GetTargetPosition(*this);

        return { 0, 0 };
    }

    glm::vec2 GhostAIComponent::CalculateChaseTarget() const
    {
        switch (m_Type)
        {
        case GhostType::Blinky: return BlinkyChase();
        case GhostType::Pinky: return PinkyChase();
        case GhostType::Inky: return InkyChase();
        case GhostType::Clyde: return ClydeChase();
        default: return { 0, 0 };
        }
    }

    glm::vec2 GhostAIComponent::BlinkyChase() const
    {
		glm::vec2 pacmanPos = m_Player->GetTransform().GetWorldPosition();
        return pacmanPos;
    }

    glm::vec2 GhostAIComponent::PinkyChase() const
    {
        glm::vec2 pacmanPos = m_Player->GetTransform().GetWorldPosition();
        glm::vec2 pacmanDir = m_Player->GetComponent<MovementComponent>()->GetCurrentDirection();
        return pacmanPos + (pacmanDir * 4.0f * 16.0f);
    }

    glm::vec2 GhostAIComponent::InkyChase() const
    {
        glm::vec2 pacmanPos = m_Player->GetTransform().GetWorldPosition();
        glm::vec2 pacmanDir = m_Player->GetComponent<MovementComponent>()->GetCurrentDirection();
        return pacmanPos + (pacmanDir * 2.0f * 16.0f) + glm::vec2(16, -16);
    }

    glm::vec2 GhostAIComponent::ClydeChase() const
    {
        glm::vec2 pacmanPos = m_Player->GetTransform().GetWorldPosition();
        glm::vec2 ghostPos = m_Owner->GetTransform().GetWorldPosition();

        float distance = glm::distance(pacmanPos, ghostPos);
        if (distance > 8 * 16)
            return pacmanPos;

        return GetScatterCorner();
    }

    glm::vec2 GhostAIComponent::CalculateScatterTarget() const
    {
        return GetScatterCorner();
    }

    glm::vec2 GhostAIComponent::GetScatterCorner() const
    {
        switch (m_Type)
        {
        case GhostType::Blinky: return { 208, -8 };
        case GhostType::Pinky: return { 0, -8 };
        case GhostType::Inky: return { 208, 248 };
        case GhostType::Clyde: return { 0, 248 };
        default: return { 0, 0 };
        }
    }

    void GhostAIComponent::SetFrightened(bool frightened)
    {
        if (frightened) {
            QueueStateChange(std::make_unique<GhostFrightenedState>());
        }
        else {
            QueueStateChange(std::make_unique<GhostChaseState>());
        }
            
    }
}
