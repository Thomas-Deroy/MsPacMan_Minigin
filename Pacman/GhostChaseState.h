#pragma once
#include "IGhostState.h"
#include "GameObject.h"
#include <memory>

namespace dae {
    class GhostChaseState : public IGhostState {
    public:
        void Enter(GhostAIComponent& ghost) override;
        std::unique_ptr<IGhostState> Update(GhostAIComponent& ghost, float deltaTime) override;
        void Exit(GhostAIComponent& ghost) override;
        glm::vec2 GetTargetPosition(const GhostAIComponent& ghost) const override;
    };
}
