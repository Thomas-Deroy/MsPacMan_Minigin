#pragma once
#include "Component.h"
#include "EventSystem.h"

namespace dae
{
    class PointsComponent : public Component
    {
    public:
        PointsComponent(GameObject* owner)
            : Component(owner), m_Score(0) {
        }

        void AddPoints(int amount)
        {
            m_Score += amount;
            EventSystem::GetInstance().Dispatch("PointsChanged");
        }

        int GetScore() const { return m_Score; }

    private:
        int m_Score;
    };
}
