#pragma once
#include "Command.h"
#include "PointsComponent.h"
#include <memory>

namespace dae
{
    class AddPointsCommand : public Command
    {
    public:
        AddPointsCommand(GameObject* player, int points)
            : m_Player(player), m_Points(points) {
        }

        void Execute() override
        {
            if (m_Player)
            {
                auto points = m_Player->GetComponent<PointsComponent>();
                if (points)
                {
                    points->AddPoints(m_Points);
                }
            }
        }

    private:
        GameObject* m_Player;
        int m_Points;
    };
}
