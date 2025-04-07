#pragma once
#include "GameObject.h"

class Paddle : public GameObject {
private:
	bool movingLeft = false;
	bool movingRight = false;
public:
	int dx; // 패들의 이동 속도

	Paddle(int x, int y, int width, int height, int dx)
		: GameObject(x, y, width, height), dx(dx) {
	}

	void Draw(CDC* pDC) override;
	bool Update(CRect boundary, CWnd* pWnd) override;
	void StartMovingLeft() { movingLeft = true; }
	void StartMovingRight() { movingRight = true; }
	void StopMoving() { movingLeft = false; movingRight = false; }
};