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

void dae::RenderComponent::Render() const
{
    if (!m_texture || !GetOwner())
        return;

    // Sprite handles its own rendering
    if (GetOwner()->GetComponent<SpriteComponent>())
        return;

    const auto& worldPos = GetOwner()->GetTransform().GetWorldPosition();
    float width = static_cast<float>(m_texture->GetWidth()) * m_scale;
    float height = static_cast<float>(m_texture->GetHeight()) * m_scale;

    Renderer::GetInstance().RenderTexture(*m_texture, worldPos.x, worldPos.y, width, height);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetScale(float scale)
{
    m_scale = scale;
}
