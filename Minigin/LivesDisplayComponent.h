#pragma once
#include "Component.h"
#include "EventSystem.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"

namespace dae
{
    class LivesDisplayComponent : public Component
    {
        public:
            LivesDisplayComponent(GameObject* owner, GameObject* player);
            ~LivesDisplayComponent() override = default;

            void OnPlayerDied();
            void OnHealthChanged();

        private:
            void UpdateText();

            GameObject* m_Player;
            TextComponent* m_TextComponent{ nullptr };
            HealthComponent* m_HealthComponent{ nullptr };
            int m_Lives{ 0 };
        };
}
