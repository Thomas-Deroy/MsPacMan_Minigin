#pragma once
#include "Command.h"
#include "GameObject.h"
#include "UIMovementComponent.h"

namespace dae
{
    class UIMoveCommand : public Command
    {
    public:
        // +1 (down) or -1 (up)
        UIMoveCommand(GameObject* object, int delta)
            : m_GameObject(object), m_Delta(delta) {
        }

        void Execute() override
        {
            m_GameObject->GetComponent<UIMovementComponent>()->MoveSelection(m_Delta);
        }

    private:
        GameObject* m_GameObject;
        int m_Delta;
    };
}