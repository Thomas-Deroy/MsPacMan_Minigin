#pragma once
#include "Singleton.h" // include this so it knows about dae::Singleton

#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cassert>
#include <fstream> 

namespace dae
{
    class HighScoreManager : public dae::Singleton<HighScoreManager> {
    public:
        void SaveHighScore(const std::string& name, int score);
        void ReadHighScore();

        std::string GetTopScoreDisplay() const;
        int GetTopScore() const;
    private:
        std::string m_HighScore{ "" };
        int m_Score{ 0 };
        const std::string m_FilePath{ "../Data/highscore.json" };

        friend class dae::Singleton<HighScoreManager>;
        HighScoreManager() = default;
    };
}

