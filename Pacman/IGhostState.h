#pragma once
#include "GameObject.h"

namespace dae {
    class GhostAIComponent;

    class IGhostState {
    public:
        virtual ~IGhostState() = default;
        virtual glm::vec2 GetTargetPosition(const GhostAIComponent& ghostAI) const = 0;
        virtual void Enter(GhostAIComponent& ghost) = 0;
        virtual std::unique_ptr<IGhostState> Update(GhostAIComponent& ghost, float deltaTime) = 0;
        virtual void Exit(GhostAIComponent& ghost) = 0;
    };
}
