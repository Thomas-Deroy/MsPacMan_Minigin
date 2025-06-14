#include "LivesDisplayComponent.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "EventSystem.h"
#include "GameObject.h"

namespace dae
{
    LivesDisplayComponent::LivesDisplayComponent(GameObject* owner, GameObject* player, Scene* scene)
        : Component(owner), m_Player(player), m_Scene(scene)
    {
        m_IconSpacing = 36.0f;

        if (!GetOwner() || !m_Player) return;

        m_RenderComponent = GetOwner()->GetComponent<RenderComponent>();
        m_HealthComponent = m_Player->GetComponent<HealthComponent>();

        if (m_HealthComponent)
        {
            m_Lives = m_HealthComponent->GetHealth();
            EventSystem::GetInstance().Subscribe(PlayerDied, [this](const Event&) { OnPlayerDied(); });

            EventSystem::GetInstance().Subscribe(HealthChanged, [this](const Event&) { OnHealthChanged(); });

            m_Lives = m_HealthComponent->GetHealth();

            for (int i = 0; i < m_Lives; ++i)
            {
                AddLifeIcon(i);
            }
        }
    }


    void LivesDisplayComponent::OnPlayerDied()
    {
        UpdateDisplay();
    }

    void LivesDisplayComponent::OnHealthChanged()
    {
        m_Lives = m_HealthComponent->GetHealth();
        UpdateDisplay();
    }

    void LivesDisplayComponent::UpdateDisplay()
    {
        if (!m_HealthComponent || !GetOwner()) return;

        const int currentLives = m_HealthComponent->GetHealth();

        if (static_cast<int>(m_LifeIcons.size()) > currentLives)
        {
            while (static_cast<int>(m_LifeIcons.size()) > currentLives)
                RemoveLifeIcon();
        }
        else
        {
            while (static_cast<int>(m_LifeIcons.size()) < currentLives)
                AddLifeIcon(static_cast<int>(m_LifeIcons.size()));
        }

        for (size_t i = 0; i < m_LifeIcons.size(); ++i)
        {
            auto* icon = m_LifeIcons[i];
            if (icon)
            {
                icon->GetTransform().SetLocalPosition(glm::vec3(i * m_IconSpacing, 0.0f, 0.0f));
            }
        }
    }

    void LivesDisplayComponent::AddLifeIcon(int index)
    {
        auto icon = std::make_unique<GameObject>();
        icon->AddComponent<RenderComponent>("MSPC_Mspacman.tga", 2.f);

        icon->SetParent(GetOwner(), false);
        icon->GetTransform().SetLocalPosition(glm::vec3(index * m_IconSpacing, 0.0f, 0.0f));

        GameObject* iconRawPtr = icon.get();

        if (m_Scene)
            m_Scene->Add(icon);

        m_LifeIcons.emplace_back(iconRawPtr);
    }

    void LivesDisplayComponent::RemoveLifeIcon()
    {
        if (!m_LifeIcons.empty())
        {
            auto* icon = m_LifeIcons.back();

            if (icon)
            {
                icon->SetParent(nullptr, false);

                if (m_Scene && m_Scene->Contains(icon))
                {
                    m_Scene->Remove(icon);
                }

                m_LifeIcons.pop_back();
            }
        }
    }

    void LivesDisplayComponent::SetIconSpacing(float spacing)
    {
        m_IconSpacing = spacing;
        UpdateDisplay();
    }

}