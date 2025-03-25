#pragma once
#include "Component.h"
#include "EventSystem.h"
#include "TextComponent.h"
#include "PointsComponent.h"
#include "GameObject.h"

namespace dae
{
    class PointsDisplayComponent : public Component
    {
        public:
            PointsDisplayComponent(GameObject* owner, GameObject* player);
            ~PointsDisplayComponent() override = default;

            void OnPointsChanged();

        private:
            void UpdateText();

            GameObject* m_Player;
            TextComponent* m_TextComponent{ nullptr };
            PointsComponent* m_PointsComponent{ nullptr };
            int m_Score{ 0 };
        };
}
