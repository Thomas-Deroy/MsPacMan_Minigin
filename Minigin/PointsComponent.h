#pragma once
#include "Component.h"
#include "EventSystem.h"
#include <steam_api.h>

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
            EventSystem::GetInstance().Notify(EventID::PointsChanged);

            if (m_Score >= 500)
            {
				UnlockAchievement("ACH_WIN_ONE_GAME");
            }
        }

        int GetScore() const { return m_Score; }



    private:
        int m_Score;

        void UnlockAchievement(const char* achievementID)
        {
            if (SteamUserStats() && SteamUserStats()->SetAchievement(achievementID))
            {
                SteamUserStats()->StoreStats();
            }
        }
    };
}
