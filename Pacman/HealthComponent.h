#pragma once
#include "Component.h"
#include "EventSystem.h"
#include <algorithm>
#include "GameEvents.h"

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
            if (m_Health <= -1)
            {
				m_IsDead = true;
                m_Health = 0;
                EventSystem::GetInstance().Notify(Event{ PlayerDied });
            }

            EventSystem::GetInstance().Notify(Event{ HealthChanged });
        }

        void AddHeal(int amount)
        {
            m_Health += amount;
            EventSystem::GetInstance().Notify(Event{ HealthChanged });
        }

        int GetHealth() const { return m_Health; }
        int GetMaxHealth() const { return m_MaxHealth; }
		bool IsDead() const { return m_IsDead; }

    private:
        int m_Health;
        int m_MaxHealth;
        bool m_IsDead{ false };
    };
}
