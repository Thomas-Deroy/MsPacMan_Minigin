#pragma once
#include "Component.h"
#include "EventSystem.h"
#include "GameEvents.h"

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
            EventSystem::GetInstance().Notify(Event{ PointsChanged });
            if (m_Score >= m_NextLifeThreshold)
            {
                EventSystem::GetInstance().Notify(Event{ ExtraLife });
                m_NextLifeThreshold += 10000; 
            }
        }

        int GetScore() const { return m_Score; }

    private:
        int m_Score{ 0 };
        int m_NextLifeThreshold{ 10000 };
    };
}
