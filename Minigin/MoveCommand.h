#pragma once
#include "Command.h"
#include "GameObject.h"
#include <glm.hpp>

namespace dae
{
    class MoveCommand : public Command
    {
    public:
        MoveCommand(std::shared_ptr<GameObject> gameObject, glm::vec2 direction, float speed)
            : m_GameObject(std::move(gameObject)), m_Direction(direction), m_Speed(speed) {
        }

        void Execute() override
        {
            if (!m_GameObject) return;

            m_GameObject->AddVelocity(m_Direction.x * m_Speed, m_Direction.y * m_Speed);
        }

    private:
        std::shared_ptr<GameObject> m_GameObject;
        glm::vec2 m_Direction;
        float m_Speed;
    };
}
