#pragma once
#include "Component.h"
#include "EventSystem.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"

namespace dae
{
    class LivesDisplay : public Component
    {
    public:
        LivesDisplay(GameObject* owner, GameObject* player)
            : Component(owner), m_Player(player)
        {
            m_TextComponent = owner->GetComponent<TextComponent>();
            m_HealthComponent = player->GetComponent<HealthComponent>(); 

            if (m_HealthComponent)
            {
                m_Lives = m_HealthComponent->GetHealth(); 
            }

            UpdateText();

            EventSystem::GetInstance().Subscribe("PlayerDied", [this]() { OnPlayerDied(); });
            EventSystem::GetInstance().Subscribe("HealthChanged", [this]() { OnHealthChanged(); }); 
        }

        void OnPlayerDied()
        {
            if (m_HealthComponent)
            {
                m_Lives = 0; 
                UpdateText();
            }
        }

        void OnHealthChanged()
        {
            if (m_HealthComponent)
            {
                m_Lives = m_HealthComponent->GetHealth(); 
                UpdateText();
            }
        }

    private:
        void UpdateText()
        {
            if (m_TextComponent)
            {
                m_TextComponent->SetText("Lives: " + std::to_string(m_Lives));
            }
        }

        GameObject* m_Player;
        TextComponent* m_TextComponent{ nullptr };
        HealthComponent* m_HealthComponent{ nullptr };
        int m_Lives{ 0 };
    };
}
