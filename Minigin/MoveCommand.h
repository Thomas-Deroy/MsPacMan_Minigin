#pragma once
#include "Command.h"
#include "GameObject.h"
#include <glm.hpp>
#include "MovementComponent.h"

namespace dae
{
    class MoveCommand : public Command
    {
    public:
        MoveCommand(GameObject* gameObject, const glm::vec2& direction)
            : m_GameObject(gameObject), m_Direction(direction) {
        }

        void Execute() override
        {
            if (!m_GameObject) return;

            auto moveComp = m_GameObject->GetComponent<MovementComponent>();
            if (!moveComp) return;

            moveComp->SetNextDirection(m_Direction); 
        }

    private:
        GameObject* m_GameObject;
        glm::vec2 m_Direction;
    };
}
