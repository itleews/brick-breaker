#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <vector>
#include <cmath>

class GameManager {
public:
    int m_brickCount = 0;
    int m_stageClear;
    int m_ballCount;
    int m_highScore = 0;
    int m_level = 1;

    std::vector<Ball> balls;
    std::vector<Paddle> paddles;
    std::vector<Brick> bricks;

    void StartGame(CRect boundary, CWnd* pWnd);
    void DrawGame(CDC* pDC);
    void HandleCollisions(CWnd* pWnd);
    void EndGame(CWnd* pWnd);
    void ResetGame(const CRect& boundary, CWnd* pWnd);
	void NextLevel(const CRect& boundary, CWnd* pWnd);
    void DestroyBall(Ball* ball);
	void DrawBricks(CRect boundary);
    int HighScore();
    double clamp(double value, double min, double max) {
        return max(min, min(value, max));
    }
};
