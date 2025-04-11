#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "Item.h"
#include <vector>
#include <cmath>

class GameManager {
public:
    int m_brickCount = 0;
    int m_ball_dx;
    int m_ball_dy;
    int m_stageClear;
    int m_highScore = 0;
    int m_life = 3;
    int m_level = 1;
    int m_nextBallId = 0;

    std::vector<Ball> balls;
    std::vector<Paddle> paddles;
    std::vector<Brick> bricks;
    std::vector<Item> items;
	std::vector<EffectMessage> effectMessages;

    void StartGame(CRect boundary, CWnd* pWnd);
    void DrawGame(CDC* pDC);
    void HandleCollisions(CWnd* pWnd);
    void EndGame(CWnd* pWnd);
    void ResetGame(const CRect& boundary, CWnd* pWnd);
	void NextLevel(const CRect& boundary, CWnd* pWnd);
    void DestroyBall(Ball* ball);
    void GenerateBall(int paddle_x, int paddle_y, int paddle_width);
	void DrawBricks(CRect boundary);
    int HighScore();
    double clamp(double value, double min, double max) {
        return max(min, min(value, max));
    }
};
