#include "pch.h"
#include "Item.h"

void Item::Move() {
	y += 5; // �������� �Ʒ��� ���������� �̵�
}

void Item::Draw(CDC* pDC) {
    CBrush brush;
    switch (type) {
    case 1: brush.CreateSolidBrush(RGB(0, 255, 0)); break; // ��: ü�� ȸ��
    case 2: brush.CreateSolidBrush(RGB(0, 0, 255)); break; // ��: ���ǵ� ��
    case 3: brush.CreateSolidBrush(RGB(255, 255, 0)); break; // ��: ���� ��
    }
    CBrush* oldBrush = pDC->SelectObject(&brush);
    pDC->Ellipse(x, y, x + width, y + height);
    pDC->SelectObject(oldBrush);
}