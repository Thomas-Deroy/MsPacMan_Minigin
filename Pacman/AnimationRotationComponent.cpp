#include "AnimationRotationComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include <cmath>

namespace dae
{
    AnimationRotationComponent::AnimationRotationComponent(GameObject* owner)
        : Component(owner)
    {
    }

    void AnimationRotationComponent::Rotate(int times, float stepDegrees)
    {
        if (times <= 0 || stepDegrees <= 0.f)
            return;

        m_remainingSteps = times * static_cast<int>(360.f / stepDegrees);
        m_stepDegrees = stepDegrees;
        m_isRotating = true;
        m_pauseTimer = 0.f;

        // Ensure the rotation is relative to the current rotation
        m_targetAngle = GetOwner()->GetTransform().GetRotation().z + m_stepDegrees;
		std::cout << "Starting rotation: " << m_targetAngle << " degrees, steps: " << m_remainingSteps << std::endl;
    }

    void AnimationRotationComponent::Update(float deltaTime)
    {
        if (!m_isRotating || m_remainingSteps <= 0)
            return;

        m_pauseTimer += deltaTime;

        if (m_pauseTimer >= m_pauseDuration)
        {
            StepToNextAngle();
            m_pauseTimer = 0.f;
        }
    }

    void AnimationRotationComponent::StepToNextAngle()
    {
        auto& transform = GetOwner()->GetTransform();
        transform.SetRotation({ 0.f, 0.f, m_targetAngle });

        m_remainingSteps--;
        if (m_remainingSteps > 0)
        {
            m_targetAngle += m_stepDegrees;
        }
        else
        {
            m_isRotating = false;
        }
    }
}
