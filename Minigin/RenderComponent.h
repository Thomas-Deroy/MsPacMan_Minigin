#pragma once
#include "Component.h"
#include <memory>
#include <string>

namespace dae
{
    class Texture2D;

    class RenderComponent : public Component
    {
    public:
        RenderComponent(GameObject* owner);
        RenderComponent(GameObject* owner, const std::string& filename, float scaler = 1.0f);
        void Render() const override;

        void SetTexture(const std::string& filename);
        void SetScale(float scale);

        void SetTexture(std::shared_ptr<Texture2D> texture) { m_texture = texture; }
        std::shared_ptr<Texture2D> GetTexture() const { return m_texture; }
    private:
        std::shared_ptr<Texture2D> m_texture;
        float m_scale{ 1.0f };
    };
}
