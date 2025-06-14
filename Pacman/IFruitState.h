#pragma once
#include "GameObject.h"

namespace dae {
    class FruitComponent;

    class IFruitState {
    public:
        virtual ~IFruitState() = default;
        virtual void Enter(FruitComponent& fruit) = 0;
        virtual std::unique_ptr<IFruitState> Update(FruitComponent& fruit, float deltaTime) = 0;
        virtual void Exit(FruitComponent& fruit) = 0;
    };
}