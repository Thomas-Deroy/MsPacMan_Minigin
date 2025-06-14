#include "GhostEatenState.h"
#include "GhostScatterState.h"
#include "GhostAIComponent.h"
#include "MovementComponent.h"
#include "GhostChaseState.h"

namespace dae {

    void GhostEatenState::Enter(GhostAIComponent& ghost) {
        ghost.ResetStateTimer();

        ghost.GetMovement()->SetSpeedMultiplier(2.0f);
        ghost.GetMovement()->SetIgnoreNodePreferences(true);
        ghost.GetSprite()->SetAnimationRow(5);
    }

    std::unique_ptr<IGhostState> GhostEatenState::Update(GhostAIComponent& ghost, float) {
        glm::vec2 ghostPos = ghost.GetOwner()->GetTransform().GetWorldPosition();
        if (ghost.GetDirectionCooldown() <= 0.f)
        {
            auto target = GetTargetPosition(ghost);
            auto dir = ghost.ChooseDirectionToward(target);

            if (dir != glm::vec2(0, 0))
            {
                ghost.GetLastDirection() = dir;
                ghost.GetMovement()->SetNextDirection(dir);
                ghost.SetDirectionCooldown(0.10f);
            }
        }
		return nullptr;
    }

    void GhostEatenState::Exit(GhostAIComponent& ghost) {
        ghost.GetMovement()->SetSpeedMultiplier(1.0f);
        ghost.GetMovement()->SetIgnoreNodePreferences(false);

        switch (ghost.GetType())
        {
        case GhostType::Blinky:
            ghost.GetSprite()->SetAnimationRow(0);
            break;
        case GhostType::Pinky:
            ghost.GetSprite()->SetAnimationRow(1);
            break;
        case GhostType::Inky:
            ghost.GetSprite()->SetAnimationRow(2);
            break;
        case GhostType::Sue:
            ghost.GetSprite()->SetAnimationRow(3);
            break;
        default:
            break;
        }
    }

    glm::vec2 GhostEatenState::GetTargetPosition(const GhostAIComponent&) const {
        return { 112, 136 };
    }

}