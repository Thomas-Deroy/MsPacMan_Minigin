#include "GhostChaseState.h"
#include "GhostScatterState.h"
#include "GhostAIComponent.h"
#include "MovementComponent.h"

namespace dae {

    void GhostChaseState::Enter(GhostAIComponent& ghost) {
        ghost.ResetStateTimer();
    }

    void GhostChaseState::Update(GhostAIComponent& ghost, float) {
		if (ghost.GetStateTimer() >= 20.0f) { //Time to switch to scatter 
            ghost.QueueStateChange(std::make_unique<GhostScatterState>());
			return;
        }

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

    void GhostChaseState::Exit(GhostAIComponent&) {}

    glm::vec2 GhostChaseState::GetTargetPosition(const GhostAIComponent& ghost) const {
        return ghost.CalculateChaseTarget();
    }

}