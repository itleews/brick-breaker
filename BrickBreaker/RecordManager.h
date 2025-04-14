#pragma once

class HighScoreManager {
public:
    static int LoadHighScore();
    static void SaveHighScore(int score);
};