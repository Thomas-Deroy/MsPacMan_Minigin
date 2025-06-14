#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include "Renderer.h"

namespace dae
{
    class Texture2D;

    class SpriteComponent : public Component
    {
    public:
        SpriteComponent(GameObject* owner, const std::string& texturePath, int rows, int columns, float frameDelay, float scaler = 1.0f);
        ~SpriteComponent();

        void Update(float elapsedSec) override;
        void Render() const override;

        void SetFrame(int frame) { m_CurrentFrame = frame; }
        int GetFrame() const { return m_CurrentFrame; }

        void SetAnimationRow(int row) { m_CurrentRow = row; }
        void SetAnimationColumn(int column) { m_CurrentColumn = column; }

        void SetLooping(bool looping) { m_IsLooping = looping; }
        bool IsLooping() const { return m_IsLooping; }

        void SetVisible(bool visible) { m_isVisible = visible; }
        bool IsVisible() const { return m_isVisible; }

        void SetSprite(const std::string& texturePath, int rows, int columns, float frameDelay, float scaler);

    private:
        RenderComponent* m_RenderComponent;

        int m_Rows{};
        int m_Columns{};
        int m_FrameWidth{};
        int m_FrameHeight{};
        float m_FrameDelay{};
        float m_Scaler{};
        float m_AccumulatedTime = 0;

        int m_CurrentFrame = 0;
        int m_CurrentRow = 0;
        int m_CurrentColumn = 0;

        bool m_IsLooping = true;
		bool m_isVisible = true;
    };
}