#include "FruitWanderState.h"
#include "FruitComponent.h"
#include "MovementComponent.h"
#include "LevelBuilder.h"
#include "FruitExitState.h"
#include <cstdlib>
#include <ctime>

namespace dae {

    void FruitWanderState::Enter(FruitComponent& fruit)
    {
        fruit.ResetStateTimer();
        SetNewTarget(fruit);
    }

    std::unique_ptr<IFruitState> FruitWanderState::Update(FruitComponent& fruit, float deltaTime)
    {
        if (fruit.GetStateTimer() >= 30.f)
        {
            return std::make_unique<FruitExitState>();
        }

        m_ChangeTargetTimer -= deltaTime;
        if (m_ChangeTargetTimer <= 0.f)
        {
            SetNewTarget(fruit);
            m_ChangeTargetTimer = TARGET_INTERVAL;
        }
        return nullptr;
    }

    void FruitWanderState::Exit(FruitComponent&) {}

    void FruitWanderState::SetNewTarget(FruitComponent& fruit)
    {
        int x = 40 + std::rand() % 300;
        int y = 60 + (std::rand() % 350);

        LevelBuilder* builder = fruit.GetLevelBuilder();
        if (!builder) return;

        Node* closestNode = builder->FindClosestNode(glm::vec2{ x, y });
        if (!closestNode) return;

        if (MovementComponent* movement = fruit.GetMovement())
        {
            movement->SetTargetDestination(closestNode);
        }
    }

}