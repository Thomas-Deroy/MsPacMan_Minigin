#pragma once
#include "Component.h"
#include "EventSystem.h"
#include <algorithm>

namespace dae
{
    class HealthComponent : public Component
    {
    public:
        HealthComponent(GameObject* owner, int maxHealth, EventId healthChangedEvent, EventId playerDiedEvent)
            : Component(owner), m_Health(maxHealth), m_MaxHealth(maxHealth), m_HealthChangedEvent(healthChangedEvent), m_PlayerDiedEvent(playerDiedEvent){
        }

        void TakeDamage(int amount)
        {
            m_Health -= amount;
            if (m_Health <= -1)
            {
                m_Health = 0;
                EventSystem::GetInstance().Notify(Event{ m_PlayerDiedEvent });
            }

            EventSystem::GetInstance().Notify(Event{ m_HealthChangedEvent });
        }

        void Heal(int)
        {
            //m_Health = std::min(m_Health + amount, m_MaxHealth);
            EventSystem::GetInstance().Notify(Event{ m_HealthChangedEvent });
        }

        int GetHealth() const { return m_Health; }
        int GetMaxHealth() const { return m_MaxHealth; }

        EventId GetHealthChangedEvent() const { return m_HealthChangedEvent; }
        EventId GetPlayerDiedEvent() const { return m_PlayerDiedEvent; }

    private:
        int m_Health;
        int m_MaxHealth;
        EventId m_HealthChangedEvent;
        EventId m_PlayerDiedEvent;
    };
}
