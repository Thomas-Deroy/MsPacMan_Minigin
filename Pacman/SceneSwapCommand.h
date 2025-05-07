#pragma once
#include "Command.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "SceneManager.h"

namespace dae
{
    class SceneSwapCommand : public Command
    {
    public:
        SceneSwapCommand(GameObject* gameObject)
            : m_GameObject(gameObject) {
        }

        void Execute() override
        {
            if (!m_GameObject) return;

            dae::SceneManager::GetInstance().SetActiveScene("Game");
        }

    private:
        GameObject* m_GameObject;
        glm::vec2 m_Direction;
    };
}
