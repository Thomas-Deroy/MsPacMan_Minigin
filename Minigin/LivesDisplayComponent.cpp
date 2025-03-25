#include "LivesDisplayComponent.h"

namespace dae
{
    LivesDisplayComponent::LivesDisplayComponent(GameObject* owner, GameObject* player)
        : Component(owner), m_Player(player)
    {
        m_TextComponent = owner->GetComponent<TextComponent>();
        m_HealthComponent = player->GetComponent<HealthComponent>();

        if (m_HealthComponent)
        {
            m_Lives = m_HealthComponent->GetHealth();
        }

        UpdateText();

        EventSystem::GetInstance().Subscribe(EventID::PlayerDied, [this]() { OnPlayerDied(); });
        EventSystem::GetInstance().Subscribe(EventID::HealthChanged, [this]() { OnHealthChanged(); });
    }

    void LivesDisplayComponent::OnPlayerDied()
    {
        if (m_HealthComponent)
        {
            m_Lives = 0;
            UpdateText();
        }
    }

    void LivesDisplayComponent::OnHealthChanged()
    {
        if (m_HealthComponent)
        {
            m_Lives = m_HealthComponent->GetHealth();
            UpdateText();
        }
    }

    void LivesDisplayComponent::UpdateText()
    {
        if (m_TextComponent)
        {
            m_TextComponent->SetText("Lives: " + std::to_string(m_Lives));
        }
    }
}