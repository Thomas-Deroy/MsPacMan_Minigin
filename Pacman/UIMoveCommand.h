#pragma once
#include "Command.h"
#include "GameObject.h"
#include "UIMovementComponent.h"

namespace dae
{
    class UIMoveCommand : public Command
    {
    public:
        // delta should be +1 (down) or -1 (up)
        UIMoveCommand(GameObject* object, int delta)
            : m_pObject(object), m_Delta(delta) {
        }

        void Execute() override
        {
            if (!m_pObject) return;
            auto comp = m_pObject->GetComponent<UIMovementComponent>();
            if (!comp) return;
            comp->MoveSelection(m_Delta);
        }

    private:
        GameObject* m_pObject;
        int m_Delta;
    };
}