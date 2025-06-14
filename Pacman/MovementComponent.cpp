#include "MovementComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "GhostAIComponent.h"
#include "GhostChaseState.h"
#include <queue>

namespace dae
{
    MovementComponent::MovementComponent(GameObject* owner, float speed, AnimationType type, LevelBuilder* builder)
		: Component(owner), m_Owner(owner), m_BaseSpeed(speed), m_AnimationType(type)
    {
        m_Sprite = owner->GetComponent<SpriteComponent>();
		m_LevelBuilder = builder;
    }

    void MovementComponent::Update(float deltaTime)
    {
        m_Speed = m_BaseSpeed * m_SpeedMultiplier;

        if (!m_CurrentNode) return;

        if (!m_TargetNode)
        {
            if (!m_Path.empty() && m_CurrentPathIndex < m_Path.size()) // If we have a path to follow
            {
                m_TargetNode = m_Path[m_CurrentPathIndex];
                ++m_CurrentPathIndex;
            }
            else
            {
                m_Path.clear();
                m_CurrentPathIndex = 0;

                if (CanMove(m_NextDirection))
                    m_CurrentDirection = m_NextDirection;
                else if (!CanMove(m_CurrentDirection))
                    return;

                for (auto neighbor : m_CurrentNode->neighbors)
                {
                    glm::vec2 dirToNeighbor = neighbor->position - m_CurrentNode->position;
                    glm::vec2 normDir = glm::normalize(dirToNeighbor);
                    glm::vec2 currentNorm = glm::normalize(m_CurrentDirection);

                    if (normDir == currentNorm)
                    {
                        m_TargetNode = neighbor;
                        break;
                    }
                }
            }

        }

        if (m_TargetNode)
        {
            glm::vec2 currentPos = m_Owner->GetTransform().GetWorldPosition();
            glm::vec2 targetPos = m_TargetNode->position;

            if (isnan(currentPos.x) || isnan(currentPos.y)) {
                currentPos = m_CurrentNode ? m_CurrentNode->position : glm::vec2(0, 0);
            }

            glm::vec2 direction =  SafeNormalize(targetPos - currentPos);

            glm::vec2 move = direction * m_Speed * deltaTime;
            if (glm::length(targetPos - (currentPos + move)) < glm::length(move))
            {
                // Snap to node
                m_Owner->SetPosition(targetPos.x, targetPos.y, 0.0f);
                MoveToNode(m_TargetNode);
            }
            else
            {
                glm::vec3 newPos = m_Owner->GetTransform().GetWorldPosition() + glm::vec3(move, 0.0f);
                m_Owner->SetPosition(newPos.x, newPos.y, newPos.z);
            }
        }
        UpdateAnimationRow();
    }

    void MovementComponent::ReverseDirection()
    {
        m_CurrentDirection = -m_CurrentDirection;

        if (m_NextDirection != glm::vec2{ 0, 0 })
        {
            m_NextDirection = -m_NextDirection;
        }

        if (m_TargetNode)
        {
            Node* temp = m_CurrentNode;
            m_CurrentNode = m_TargetNode;
            m_TargetNode = temp;
        }
    }

    bool MovementComponent::CanMove(const glm::vec2& dir) const
    {
        if (!m_CurrentNode) return false;

        for (auto neighbor : m_CurrentNode->neighbors)
        {
            glm::vec2 dirToNeighbor = neighbor->position - m_CurrentNode->position;
            if (glm::normalize(dirToNeighbor) == glm::normalize(dir)) {
                if (m_IgnoreTunnels && neighbor->isTunnel)
                    return false;

                if (!neighbor->preferNot || m_IgnoreNodePreferences)
                    return true;
            }

        }
        return false;
    }

    void MovementComponent::MoveToNode(Node* node)
    {
        m_CurrentNode = node;
        m_TargetNode = nullptr;

        if (!m_LevelBuilder) return;
        // Tunnel logic
        if (node->isTunnel && !m_IgnoreTunnels)
        {
            for (const auto& pair : m_LevelBuilder->GetTunnelPairs())
            {
                if (node == pair.left)
                {
                    m_CurrentNode = pair.right;
					m_Owner->SetPosition(pair.right->position.x - 24.f, pair.right->position.y, 0.0f); 
                    m_TargetNode = nullptr;
                    FindAndSetNode(m_LevelBuilder);
					return;
                }
                else if (node == pair.right)
                {
                    m_CurrentNode = pair.left;
                    m_Owner->SetPosition(pair.left->position.x, pair.left->position.y, 0.0f);
                    m_TargetNode = nullptr;
					FindAndSetNode(m_LevelBuilder);
                    return;
                }
            }
        }
    }


