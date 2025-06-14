#pragma once
#include "Command.h"
#include "HealthComponent.h"
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"
#include <memory>

namespace dae
{
    class TakeDamageCommand : public Command
    {
    public:
        TakeDamageCommand(GameObject* player, int damage)
            : m_Player(player), m_Damage(damage) {
        }

        void Execute() override
        {
            m_Player->GetComponent<HealthComponent>()->TakeDamage(m_Damage);
        }

    private:
        GameObject* m_Player;
        int m_Damage;
    };
}
