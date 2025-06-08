#include "GhostExitHouseState.h"
#include "GhostEatenState.h"
#include "GhostScatterState.h"
#include "GhostAIComponent.h"
#include "MovementComponent.h"
#include "GhostChaseState.h"

namespace dae
{
    void GhostExitHouseState::Enter(GhostAIComponent& ghost)
    {
        ghost.ResetStateTimer();
        ghost.GetMovement()->SetSpeedMultiplier(0.50f);
        ghost.GetMovement()->SetIgnoreNodePreferences(true);
    }

    void GhostExitHouseState::Update(GhostAIComponent& ghost, float)
    {
        if (ghost.GetStateTimer() >= 2.0f)
        {
            ghost.QueueStateChange(std::make_unique<GhostScatterState>());
            return;
        }

        if (ghost.GetDirectionCooldown() <= 0.f)
        {
            auto target = glm::vec2(180.f, 200.f);
            auto dir = ghost.ChooseDirectionToward(target);
            if (dir != glm::vec2(0, 0))
            {
                ghost.GetLastDirection() = dir;
                ghost.GetMovement()->SetNextDirection(dir);
                ghost.SetDirectionCooldown(0.001f); // Needs to get out
            }
        }
    }

    void GhostExitHouseState::Exit(GhostAIComponent& ghost)
    {
        ghost.GetMovement()->SetSpeedMultiplier(1.f);
        ghost.GetMovement()->SetIgnoreNodePreferences(false);
    }

    glm::vec2 GhostExitHouseState::GetTargetPosition(const GhostAIComponent&) const
    {
        return glm::vec2(180.f, 200.f);
    }
}