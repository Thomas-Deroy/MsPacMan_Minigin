#pragma once
#include "Component.h"
#include "EventSystem.h"

namespace dae
{
    class PointsComponent : public Component
    {
    public:
        PointsComponent(GameObject* owner, EventId pointsChangedEvent)
            : Component(owner), m_Score(0), m_PointsChangedEvent(pointsChangedEvent) {
        }

        void AddPoints(int amount)
        {
            m_Score += amount;
            EventSystem::GetInstance().Notify(Event{ m_PointsChangedEvent });
        }

        int GetScore() const { return m_Score; }

        EventId GetPointsChangedEvent() const { return m_PointsChangedEvent; }

    private:
        int m_Score;
        EventId m_PointsChangedEvent;
    };
}
