#pragma once
#include "Component.h"
#include "EventSystem.h"

namespace dae
{
    class HealthComponent : public Component
    {
    public:
        HealthComponent(GameObject* owner, int maxHealth)
            : Component(owner), m_Health(maxHealth), m_MaxHealth(maxHealth) {
        }

        void TakeDamage(int amount)
        {
            m_Health -= amount;
            if (m_Health <= 0)
            {
                m_Health = 0;
                EventSystem::GetInstance().Notify(EventID::PlayerDied);
            }

            EventSystem::GetInstance().Notify(EventID::HealthChanged);
        }

        void Heal(int amount)
        {
            m_Health = std::min(m_Health + amount, m_MaxHealth);
            EventSystem::GetInstance().Notify(EventID::HealthChanged);
        }

        int GetHealth() const { return m_Health; }
        int GetMaxHealth() const { return m_MaxHealth; }

    private:
        int m_Health;
        int m_MaxHealth;
    };
}
