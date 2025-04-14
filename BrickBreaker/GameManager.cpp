#include "pch.h"  
#include "GameManager.h"  
#include "ChildView.h"
#include "RecordManager.h"
#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()
#include <print>

void GameManager::StartGame(CRect boundary, CWnd* pWnd) {
    // 게임 영역의 중앙 계산
    int centerX = boundary.left + (boundary.Width() / 2);

    // 패들 배치 (화면 중앙 아래에 배치)
    int paddleWidth = 200 - m_level * 10;
    int paddleHeight = 20;
    int paddleX = centerX - paddleWidth / 2;
    int paddleY = boundary.bottom - paddleHeight - 10;

    paddles.push_back(Paddle(paddleX, paddleY, paddleWidth, paddleHeight, 0));

    int ballRadius = 20;
    int baseSpeed = 20;
    double speedMultiplier = 1.0 + (m_level - 1) * 0.2;
    m_ball_dx = static_cast<int>(baseSpeed * speedMultiplier);
    m_ball_dy = static_cast<int>(-baseSpeed * speedMultiplier);

	GenerateBall(paddleX, paddleY, paddleWidth);

	DrawBricks(boundary); // 벽돌 그리기
}

void GameManager::EndGame(CWnd* pWnd) {  
    // 게임 종료
    KillTimer(pWnd->GetSafeHwnd(), 1);
}  

void GameManager::ResetGame(const CRect& boundary, CWnd* pWnd) {
    // 게임 상태 초기화
    m_brickCount = 0;
    m_level = 1;
    m_life = 3;
    m_nextBallId = 0;
    balls.clear();
    paddles.clear();
    bricks.clear();
    items.clear();
    StartGame(boundary, pWnd);
    CChildView* pChildView = static_cast<CChildView*>(pWnd);
    pChildView->m_startTick = GetTickCount64();
}

void GameManager::NextLevel(const CRect& boundary, CWnd* pWnd) {
    m_level++;
    m_life = 3;
    balls.clear();
    paddles.clear();
    bricks.clear();
	items.clear();
    StartGame(boundary, pWnd);
}


void GameManager::DrawGame(CDC* pDC) {  
    // 게임 오브젝트 그리기
    for (auto& ball : balls) {  
        ball.Draw(pDC);
    }  
    for (auto& paddle : paddles) {  
        paddle.Draw(pDC);
    }  
    for (auto& brick : bricks) {  
        if (!brick.isBroken) {
            brick.Draw(pDC);  
        }  
    }
    for (auto& item : items) {
        item.Draw(pDC);
    }
}

void GameManager::DestroyBall(Ball* ball) {
    auto it = std::remove_if(balls.begin(), balls.end(), [ball](const Ball& b) {
        return b == *ball; // 값 비교
        });
    balls.erase(it, balls.end());
}

void GameManager::GenerateBall(int paddle_x, int paddle_y, int paddle_width) {
	// 공 생성 로직
    int centerX = paddle_x + paddle_width / 2; // 공의 x 좌표
	int centerY = paddle_y - 20; // 공의 y 좌표  
    balls.push_back(Ball(m_nextBallId++, centerX, centerY, 20, m_ball_dx, m_ball_dy));
}

