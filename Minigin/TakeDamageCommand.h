#pragma once
#include "Command.h"
#include "HealthComponent.h"

namespace dae
{
    class TakeDamageCommand : public Command
    {
    public:
        TakeDamageCommand(std::shared_ptr<GameObject> player, int damage)
            : m_Player(player), m_Damage(damage) {
        }

        void Execute() override
        {
            if (m_Player)
            {
                auto health = m_Player->GetComponent<HealthComponent>();
                if (health)
                {
                    health->TakeDamage(m_Damage);
                }
            }
        }

    private:
        std::shared_ptr<GameObject> m_Player;
        int m_Damage;
    };
}
