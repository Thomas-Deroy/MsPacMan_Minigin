#include "GhostScatterState.h"
#include "GhostChaseState.h"
#include "GhostAIComponent.h"
#include "MovementComponent.h"

namespace dae {

    void GhostScatterState::Enter(GhostAIComponent& ghost) {
        ghost.ResetStateTimer();
    }

    void GhostScatterState::Update(GhostAIComponent& ghost, float) {
        if (ghost.GetStateTimer() >= 7.0f)
            ghost.QueueStateChange(std::make_unique<GhostChaseState>());

        if (ghost.GetDirectionCooldown() <= 0.f)
        {
            auto target = GetTargetPosition(ghost);
            auto dir = ghost.ChooseDirectionToward(target);
            if (dir != glm::vec2(0, 0))
            {
                ghost.GetLastDirection() = dir;
                ghost.GetMovement()->SetNextDirection(dir);
                ghost.SetDirectionCooldown(0.15f);
            }
        }
    }

    void GhostScatterState::Exit(GhostAIComponent&) {}

    glm::vec2 GhostScatterState::GetTargetPosition(const GhostAIComponent& ghost) const {
        return ghost.CalculateScatterTarget();
    }

}