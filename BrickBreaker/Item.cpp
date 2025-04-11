#include "pch.h"
#include "Item.h"

void Item::Move() {
	y += 5; // 아이템이 아래로 떨어지도록 이동
}

void Item::Draw(CDC* pDC) {
    CBrush brush;
    switch (type) {
    case 1: brush.CreateSolidBrush(RGB(0, 255, 0)); break; // 예: 체력 회복
    case 2: brush.CreateSolidBrush(RGB(0, 0, 255)); break; // 예: 스피드 업
    case 3: brush.CreateSolidBrush(RGB(255, 255, 0)); break; // 예: 점수 업
    }
    CBrush* oldBrush = pDC->SelectObject(&brush);
    pDC->Ellipse(x, y, x + width, y + height);
    pDC->SelectObject(oldBrush);
}