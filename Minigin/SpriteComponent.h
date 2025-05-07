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

        void SetRotation(float angle);
        float GetRotation() const { return m_Rotation; }

    private:
        RenderComponent* m_RenderComponent;

        int m_Rows, m_Columns;
        int m_FrameWidth, m_FrameHeight;
        float m_FrameDelay, m_Scaler;
        float m_AccumulatedTime = 0;
        int m_CurrentFrame = 0;
        int m_CurrentRow = 0;
        int m_CurrentColumn = 0;

        float m_Rotation = 0.0f;
    };
}