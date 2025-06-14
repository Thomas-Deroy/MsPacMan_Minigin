#include "HighScoreManager.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <filesystem>
#include "../3rdParty/nlohmann/json.hpp"
using json = nlohmann::json;

namespace dae
{
    void HighScoreManager::SaveHighScore(const std::string& name, int score) {
        json highScoreJson;
        highScoreJson["name"] = name;
        highScoreJson["score"] = score;

        std::ofstream outFile(m_FilePath);
        outFile << highScoreJson.dump(4);
    }

    void HighScoreManager::ReadHighScore() {
        std::ifstream inFile(m_FilePath);
        if (inFile) {
            json highScoreJson;
            inFile >> highScoreJson;

            m_Score = highScoreJson.value("score", 0);
            m_HighScore = std::to_string(m_Score) + " - " + highScoreJson.value("name", "NAME");
        }
    }

    int HighScoreManager::GetTopScore() const {
        return m_Score;
    }

    std::string HighScoreManager::GetTopScoreDisplay() const {
        if (m_HighScore.empty())
            return "0 - XXX";
        else
            return m_HighScore;
    }
}

