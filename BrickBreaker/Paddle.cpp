#include "pch.h"
#include "Paddle.h"

void Paddle::Draw(CDC* pDC) {
    CBrush brush(RGB(255, 0, 0));  // ���������� �е� �׸���
    pDC->SelectObject(&brush);
    pDC->Rectangle(x, y, x + width, y + height);
}

bool Paddle::Update(CRect boundary, CWnd* pWnd) {
    CRect oldRect(x - dx, y - dy, x + width + dx, y + height + dy);  // ���� ��ġ Ȯ��

    dx = 20;  // �е��� �̵� �ӵ�
    dy = 20;

    int HeightLimit = boundary.Height() - boundary.Height() / 5;  // ���� ����

    if (movingLeft && boundary.left < x - dx) {
        x -= dx;
    }
    else if (movingRight && boundary.right > x + width + dx) {
        x += dx;
    }
    if (movingUp && y - dy > HeightLimit) {
        y -= dy;
    }
    else if (movingDown && boundary.bottom > y + height + dy) {
        y += dy;
    }

    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);
    }

    return TRUE;
}