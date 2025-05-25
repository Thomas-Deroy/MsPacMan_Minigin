#pragma once
#include "IGhostState.h"
#include "GameObject.h"

namespace dae {
    class GhostScatterState : public IGhostState {
    public:
        void Enter(GhostAIComponent& ghost) override;
        void Update(GhostAIComponent& ghost, float deltaTime) override;
        void Exit(GhostAIComponent& ghost) override;
        glm::vec2 GetTargetPosition(const GhostAIComponent& ghost) const override;
    };
}