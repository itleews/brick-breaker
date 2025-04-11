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
    float velocityY = -1.0f; // 위로 살짝 떠오르게
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
        y += 10; // 밑으로 떨어지게
    }

    void Draw(CDC* pDC) {
        if (!isActive) return;

        CPen pen(PS_SOLID, 5, RGB(0, 255, 0));
        CPen* pOldPen = pDC->SelectObject(&pen);

        int centerX = x + size / 2;
        int centerY = y + size / 2;
        int halfSize = size / 2;

        // 수평선
        pDC->MoveTo(centerX - halfSize, centerY);
        pDC->LineTo(centerX + halfSize, centerY);

        // 수직선
        pDC->MoveTo(centerX, centerY - halfSize);
        pDC->LineTo(centerX, centerY + halfSize);

        pDC->SelectObject(pOldPen);
    }


    CRect GetRect() const {
        return CRect(x, y, x + size, y + size);
    }
};