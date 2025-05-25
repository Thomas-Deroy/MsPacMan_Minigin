#pragma once
#include "Component.h"
#include "GameObject.h"

namespace dae
{
    class UIMovementComponent : public Component
    {
    public:
        UIMovementComponent(GameObject* owner, int maxIndex, float stepSize, const glm::vec2& direction)
            : Component(owner), m_MaxIndex(maxIndex), m_StepSize(stepSize), m_Direction(glm::normalize(direction))
        {
            m_StartPosition = owner->GetTransform().GetWorldPosition();
        }

        void MoveSelection(int delta)
        {
            int newIndex = m_CurrentIndex + delta;
            if (newIndex < 0 || newIndex >= m_MaxIndex) return;

            m_CurrentIndex = newIndex;
            glm::vec2 offset = m_Direction * m_StepSize * static_cast<float>(m_CurrentIndex);
            GetOwner()->GetTransform().SetWorldPosition(glm::vec3(m_StartPosition + offset, 0.0f));
        }

        void ResetSelection()
        {
            m_CurrentIndex = 0;
            GetOwner()->GetTransform().SetWorldPosition(glm::vec3(m_StartPosition, 0.0f)); 
        }

        int GetCurrentIndex() const { return m_CurrentIndex; }
        int GetMaxIndex() const { return m_MaxIndex; }

    private:
        glm::vec2 m_StartPosition{};
        glm::vec2 m_Direction{ 0.0f, 1.0f };
        float m_StepSize{ 30.0f };
        int m_CurrentIndex{ 0 };
        int m_MaxIndex{ 3 };
    };
}