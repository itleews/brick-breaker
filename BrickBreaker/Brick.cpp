// Brick.cpp
#include "pch.h"
#include "Brick.h"
#include "Item.h"
#include <afxwin.h>

void Brick::Draw(CDC* pDC) {
    if (!isBroken) {
        CBrush brush(color);
        pDC->SelectObject(&brush);
        pDC->Rectangle(x, y, x + width, y + height);
    }
}

bool Brick::Update(CRect boundary, CWnd* pWnd) {
    CRect oldRect(x, y, x + width, y + height);

    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);
    }

    return TRUE;
}

void Brick::Break() {  
   if (hitCount > 0) {  
       hitCount--;  // ������ ����  

       // ���� ��� ���� (���� hitCount�� ���� ���� �����)  
       if (hitCount > 0) {  
           int r = 0, g = 0, b = 0;
           switch (hitCount) {  
           case 1:
               r = 255; g = 160; b = 122;  // Light Salmon
               break;
           case 2:
               r = 240; g = 128; b = 128;  // Light Coral
               break;
           }  

           color = RGB(r, g, b); // ���� ������Ʈ  
       }  
       else if (hitCount == 0) {
           isBroken = true;  // ������ ������ �μ���  
       }  
   }  
}