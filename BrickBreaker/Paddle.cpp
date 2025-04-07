#include "pch.h"
#include "Paddle.h"

void Paddle::Draw(CDC* pDC) {
    CBrush brush(RGB(45, 52, 54));  // ���� ����
    pDC->SelectObject(&brush);  // �귯�� ����

    // �ձ� �𼭸��� ���� �簢�� �׸���
    pDC->RoundRect(x, y, x + width, y + height, 20, 20);  // 20�� �𼭸� ������
}

bool Paddle::Update(CRect boundary, CWnd* pWnd) {
    CRect oldRect(x - dx, y, x + width + dx, y + height);  // ���� ��ġ Ȯ��

    dx = 30;  // �е��� �̵� �ӵ�

    if (movingLeft && boundary.left < x - dx) {
        x -= dx;
    }
    else if (movingRight && boundary.right > x + width + dx) {
        x += dx;
    }

    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);
    }

    return TRUE;
}