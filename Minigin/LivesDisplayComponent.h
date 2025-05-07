#pragma once
#include "GameObject.h"
#include "Component.h"
#include <vector>
#include <memory>
#include "Scene.h"

namespace dae
{
    class GameObject;
    class HealthComponent;
    class RenderComponent;

    class LivesDisplayComponent : public Component
    {
    public:
        LivesDisplayComponent(GameObject* owner, GameObject* player, Scene* scene);

        void SetIconSpacing(float spacing);

    private:
        void OnPlayerDied();
        void OnHealthChanged();
        void UpdateRender();
        void AddLifeIcon(int index);
        void RemoveLifeIcon();

        Scene* m_Scene{ nullptr };
        GameObject* m_Player;
        std::vector<GameObject*> m_LifeIcons;

        float m_IconSpacing{ 24.0f };

        RenderComponent* m_RenderComponent{ nullptr };
        HealthComponent* m_HealthComponent{ nullptr };
        int m_Lives{ 0 };
    };
}