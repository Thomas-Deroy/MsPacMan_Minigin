#pragma once
#include "IFruitState.h"
#include "GameObject.h"

namespace dae {
    class FruitWanderState : public IFruitState {
    public:
        void Enter(FruitComponent& fruit) override;
        std::unique_ptr<IFruitState> Update(FruitComponent& fruit, float deltaTime) override;
        void Exit(FruitComponent& fruit) override;

    private:
        float m_ChangeTargetTimer{ 0.f };
        const float TARGET_INTERVAL = 4.0f;
        void SetNewTarget(FruitComponent& fruit);
    };
}
