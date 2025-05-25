#include "GhostFrightenedState.h"
#include "GhostChaseState.h"
#include "GhostAIComponent.h"
#include "MovementComponent.h"
#include "SpriteComponent.h"

namespace dae {

    void GhostFrightenedState::Enter(GhostAIComponent& ghost) {
        ghost.ResetStateTimer();
        if (auto* move = ghost.GetMovement()) {
            move->SetSpeedMultiplier(0.75f);
            move->ReverseDirection();
        }

        ghost.GetSprite()->SetAnimationRow(4);
        ghost.GetSprite()->SetAnimationColumn(0);
    }

    void GhostFrightenedState::Update(GhostAIComponent& ghost, float) {
        if (ghost.GetStateTimer() >= 6.0f) {
            ghost.QueueStateChange(std::make_unique<GhostChaseState>());
            return;
        }

        if (ghost.GetStateTimer() >= 4.0f)
            ghost.GetSprite()->SetLooping(true);

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

    void GhostFrightenedState::Exit(GhostAIComponent& ghost) {
        if (auto* move = ghost.GetMovement()) move->SetSpeedMultiplier(1.0f);

        ghost.GetSprite()->SetLooping(false);
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
        case GhostType::Clyde:
            ghost.GetSprite()->SetAnimationRow(3);
            break;
        default:
            break;
        }
    }

    glm::vec2 GhostFrightenedState::GetTargetPosition(const GhostAIComponent& ghost) const {
        return ghost.GetOwner()->GetTransform().GetWorldPosition();
    }

}