#include "FPSComponent.h"
#include <iomanip>
#include <sstream>

dae::FPSComponent::FPSComponent(GameObject* owner, std::shared_ptr<Font> font)
    : Component(owner)
{
    m_TextComponent = owner->GetComponent<TextComponent>();

    if (!m_TextComponent) m_TextComponent = owner->AddComponent<TextComponent>("FPS: 0", font);

}

void dae::FPSComponent::Update(float deltaTime)
{
    if (!m_TextComponent || deltaTime <= 0.0f)
        return;

    m_AccumulatedTime += deltaTime;

    if (m_AccumulatedTime >= 1.0f)
    {
        float avgFPS = 1 / deltaTime;
        avgFPS = std::min(avgFPS, 240.0f); // Cap the FPS to 240

        std::stringstream fpsStream;
        fpsStream << std::fixed << std::setprecision(1) << avgFPS;

        m_TextComponent->SetText(fpsStream.str() + " FPS");

        m_AccumulatedTime -= 1.0f;
    }
}

