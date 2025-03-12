#pragma once
#include "Component.h"
#include "TextComponent.h"
#include "GameObject.h"

namespace dae
{
    class FPSComponent : public Component
    {
    public:
        FPSComponent(GameObject* owner, std::shared_ptr<Font> font);

        void Update(float deltaTime) override;

    private:
        TextComponent* m_TextComponent{};

        float m_AccumulatedTime{ 0.0f };
    };
}
