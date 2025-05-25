#include "SpriteComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

namespace dae
{
    SpriteComponent::SpriteComponent(GameObject* owner, const std::string& texturePath, int rows, int columns, float frameDelay, float scaler)
        : Component(owner), m_Rows(rows), m_Columns(columns), m_FrameDelay(frameDelay), m_Scaler(scaler),
        m_CurrentRow(0), m_CurrentColumn(0)
    {
        m_RenderComponent = owner->GetComponent<RenderComponent>();
        if (!m_RenderComponent)
        {
            m_RenderComponent = owner->AddComponent<RenderComponent>(texturePath);
        }

        auto texture = ResourceManager::GetInstance().LoadTexture(texturePath);
        m_FrameWidth = texture->GetWidth() / m_Columns;
        m_FrameHeight = texture->GetHeight() / m_Rows;
    }

    SpriteComponent::~SpriteComponent() {}

    void SpriteComponent::Update(float elapsedSec)
    {
        if (!m_IsLooping)
            return;

        m_AccumulatedTime += elapsedSec;
        if (m_AccumulatedTime >= m_FrameDelay)
        {
            m_AccumulatedTime -= m_FrameDelay;
            m_CurrentColumn = (m_CurrentColumn + 1) % m_Columns;
        }
    }

    void SpriteComponent::Render() const
    {
        if (!m_RenderComponent)
            return;

        auto owner = GetOwner();
        if (!owner)
            return;

        auto worldPos = owner->GetTransform().GetWorldPosition();
        SDL_Rect srcRect{
            m_CurrentColumn * m_FrameWidth,
            m_CurrentRow * m_FrameHeight,
            m_FrameWidth,
            m_FrameHeight
        };

        SDL_Rect dstRect{
            static_cast<int>(worldPos.x),
            static_cast<int>(worldPos.y),
            static_cast<int>(m_FrameWidth * m_Scaler),
            static_cast<int>(m_FrameHeight * m_Scaler)
        };

        Renderer::GetInstance().RenderTexture(*m_RenderComponent->GetTexture(), dstRect, srcRect, m_Rotation, SDL_FLIP_NONE
        );
    }

    void SpriteComponent::SetRotation(float angle)
    {
        m_Rotation = angle;
    }

    void dae::SpriteComponent::SetSprite(const std::string& texturePath, int rows, int columns, float frameDelay, float scaler)
    {
        if (!m_RenderComponent)
        {
            m_RenderComponent = GetOwner()->AddComponent<RenderComponent>(texturePath, scaler);
        }
        else
        {
            m_RenderComponent->SetTexture(texturePath);
            m_RenderComponent->SetScale(scaler);
        }

        m_Rows = rows;
        m_Columns = columns;
        m_FrameDelay = frameDelay;
        m_Scaler = scaler;

        auto texture = ResourceManager::GetInstance().LoadTexture(texturePath);
        if (texture)
        {
            m_FrameWidth = texture->GetWidth() / m_Columns;
            m_FrameHeight = texture->GetHeight() / m_Rows;
        }

        m_CurrentRow = 0;
        m_CurrentColumn = 0;
        m_AccumulatedTime = 0.f;
    }


}