void GameManager::HandleCollisions(CWnd* pWnd) {
    CChildView* pChildView = static_cast<CChildView*>(pWnd);
    CRect boundary = pChildView->m_boundary;

    // 공과 벽돌, 공과 패들 충돌 체크
    
    for (auto& ball : balls) {
        double closestDistanceSquared = DBL_MAX;
        Brick* collidedBrick = nullptr;

        for (auto& brick : bricks) {
            if (brick.isBroken)
                continue;

            double closestX = clamp(ball.m_x, (double)brick.x, (double)(brick.x + brick.width));
            double closestY = clamp(ball.m_y, (double)brick.y, (double)(brick.y + brick.height));

            double dx = ball.m_x - closestX;
            double dy = ball.m_y - closestY;
            double distanceSquared = dx * dx + dy * dy;

            if (distanceSquared <= ball.m_radius * ball.m_radius && distanceSquared < closestDistanceSquared) {
                closestDistanceSquared = distanceSquared;
                collidedBrick = &brick;
            }
        }

        // 한 개만 처리
        if (collidedBrick) {
            //std::println("충돌! 볼 위치 ({}, {}) / 브릭 위치 [{}, {}]", ball.m_x, ball.m_y, collidedBrick->x, collidedBrick->y);

            collidedBrick->Break();
            collidedBrick->Update(boundary, pWnd);

            if (collidedBrick->itemType != 0) {
                items.push_back(Item(collidedBrick->x + collidedBrick->width / 2, collidedBrick->y, (ItemType)collidedBrick->itemType));
            }

            if (collidedBrick->isBroken) {
                m_brickCount++;
                m_stageClear--;
            }

            // 방향성 변경
            if (ball.m_dy < 0) {
                //std::println("y 방향성 변경");
                ball.m_dy = -ball.m_dy;
            }

            if (ball.m_y <= collidedBrick->y + collidedBrick->height) {
                //std::println("x 방향성 변경");
                ball.m_dx = -ball.m_dx;
            }
        }
    }

    for (auto& paddle : paddles) 
    {
        for (auto& ball : balls)
        {
            double closestX = clamp(ball.m_x, (double)paddle.x, (double)(paddle.x + paddle.width));
            double closestY = clamp(ball.m_y, (double)paddle.y, (double)(paddle.y + paddle.height));

            double dx = ball.m_x - closestX;
            double dy = ball.m_y - closestY;

            double distanceSquared = dx * dx + dy * dy;

            if (distanceSquared > ball.m_radius * ball.m_radius) // 공과 패들의 거리
                continue;
			
            // 방향성 변경
            if (ball.m_dy > 0) {
                //std::println("--패들-- y 방향성 변경");
                ball.m_dy = -ball.m_dy;
            }
                        
            if (ball.m_y + ball.m_radius >= paddle.y + paddle.height) {
				//std::println("**패들** x 방향성 변경");
                ball.m_dx = -ball.m_dx;
            }

        }
    }

    for (auto& item : items) {
        if (!item.isActive) continue;

        item.Update();

        for (auto& paddle : paddles) {
            CRect paddleRect(paddle.x, paddle.y, paddle.x + paddle.width, paddle.y + paddle.height);
            CRect itemRect = item.GetRect();
            CRect intersectRect;
            CString message;

            if (intersectRect.IntersectRect(&itemRect, &paddleRect)) {
                item.isActive = false;

                switch (item.type) {
                case ItemType::PaddleEnlarge:
                    paddle.width += 50;
                    message = _T("패들 사이즈 UP!");
                    break;
                case ItemType::Heal:
                    m_life++;
					message = _T("생명 +1!");
                    break;
                case ItemType::BallIncrease:
					GenerateBall(paddle.x, paddle.y, paddle.width);
                    message = _T("볼 +1!");
                    break;
                }

                // 메시지 저장
                effectMessages.push_back({
                    message,
                    CPoint(paddle.x + paddle.width / 2, paddle.y - 30),
                    60, // 약 1초
					item.type
                });
            }
        }
    }
}

void GameManager::DrawBricks(CRect boundary) {
    int rows = 10;
    int cols = 20;
    int brickWidth = (boundary.Width() - 110) / cols;
    int brickHeight = (boundary.Height() / 3) / rows;
    int startX = 20; // 시작 x 좌표
    int startY = 20; // 시작 y 좌표
    int gap = 5; // 벽돌 간의 간격
    m_stageClear = rows * cols;

    srand(time(0));  // 랜덤 시드 설정 (한 번만 호출)  

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = startX + col * (brickWidth + gap);
            int y = startY + row * (brickHeight + gap);

            int itemChance = rand() % 100;
            int itemType = 0;
            int hitCount = 1;
            int r = 0, g = 0, b = 0;
            if (itemChance < 15) {  // 15% 확률
                itemType = (rand() % 3) + 1; // 아이템 종류 1~3 중 랜덤
            }

            // 랜덤 내구도 (1~3)
            if (itemType == 0) {
                hitCount = (rand() % m_level) + 1;
                switch (hitCount) {
                case 1:
                    r = 255; g = 160; b = 122;  // Light Salmon
                    break;
                case 2:
                    r = 240; g = 128; b = 128;  // Light Coral
                    break;
                case 3:
                    r = 205; g = 92;  b = 92;   // Indian Red
                    break;
                default:
                    r = 139; g = 0; b = 0;  // Dark Red
                    break;
                }
            }
            else {
				// 아이템 색상
                r = 102; g = 205; b = 170;  // Medium Aquamarine
            }
            COLORREF color = RGB(r, g, b);

            bricks.push_back(Brick(x, y, brickWidth, brickHeight, color, hitCount, itemType));
        }
    }
}

int GameManager::HighScore() {
	// 최고 점수 계산
    int score = m_brickCount * 10;
    m_highScore = HighScoreManager::LoadHighScore();

    if (score > m_highScore) {
        HighScoreManager::SaveHighScore(score);
        m_highScore = score;
    }
    // std::println("현재 점수: {}, 최고 점수: {}", score, m_highScore);
	return m_highScore;
}