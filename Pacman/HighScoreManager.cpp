#include "HighScoreManager.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <filesystem>

void HighScoreManager::SaveHighScore(const std::string& name, int score, const std::string& file) {
    
    std::ofstream outFile(file, std::ios::binary);
	m_Score = score;
    std::string highscore = std::to_string(score) + " - " + name;

    if (outFile) {
        size_t size = highscore.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(highscore.data(), size);
        outFile.close();
    }

}

void HighScoreManager::ReadHighScore(const std::string& file) {

    std::ifstream inFile(file, std::ios::binary);

    if (inFile) {
        size_t size = 0;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        m_HighScore.resize(size);
        inFile.read(&m_HighScore[0], size);
        inFile.close();
    }
}

int HighScoreManager::GetTopScore() const {
    return m_Score;
}

std::string HighScoreManager::GetTopScoreDisplay() const {
    
    if (m_HighScore == "") return "000000 - NAME";
    else return m_HighScore;
}
