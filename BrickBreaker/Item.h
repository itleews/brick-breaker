#pragma once

enum class ItemType {
    None = 0,
    PaddleEnlarge = 1,
    Heal = 2,
    BallIncrease = 3
};

class EffectMessage {
public:
    CString text;
    CPoint position;
    int timer;
    ItemType type;
    float velocityY = -1.0f; // ���� ��¦ ��������
};

class Item {
public:
    int x, y, size;
    ItemType type;
    bool isActive;

    Item(int x, int y, ItemType type)
        : x(x), y(y), type(type), isActive(true), size(20) {
    }

    void Update() {
        y += 10; // ������ ��������
    }

    void Draw(CDC* pDC) {
        if (!isActive) return;

        CPen pen(PS_SOLID, 5, RGB(0, 255, 0));
        CPen* pOldPen = pDC->SelectObject(&pen);

        int centerX = x + size / 2;
        int centerY = y + size / 2;
        int halfSize = size / 2;

        // ����
        pDC->MoveTo(centerX - halfSize, centerY);
        pDC->LineTo(centerX + halfSize, centerY);

        // ������
        pDC->MoveTo(centerX, centerY - halfSize);
        pDC->LineTo(centerX, centerY + halfSize);

        pDC->SelectObject(pOldPen);
    }


    CRect GetRect() const {
        return CRect(x, y, x + size, y + size);
    }
};