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
        if (!m_textTexture || !GetOwner())
            return;

        const auto& worldPos = GetOwner()->GetTransform().GetWorldPosition();
        Renderer::GetInstance().RenderTexture(*m_textTexture, worldPos.x, worldPos.y);
    }

    void TextComponent::SetText(const std::string& text)
    {
        if (m_text != text)
        {
            m_text = text;
            m_needsUpdate = true;
        }
    }
}
