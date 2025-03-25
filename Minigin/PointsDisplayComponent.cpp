#include "PointsDisplayComponent.h"

namespace dae
{
    PointsDisplayComponent::PointsDisplayComponent(GameObject* owner, GameObject* player)
        : Component(owner), m_Player(player)
    {
        m_TextComponent = owner->GetComponent<TextComponent>();
        m_PointsComponent = player->GetComponent<PointsComponent>();

        if (m_PointsComponent)
        {
            m_Score = m_PointsComponent->GetScore();
        }

        UpdateText();

        EventSystem::GetInstance().Subscribe(EventID::PointsChanged, [this]() { OnPointsChanged(); });
    }

    void PointsDisplayComponent::OnPointsChanged()
    {
        if (m_PointsComponent)
        {
            m_Score = m_PointsComponent->GetScore();
            UpdateText();
        }
    }

    void PointsDisplayComponent::UpdateText()
    {
        if (m_TextComponent)
        {
            m_TextComponent->SetText("Score: " + std::to_string(m_Score));
        }
    }
}