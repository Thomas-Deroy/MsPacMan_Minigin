#pragma once
#include "IFruitState.h"
#include "GameObject.h"

namespace dae {
    class FruitHiddenState : public IFruitState 
        {
        public:
            void Enter(FruitComponent& fruit) override;
            std::unique_ptr<IFruitState> Update(FruitComponent& fruit, float deltaTime) override;
            void Exit(FruitComponent& fruit) override;
        };
    }