    void MovementComponent::FindAndSetNode(LevelBuilder* builder)
    {
        if (!builder) return;

        auto* collider = m_Owner->GetComponent<ColliderComponent>();
        if (!collider) return;


        glm::vec2 center = collider->GetCenter();
        Node* startNode = builder->FindClosestNode(center);
        if (startNode)
        {
            m_CurrentNode = startNode;

            glm::vec2 nodeCenter = startNode->position;

            m_Owner->GetTransform().SetWorldPosition(glm::vec3(nodeCenter, 0.0f));
        }
    }

    void MovementComponent::UpdateAnimationRow()
    {
        if (!m_Sprite || m_AnimationType == AnimationType::None) return;

        if (m_AnimationType == AnimationType::Pacman)
        {
            if (m_CurrentDirection.x > 0) m_Sprite->SetAnimationRow(0);
            else if (m_CurrentDirection.x < 0) m_Sprite->SetAnimationRow(1);
            else if (m_CurrentDirection.y < 0) m_Sprite->SetAnimationRow(2);
            else if (m_CurrentDirection.y > 0) m_Sprite->SetAnimationRow(3);
			return;
        }

        if (m_Owner->GetComponent<GhostAIComponent>()->GetFrightened()) return;

		if (m_AnimationType == AnimationType::Ghost)
		{
            if (m_CurrentDirection.x > 0) m_Sprite->SetAnimationColumn(0);
            else if (m_CurrentDirection.x < 0) m_Sprite->SetAnimationColumn(1);
            else if (m_CurrentDirection.y < 0) m_Sprite->SetAnimationColumn(2);
            else if (m_CurrentDirection.y > 0) m_Sprite->SetAnimationColumn(3);
			return;
		}

    }

    void MovementComponent::SetTargetDestination(Node* destination)
    {
        if (!m_CurrentNode || !destination) return;

        m_FinalTargetNode = destination;

        std::unordered_map<Node*, Node*> cameFrom;
        std::queue<Node*> frontier;
        frontier.push(m_CurrentNode);
        cameFrom[m_CurrentNode] = nullptr;

        while (!frontier.empty())
        {
            Node* current = frontier.front();
            frontier.pop();

            if (current == destination)
                break;

            for (Node* neighbor : current->neighbors)
            {
                if (cameFrom.find(neighbor) == cameFrom.end())
                {
                    frontier.push(neighbor);
                    cameFrom[neighbor] = current;
                }
            }
        }

        // Reconstruct path
        m_Path.clear();
        Node* current = destination;
        while (current && current != m_CurrentNode)
        {
            m_Path.insert(m_Path.begin(), current);
            current = cameFrom[current];
        }
        m_CurrentPathIndex = 0;
    }

    void MovementComponent::SetNextDirection(const glm::vec2& dir)
    {
        if (dir != glm::vec2{ 0, 0 }) m_NextDirection = dir;
    }

    void MovementComponent::SetCurrentNode(Node* node)
    {
        m_CurrentNode = node;
        m_TargetNode = nullptr;
    }

    void MovementComponent::SetTargetNode(Node* targetNode)
    {
        m_TargetNode = targetNode;
    }

    const glm::vec2& MovementComponent::GetCurrentDirection() const
    {
        return m_CurrentDirection;
    }

    const glm::vec2& MovementComponent::GetNextDirection() const
    {
        return m_NextDirection;
    }

    glm::vec2 dae::MovementComponent::SafeNormalize(const glm::vec2& vec) const {
        float len = glm::length(vec);
        return len > 0 ? vec / len : glm::vec2(1, 0);
    }

    void dae::MovementComponent::Reset()
    {
        m_CurrentDirection = glm::vec2{ 0, 0 };
        m_NextDirection = glm::vec2{ 0, 0 };

        m_Path.clear();
        m_CurrentPathIndex = 0;
        m_TargetNode = nullptr;
        m_FinalTargetNode = nullptr;

        m_IgnoreNodePreferences = false;

        UpdateAnimationRow();
    }

    void dae::MovementComponent::Reset(LevelBuilder* builder)
    {
        m_CurrentDirection = glm::vec2{ 0, 0 };
        m_NextDirection = glm::vec2{ 0, 0 };

        m_Path.clear();
        m_CurrentPathIndex = 0;
        m_TargetNode = nullptr;
        m_FinalTargetNode = nullptr;

        m_IgnoreNodePreferences = false;
		FindAndSetNode(builder);

        UpdateAnimationRow();
    }
}
