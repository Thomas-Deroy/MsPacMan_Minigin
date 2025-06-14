#include "FruitHiddenState.h"
#include "FruitComponent.h"
#include "FruitEnterState.h"
#include "ColliderComponent.h"
#include "MovementComponent.h"

namespace dae {

    void FruitHiddenState::Enter(FruitComponent& fruit)
    {
        fruit.ResetStateTimer();
		fruit.GetSprite()->SetVisible(false);
        fruit.GetCollider()->SetLayer(dae::CollisionLayer::Default);
		fruit.GetMovement()->Reset();

        int x = 20 + std::rand() % 409;
        int y = 50 + (std::rand() % 467);

        fruit.GetOwner()->SetPosition(static_cast<float>(x), static_cast<float>(y));
		fruit.GetOwner()->SetActive(false);
    }

    std::unique_ptr<IFruitState> FruitHiddenState::Update(FruitComponent& fruit, float)
    {
        if (fruit.GetStateTimer() >= 5.f)
        {
            return std::make_unique<FruitEnterState>();
        }
        return nullptr;
    }

    void FruitHiddenState::Exit(FruitComponent& fruit)
    {
        fruit.GetSprite()->SetVisible(true);
        fruit.GetCollider()->SetLayer(dae::CollisionLayer::Object);
    }

}