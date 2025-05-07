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

class HighScoreManager : public dae::Singleton<HighScoreManager> {
public:
    void SaveHighScore(const std::string& name, int score, const std::string& file = "../Data/highscore.bin");
    void ReadHighScore(const std::string& file = "../Data/highscore.bin");
    std::string GetTopScoreDisplay() const;
	int GetTopScore() const;
private:
    std::string m_HighScore{ "" };
	int m_Score{ 0 };

    friend class dae::Singleton<HighScoreManager>; 
    HighScoreManager() = default; 
};
