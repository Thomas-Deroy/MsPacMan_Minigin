#pragma once
#include "Component.h"
#include <SDL.h>
#include <memory>
#include <string>

namespace dae
{
    class Font;
    class Texture2D;

    class TextComponent : public Component
    {
    public:
        TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font);

        void Update(float) override;
        void Render() const override;

        void SetText(const std::string& text);
        void SetColor(float r, float g, float b, float a = 1.f);

        void SetVisible(bool visible) { m_isVisible = visible; }
        bool IsVisible() const { return m_isVisible; }

    private:
        bool m_needsUpdate{ true };

        std::string m_text{};
        std::shared_ptr<Font> m_font;
        std::shared_ptr<Texture2D> m_textTexture;

        bool m_isVisible{ true };

        SDL_Color m_color{ 255, 255, 255, 255 }; 
    };
}
