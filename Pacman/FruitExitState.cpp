#include "FruitExitState.h"
#include "FruitHiddenState.h"
#include "FruitComponent.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include <limits>

namespace dae {

    void FruitExitState::Enter(FruitComponent& fruit)
    {
        fruit.ResetStateTimer();
        LevelBuilder* builder = fruit.GetLevelBuilder();
        if (!builder) return;

        glm::vec2 currentPos = fruit.GetOwner()->GetTransform().GetWorldPosition();
        float closestDist = std::numeric_limits<float>::max();
        Node* closestTunnel = nullptr;

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

        Node* firstStepNode = nullptr;
        glm::vec2 direction{};

        for (Node* neighbor : closestTunnel->neighbors)
        {
            if (!neighbor) continue;

            glm::vec2 delta = neighbor->position - closestTunnel->position;

            if (std::abs(delta.y) < 1.0f)
            {
                direction = glm::normalize(delta);
                firstStepNode = neighbor;
                break;
            }
        }

        Node* secondStepNode = firstStepNode;

        if (firstStepNode)
        {
            for (Node* neighbor : firstStepNode->neighbors)
            {
                if (!neighbor) continue;

                glm::vec2 delta = neighbor->position - firstStepNode->position;
                if (glm::length(glm::normalize(delta) - direction) < 0.1f)
                {
                    secondStepNode = neighbor;
                    break;
                }
            }
        }

        m_ExitNode = secondStepNode;

        if (m_ExitNode)
        {
            fruit.GetMovement()->SetTargetDestination(m_ExitNode);
        }
    }

    std::unique_ptr<IFruitState> FruitExitState::Update(FruitComponent&, float)
    {
        return nullptr;
    }

    void FruitExitState::Exit(FruitComponent&) {}

}