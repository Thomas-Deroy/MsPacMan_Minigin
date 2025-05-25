#pragma once
#include "Command.h"
#include "GameObject.h"
#include "NameInputComponent.h"


namespace dae
{
    class LetterUpCommand : public Command
    {
    public:
        LetterUpCommand(GameObject* go) : m_GameObject(go) {}

        void Execute() override
        {
            auto comp = m_GameObject->GetComponent<NameInputComponent>();
            if (comp) {
                comp->SelectNextLetter();
            }
        }

    private:
        GameObject* m_GameObject;
    };

    class LetterDownCommand : public Command
    {
    public:
        LetterDownCommand(GameObject* go) : m_GameObject(go) {}

        void Execute() override
        {
            if (auto comp = m_GameObject->GetComponent<NameInputComponent>())
                comp->SelectPreviousLetter();
        }

    private:
        GameObject* m_GameObject;
    };

    class ConfirmLetterCommand : public Command
    {
    public:
        ConfirmLetterCommand(GameObject* go) : m_GameObject(go) {}

        void Execute() override
        {
            if (auto comp = m_GameObject->GetComponent<NameInputComponent>())
                comp->ConfirmLetter();
        }

    private:
        GameObject* m_GameObject;
    };

}