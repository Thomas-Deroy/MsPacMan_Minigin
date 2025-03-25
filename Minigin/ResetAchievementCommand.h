#pragma once
#include "Command.h"
#include <steam_api.h>

namespace dae
{
    class ResetAchievementCommand : public Command
    {
    public:
        ResetAchievementCommand() = default;

        void Execute() override
        {
            if (SteamUserStats() && SteamUserStats()->ClearAchievement("ACH_WIN_ONE_GAME"))
            {
                SteamUserStats()->StoreStats(); 
            }
        }
    };
}
