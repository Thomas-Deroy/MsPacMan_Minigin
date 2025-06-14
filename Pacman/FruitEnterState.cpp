#include "FruitEnterState.h"
#include "FruitComponent.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include "LevelBuilder.h"
#include "FruitExitState.h"
#include "FruitWanderState.h"
#include <limits>

namespace dae
{
    void FruitEnterState::Enter(FruitComponent& fruit)
    {
        fruit.ResetStateTimer();
        LevelBuilder* builder = fruit.GetLevelBuilder();
        if (!builder) return;

        glm::vec2 currentPos = fruit.GetOwner()->GetTransform().GetWorldPosition();
        float closestDist = std::numeric_limits<float>::max();
        Node* closestTunnel = nullptr;

        // Find the closest tunnel node
        for (Node* node : builder->GetAllNodes())
        {
            if (!node || !node->isTunnel)
                continue;

            float dist = glm::distance(currentPos, node->position);
            if (dist < closestDist)
            {
                closestDist = dist;
                closestTunnel = node;
            }
        }

        if (!closestTunnel) return;

        // Get the neighboring of the tunnel
        Node* firstStepNode = nullptr;
        for (Node* neighbor : closestTunnel->neighbors)
        {
            if (!neighbor) continue;

            glm::vec2 delta = neighbor->position - closestTunnel->position;

            if (std::abs(delta.y) < 1.0f)
            {
                firstStepNode = neighbor;
                break;
            }
        }

        Node* secondStepNode = nullptr;
        if (firstStepNode)
        {
            for (Node* neighbor : firstStepNode->neighbors)
            {
                if (neighbor && neighbor != closestTunnel)
                {
                    secondStepNode = neighbor;
                    break;
                }
            }
        }

        m_EntryTargetNode = secondStepNode;

        if (m_EntryTargetNode)
        {
            fruit.GetOwner()->SetPosition(m_EntryTargetNode->position);
            fruit.GetMovement()->FindAndSetNode(fruit.GetLevelBuilder());
            fruit.GetMovement()->SetTargetDestination(m_EntryTargetNode);
        }
    }

    std::unique_ptr<IFruitState> FruitEnterState::Update(FruitComponent& fruit, float)
    {
        if (fruit.GetStateTimer() >= 2.f)
        {
            return std::make_unique<FruitWanderState>();
        }
		return nullptr;
    }

    void FruitEnterState::Exit(FruitComponent&) {}
}
