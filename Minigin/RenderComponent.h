#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include <SDL.h>

namespace dae
{
    enum class FadeEffect
    {
        None,
        FadeIn,
        FadeOut,
        FadeInAndOut,
        FadeOutAndIn
    };

    class Texture2D;

    class RenderComponent : public Component
    {
    public:
        RenderComponent(GameObject* owner);
        RenderComponent(GameObject* owner, const std::string& filename, float scaler = 1.0f);
		void Update(float deltaTime) override;
        void Render() const override;

        void SetTexture(const std::string& filename);
        void SetTexture(std::shared_ptr<Texture2D> texture) { m_texture = texture; }
        std::shared_ptr<Texture2D> GetTexture() const { return m_texture; }

        void SetScale(float scale);
        void SetColor(float r, float g, float b, float a = 1.0f);
        void SetVisible(bool visible) { m_isVisible = visible; }
        bool IsVisible() const { return m_isVisible; }
        void SetFadeEffect(FadeEffect fadeEffect, float duration);
    private:
        std::shared_ptr<Texture2D> m_texture;
        float m_scale{ 1.0f };
        SDL_Color m_color{ 255, 255, 255, 255 };
        bool m_isVisible{ true };

        FadeEffect m_fadeEffect{ FadeEffect::None };
        bool m_fadeActive{ false };
        float m_fadeDuration{ 1.0f };
        float m_fadeTimer{ 0.0f };
    };
}
