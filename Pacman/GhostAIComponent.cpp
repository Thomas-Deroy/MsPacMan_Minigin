#include "GhostAIComponent.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include <algorithm>
#include <random>

namespace dae
{
    GhostAIComponent::GhostAIComponent(GameObject* owner, GameObject* pacman, GhostType type)
        : Component(owner), m_Pacman(pacman), m_Type(type)
    {
        m_MovementComponent = owner->GetComponent<MovementComponent>();
    }

    void GhostAIComponent::Update(float deltaTime)
    {
        if (!m_MovementComponent || !m_Pacman) return;

        // Update state timer and handle state transitions
        m_StateTimer += deltaTime;
        m_DirectionChangeCooldown -= deltaTime;

        switch (m_CurrentState)
        {
        case GhostState::Scatter:
            if (m_StateTimer >= m_ScatterDuration)
                SetState(GhostState::Chase);
            break;
        case GhostState::Chase:
            if (m_StateTimer >= m_ChaseDuration)
                SetState(GhostState::Scatter);
            break;
        case GhostState::Frightened:
            if (m_StateTimer >= m_FrightenedDuration)
                SetState(GhostState::Chase);
            break;
        case GhostState::Eaten:
            // Return to ghost house logic would go here
            break;
        }

        // Only change direction at certain intervals to reduce twitchiness
        if (m_DirectionChangeCooldown <= 0)
        {
            glm::vec2 targetPos = GetTargetPosition();
            glm::vec2 newDir = ChooseDirectionToward(targetPos);

            // Only update if we got a valid new direction
            if (newDir != glm::vec2(0, 0))
            {
                m_LastDirection = newDir;
                m_MovementComponent->SetNextDirection(newDir);
                m_DirectionChangeCooldown = 0.15f; // Only change direction every 0.15 seconds
            }
        }
    }

    void GhostAIComponent::SetState(GhostState newState)
    {
        if (m_CurrentState == newState) return;

        // State exit logic
        if (m_CurrentState == GhostState::Frightened)
        {
            if (m_MovementComponent)
                m_MovementComponent->SetSpeedMultiplier(1.0f);
        }

        m_CurrentState = newState;
        m_StateTimer = 0.f;

        // State entry logic
        if (newState == GhostState::Frightened)
        {
            if (m_MovementComponent)
            {
                m_MovementComponent->SetSpeedMultiplier(0.75f); // Slightly slower than original
                m_MovementComponent->ReverseDirection();
            }
        }
    }

    glm::vec2 GhostAIComponent::ChooseDirectionToward(const glm::vec2& targetPos)
    {
        if (!m_MovementComponent) return { 0, 0 };

        Node* currentNode = m_MovementComponent->GetCurrentNode();
        if (!currentNode || currentNode->neighbors.empty())
            return { 0, 0 };

        // In frightened state, choose random but valid direction
        if (m_CurrentState == GhostState::Frightened)
        {
            std::vector<glm::vec2> validDirections;
            for (auto neighbor : currentNode->neighbors)
            {
                if (neighbor && neighbor->isTraversable)
                {
                    glm::vec2 dir = glm::normalize(neighbor->position - currentNode->position);
                    // Avoid 180-degree turns (more natural movement)
                    if (glm::dot(dir, m_LastDirection) > -0.9f)
                        validDirections.push_back(dir);
                }
            }

            if (!validDirections.empty())
            {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, static_cast<int>(validDirections.size() - 1));
                return validDirections[dist(gen)];
            }
            return m_LastDirection;
        }

        // Normal state - find best direction (original Pac-Man rules)
        glm::vec2 bestDir = m_LastDirection;
        float bestDistance = std::numeric_limits<float>::max();

        // Ghosts can't reverse direction unless in frightened state
        glm::vec2 oppositeDir = -m_LastDirection;

        for (auto neighbor : currentNode->neighbors)
        {
            if (!neighbor || !neighbor->isTraversable) continue;

            glm::vec2 dir = glm::normalize(neighbor->position - currentNode->position);

            // Skip the opposite direction (except when entering frightened state)
            if (dir == oppositeDir) continue;

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
        switch (m_CurrentState)
        {
        case GhostState::Chase: return CalculateChaseTarget();
        case GhostState::Scatter: return CalculateScatterTarget();
        case GhostState::Frightened: return m_Owner->GetTransform().GetWorldPosition(); // Random movement
        case GhostState::Eaten: return { 112, 136 }; // Ghost house position (example coordinates)
        default: return { 0, 0 };
        }
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

    // Red ghost - direct chaser
    glm::vec2 GhostAIComponent::BlinkyChase() const
    {
        return m_Pacman->GetTransform().GetWorldPosition();
    }

    // Pink ghost - ambusher (targets 4 tiles ahead of Pac-Man)
    glm::vec2 GhostAIComponent::PinkyChase() const
    {
        glm::vec2 pacmanPos = m_Pacman->GetTransform().GetWorldPosition();
        glm::vec2 pacmanDir = m_Pacman->GetComponent<MovementComponent>()->GetCurrentDirection();

        // Target 4 tiles ahead of Pac-Man's direction
        return pacmanPos + (pacmanDir * 4.0f * 16.0f); // Assuming 16px per tile
    }

    // Cyan ghost - unpredictable (uses Blinky's position as reference)
    glm::vec2 GhostAIComponent::InkyChase() const
    {
        // This would need reference to Blinky's position in a real implementation
        // For now, we'll use a simple offset pattern
        glm::vec2 pacmanPos = m_Pacman->GetTransform().GetWorldPosition();
        glm::vec2 pacmanDir = m_Pacman->GetComponent<MovementComponent>()->GetCurrentDirection();

        // Example Inky behavior - more complex in real Pac-Man
        return pacmanPos + (pacmanDir * 2.0f * 16.0f) + glm::vec2(16, -16);
    }

    // Orange ghost - runs away when close
    glm::vec2 GhostAIComponent::ClydeChase() const
    {
        glm::vec2 pacmanPos = m_Pacman->GetTransform().GetWorldPosition();
        glm::vec2 ghostPos = m_Owner->GetTransform().GetWorldPosition();

        float distance = glm::distance(pacmanPos, ghostPos);

        // If far away, chase like Blinky
        if (distance > 8 * 16) // 8 tiles
            return pacmanPos;

        // If close, run to scatter corner
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
        case GhostType::Blinky: return { 208, -8 };   // Top-right
        case GhostType::Pinky: return { 0, -8 };       // Top-left
        case GhostType::Inky: return { 208, 248 };     // Bottom-right
        case GhostType::Clyde: return { 0, 248 };      // Bottom-left
        default: return { 0, 0 };
        }
    }

    void GhostAIComponent::SetFrightened(bool frightened)
    {
        SetState(frightened ? GhostState::Frightened : GhostState::Chase);
    }
}