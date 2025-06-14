#pragma once
#include "Component.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include <queue>

namespace dae
{
    class AnimationRotationComponent final : public Component
    {
    public:
        explicit AnimationRotationComponent(GameObject* owner);

        void Update(float deltaTime) override;
        void Rotate(int times, float stepDegrees);

    private:
        void StepToNextAngle();

		MovementComponent* m_MovementComponent{ nullptr };

        int m_remainingSteps{};         
        float m_stepDegrees{};          
        float m_targetAngle{};          
        float m_pauseDuration{ 0.15f }; 
        float m_pauseTimer{};           

        bool m_isRotating{ false };     
    };
}
