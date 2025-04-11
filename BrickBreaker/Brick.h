#pragma once  
#include "GameObject.h"

class Brick : public GameObject {  
public:  
   COLORREF color;
   COLORREF originalColor;
   int hitCount; 
   int itemType;
   bool isBroken;

   Brick(int x, int y, int width, int height, COLORREF color, int hitCount, int itemType)
       : GameObject(x, y, width, height), color(color), originalColor(color), hitCount(hitCount), itemType(itemType), isBroken(false) {
   }

   void Draw(CDC* pDC) override;  
   bool Update(CRect boundary, CWnd* pWnd) override;
   void Break();  
};
