#include "pch.h"
#include "Brick.h"

void Brick::Draw(CDC* pDC) {
    if (!isBroken) {
        CBrush brush(RGB(0, 255, 0));  // �ʷϻ����� ���� �׸���
        pDC->SelectObject(&brush);
        pDC->Rectangle(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
    }
}

bool Brick::Update(CRect boundary, CWnd* pWnd) {
    CRect oldRect(x - width / 2, y - height / 2, x + width / 2, y + height / 2);

    // pWnd�� nullptr���� Ȯ���� �� ��ȿȭ
    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);  // CWnd�� ���� ��ȿȭ ��û
    }

	return TRUE;
}

void Brick::Break() {
    isBroken = true;
}
