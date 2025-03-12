#pragma once
#include "Component.h"
#include "EventSystem.h"
#include "TextComponent.h"
#include "PointsComponent.h"
#include "GameObject.h"

namespace dae
{
    class PointsDisplay : public Component
    {
    public:
        PointsDisplay(GameObject* owner, GameObject* player)
            : Component(owner), m_Player(player)
        {
            m_TextComponent = owner->GetComponent<TextComponent>();
            m_PointsComponent = player->GetComponent<PointsComponent>();

            if (m_PointsComponent)
            {
                m_Score = m_PointsComponent->GetScore();
            }

            UpdateText();

            EventSystem::GetInstance().Subscribe("PointsChanged", [this]() { OnPointsChanged(); });
        }

        void OnPointsChanged()
        {
            if (m_PointsComponent)
            {
                m_Score = m_PointsComponent->GetScore();
                UpdateText();
            }
        }

    private:
        void UpdateText()
        {
            if (m_TextComponent)
            {
                m_TextComponent->SetText("Score: " + std::to_string(m_Score));
            }
        }

        GameObject* m_Player;
        TextComponent* m_TextComponent{ nullptr };
        PointsComponent* m_PointsComponent{ nullptr };
        int m_Score{ 0 };
    };
}
