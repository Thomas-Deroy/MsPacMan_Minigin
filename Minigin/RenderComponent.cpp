#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "Texture2D.h"

dae::RenderComponent::RenderComponent(GameObject* owner)
    : Component(owner)
{
}

dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& filename, float scaler)
    : Component(owner), m_scale(scaler)
{
    SetTexture(filename);
}

void dae::RenderComponent::Update(float deltaTime)
{
    if (!m_fadeActive || m_fadeEffect == FadeEffect::None)
        return;

    m_fadeTimer += deltaTime;

    float progress = m_fadeTimer / m_fadeDuration;
    if (progress > 1.0f)
        progress = 1.0f;

    switch (m_fadeEffect)
    {
    case FadeEffect::FadeIn:
        m_color.a = static_cast<Uint8>(progress * 255.0f);
        break;
    case FadeEffect::FadeOut:
        m_color.a = static_cast<Uint8>((1.0f - progress) * 255.0f);
        break;
    case FadeEffect::FadeInAndOut:
        m_color.a = static_cast<Uint8>((progress < 0.5f ? progress * 2.0f : (1.0f - progress) * 2.0f) * 255.0f);
        break;
    case FadeEffect::FadeOutAndIn:
        m_color.a = static_cast<Uint8>((progress < 0.5f ? (1.0f - progress * 2.0f) : (progress - 0.5f) * 2.0f) * 255.0f);
        break;
    default:
        break;
    }

    if (m_fadeTimer >= m_fadeDuration)
    {
        m_fadeActive = false;
        m_fadeEffect = FadeEffect::None;

        // Clamp alpha to end state
        if (m_color.a > 255) m_color.a = 255;
        if (m_color.a < 0) m_color.a = 0;
    }
}

void dae::RenderComponent::Render() const
{
    if (!m_texture || !GetOwner() || !m_isVisible)
        return;

    // Sprite handles its own rendering
    if (GetOwner()->GetComponent<SpriteComponent>())
        return;

    const auto& worldPos = GetOwner()->GetTransform().GetWorldPosition();
    const auto& rotation = GetOwner()->GetTransform().GetRotation();
    float width = static_cast<float>(m_texture->GetWidth()) * m_scale;
    float height = static_cast<float>(m_texture->GetHeight()) * m_scale;

    Renderer::GetInstance().RenderTexture(*m_texture, worldPos.x, worldPos.y, width, height, m_color, rotation.z);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
    auto newTexture = ResourceManager::GetInstance().LoadTexture(filename);
    if (m_texture != newTexture) // different pointer
    {
        m_texture = newTexture;
    }
}

void dae::RenderComponent::SetScale(float scale)
{
    m_scale = scale;
}

void dae::RenderComponent::SetColor(float r, float g, float b, float a)
{
    m_color = SDL_Color{
        static_cast<Uint8>(r * 255.0f),
        static_cast<Uint8>(g * 255.0f),
        static_cast<Uint8>(b * 255.0f),
        static_cast<Uint8>(a * 255.0f)
    };
}

void dae::RenderComponent::SetFadeEffect(FadeEffect fadeEffect, float duration)
{
    m_fadeEffect = fadeEffect;
    m_fadeDuration = (duration > 0.01f) ? duration : 1.0f;
    m_fadeTimer = 0.0f;
    m_fadeActive = true;

    if (fadeEffect == FadeEffect::FadeIn || fadeEffect == FadeEffect::FadeInAndOut)
        m_color.a = 0;
    else if (fadeEffect == FadeEffect::FadeOut || fadeEffect == FadeEffect::FadeOutAndIn)
        m_color.a = 255;
}


