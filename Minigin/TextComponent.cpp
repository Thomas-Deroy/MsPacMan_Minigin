#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "ResourceManager.h"
#include "GameObject.h"

namespace dae
{
    TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font)
        : Component(owner), m_text(text), m_font(std::move(font))
    {
        SetText(text);
    }

    void TextComponent::Update(float)
    {
        if (m_needsUpdate)
        {
            m_textTexture = ResourceManager::GetInstance().CreateTextTexture(m_text, m_font);
            m_needsUpdate = false;
        }
    }

    void TextComponent::Render() const
    {
        if (!m_textTexture || !GetOwner() || !m_isVisible)
            return;

        const auto& worldPos = GetOwner()->GetTransform().GetWorldPosition();

        RenderParams params{};
        params.x = worldPos.x;
        params.y = worldPos.y;
        params.color = m_color;

        Renderer::GetInstance().RenderTexture(*m_textTexture, params);

    }

    void TextComponent::SetText(const std::string& text)
    {
        if (m_text != text)
        {
            m_text = text;
            m_needsUpdate = true;
        }
    }

    void TextComponent::SetColor(float r, float g, float b, float a)
    {
        m_color = SDL_Color{
            static_cast<Uint8>(r * 255.0f),
            static_cast<Uint8>(g * 255.0f),
            static_cast<Uint8>(b * 255.0f),
            static_cast<Uint8>(a * 255.0f)
        };
        m_needsUpdate = true;
    }
}
