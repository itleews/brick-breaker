
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "BrickBreaker.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
	m_startTick = 0;
	m_boundary.SetRect(0, 0, 100, 100);
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

enum GameState {
	GAME_RUNNING,
	GAME_WIN,
	GAME_LOSE,
	GAME_READY
};

GameState m_gameState = GAME_READY; // 초기 상태

// CChildView 메시지 처리기

BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (m_gameState == GAME_LOSE)
        {
            m_gameState = GAME_READY;
			KillTimer(2);
			m_bShowContinueMsg = true;
            Invalidate(); // 화면 다시 그리기
        }
		else if (m_gameState == GAME_WIN)
		{
			KillTimer(2);
			m_bShowContinueMsg = true;
			m_gameState = GAME_RUNNING;
			SetTimer(1, 16, nullptr);
			m_gameManager.NextLevel(m_boundary, this);
		}
    }
    return CWnd::PreTranslateMessage(pMsg);
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPCHILDREN;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}


void CChildView::OnPaint()  
{  
   CPaintDC dc(this); // 그리기 DC  
   CDC memDC;  
   memDC.CreateCompatibleDC(&dc);  

   CRect rect;  
   GetClientRect(&rect);  

   CBitmap bitmap;  
   bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());  
   CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);  

   DrawBackground(&memDC, rect);  
   DrawGameScene(&memDC);  
   DrawStatus(&memDC, rect);  

   switch (m_gameState) {  
   case GAME_RUNNING:  
       DrawGameScene(&memDC);  
       break;  
   case GAME_WIN:  
       DrawGameResultMessage(&memDC, rect);  
       break;  
   case GAME_LOSE:  
       DrawGameResultMessage(&memDC, rect);  
       break;  
   case GAME_READY:  
       DrawStartScreen(&memDC, rect);  
       break;  
   }  

   DrawItemMessage(&memDC, rect);

   dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);  

   memDC.SelectObject(pOldBitmap);  
   bitmap.DeleteObject();  
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_LEFT) {
		m_gameManager.paddles[0].StartMovingLeft();
	}
	else if (nChar == VK_RIGHT) {
		m_gameManager.paddles[0].StartMovingRight();
	}
	else if (nChar == VK_UP) {
		if (m_gameState == GAME_READY) {
			m_startTick = GetTickCount64();
			m_gameManager.ResetGame(m_boundary, this);
			m_gameState = GAME_RUNNING;
			SetTimer(1, 16, nullptr);
		}
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_LEFT || nChar == VK_RIGHT) {
		m_gameManager.paddles[0].StopMoving();
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CChildView::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent == 1) {
		ULONGLONG elapsedMillis = GetTickCount64() - m_startTick;
		int minutes = (elapsedMillis / (1000 * 60)) % 60;
		int seconds = (elapsedMillis / 1000) % 60;
		int milliseconds = elapsedMillis % 1000;
		
		m_strTime.Format(_T("%02d:%02d.%03d"), minutes, seconds, milliseconds);

		Invalidate(FALSE);

		for (auto& ball : m_gameManager.balls) {
			if (m_gameManager.m_stageClear == 0) {
				KillTimer(1);
				m_gameState = GAME_WIN;
				return;
			}

			if (ball.Update(m_boundary, this)) {
				continue;
			}

			// 공 제거 로직
			if (m_gameManager.balls.size() > 1) {
				m_gameManager.DestroyBall(&ball);
				continue;
			}

			// 공이 하나만 남았고 생명이 있으면 새 공 추가
			if (m_gameManager.balls.size() == 1 && m_gameManager.m_life > 0) {
				m_gameManager.DestroyBall(&ball);
				m_gameManager.GenerateBall(m_gameManager.paddles[0].x, m_gameManager.paddles[0].y, m_gameManager.paddles[0].width);
				m_gameManager.m_life--;
				continue;
			}

			if (m_gameManager.m_life == 0) {
				m_gameState = GAME_LOSE;
				m_gameManager.DestroyBall(&ball);
				m_gameManager.EndGame(this);
				highScore = m_gameManager.HighScore();
				SetTimer(2, 500, NULL);
				return;
			}
		}

		for (auto& paddle : m_gameManager.paddles) {
			paddle.Update(m_boundary, this);
		}
		for (auto& brick : m_gameManager.bricks) {
			brick.Update(m_boundary, this);
		}

		for (auto it = m_gameManager.effectMessages.begin(); it != m_gameManager.effectMessages.end(); ) {
			it->timer--;
			it->position.y += static_cast<int>(it->velocityY); // 위로 이동

			if (it->timer <= 0) {
				it = m_gameManager.effectMessages.erase(it);
			}
			else {
				++it;
			}
		}

		m_gameManager.HandleCollisions(this);
	}
	else if (nIDEvent == 2) {
		// 깜빡이기 토글
		m_bShowContinueMsg = !m_bShowContinueMsg;
		Invalidate(FALSE); // 다시 그리기
	}
	CWnd::OnTimer(nIDEvent);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CChildView::OnDestroy()
{
	KillTimer(1); // 타이머 중지
	CWnd::OnDestroy();
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (cx > 0 && cy > 0)
	{
		m_boundary.SetRect(10, 10, cx - 200, cy - 10);
		m_gameManager.ResetGame(m_boundary, this);
	}
}

void CChildView::DrawBackground(CDC* pDC, const CRect& rect)
{
	pDC->FillSolidRect(&rect, RGB(255, 255, 255)); // 흰 배경
	pDC->Rectangle(m_boundary); // 외곽선
}

void CChildView::DrawGameScene(CDC* pDC)
{
	m_gameManager.DrawGame(pDC);
}

void CChildView::DrawStatus(CDC* pDC, const CRect& rect)
{
	// 공통 설정
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);

	int baseX = rect.right - 190;
	int currentY = 10;

	// 상태 제목
	pDC->TextOutW(baseX, currentY, _T("게임 결과"));
	currentY += 30;

	// 시간 표시용 폰트 설정
	LOGFONT logFont = { 0 };
	logFont.lfHeight = 48;
	_tcscpy_s(logFont.lfFaceName, _T("맑은 고딕"));
	CFont font;
	font.CreateFontIndirect(&logFont);
	CFont* pOldFont = pDC->SelectObject(&font);

	pDC->TextOutW(baseX, currentY, m_strTime);
	currentY += 60;

	// 현재 레벨
	CString levelText;
	levelText.Format(_T("Lv. %d"), m_gameManager.m_level);
	pDC->TextOutW(baseX, currentY, levelText);
	currentY += 60;

	// 점수용 폰트 설정
	pDC->SelectObject(pOldFont); // 원래 폰트 복원

	LOGFONT scoreFont = { 0 };
	scoreFont.lfHeight = 25;
	CFont smallFont;
	smallFont.CreateFontIndirect(&scoreFont);
	pOldFont = pDC->SelectObject(&smallFont);

	// 현재 점수
	CString strScoreText = _T("현재 점수");
	pDC->TextOutW(baseX, currentY, strScoreText);
	currentY += 25;

	int score = m_gameManager.m_brickCount * 10;
	CString strScore;
	strScore.Format(_T("%d"), score);
	pDC->TextOutW(baseX, currentY, strScore);
	currentY += 40;

	// 최고 점수
	CString strHighScoreText = _T("최고 점수");
	pDC->TextOutW(baseX, currentY, strHighScoreText);
	currentY += 25;

	CString strHighScore;
	strHighScore.Format(_T("%d"), highScore);
	pDC->TextOutW(baseX, currentY, strHighScore);

	// 생명 표시
	currentY += 40;
	CString strLifeText = _T("남은 생명");
	pDC->TextOutW(baseX, currentY, strLifeText);
	currentY += 30;

	// 폰트 복원
	pDC->SelectObject(pOldFont);

	int heartSize = 30;
	int spacing = 5;
	for (int i = 0; i < m_gameManager.m_life; i++)
	{
		int x = baseX + i * (heartSize + spacing);
		int y = currentY;

		// 노란 브러시 생성
		CBrush yellowBrush(RGB(255, 255, 0));
		CBrush* pOldBrush = pDC->SelectObject(&yellowBrush);

		// 원(공) 그리기
		pDC->Ellipse(x, y, x + heartSize, y + heartSize);

		pDC->SelectObject(pOldBrush);
	}
}

void CChildView::DrawStartScreen(CDC* pDC, const CRect& rect)
{
	// 메인 타이틀 폰트 설정
	LOGFONT titleFontDef = { 0 };
	titleFontDef.lfHeight = 60;
	_tcscpy_s(titleFontDef.lfFaceName, _T("맑은 고딕"));

	CFont titleFont;
	titleFont.CreateFontIndirect(&titleFontDef);
	CFont* pOldFont = pDC->SelectObject(&titleFont);

	CString title = _T("게임 시작");

	// 제목 크기 계산
	CRect titleRect(0, 0, 0, 0);
	pDC->DrawText(title, &titleRect, DT_CALCRECT);
	int titleWidth = titleRect.Width();
	int titleHeight = titleRect.Height();

	// 박스 크기 및 위치 계산
	int boxWidth = max(titleWidth, 500);
	int boxHeight = titleHeight + 250;
	int x = (rect.Width() - boxWidth) / 2;
	int y = (rect.Height() - boxHeight) / 2;
	CRect boxRect(x, y, x + boxWidth, y + boxHeight);

	// 박스 배경 및 테두리 그리기
	CBrush boxBrush(RGB(230, 230, 230));
	CPen boxPen(PS_SOLID, 2, RGB(100, 100, 100));
	CBrush* pOldBrush = pDC->SelectObject(&boxBrush);
	CPen* pOldPen = pDC->SelectObject(&boxPen);

	pDC->RoundRect(&boxRect, CPoint(20, 20));

	// 원래 상태 복구
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	// 제목 출력
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(x + (boxWidth - titleWidth) / 2, y + 20, title);

	// 방향키 버튼 위치 계산
	int keySize = 70;
	int keyCenterX = x + boxWidth / 2;
	int keyTopY = y + titleHeight + 60;

	CRect keys[4];
	keys[0] = CRect(keyCenterX - keySize / 2, keyTopY, keyCenterX + keySize / 2, keyTopY + keySize); // ↑
	keys[1] = CRect(keyCenterX - keySize / 2, keyTopY + keySize + 10, keyCenterX + keySize / 2, keyTopY + keySize * 2 + 10); // ↓
	keys[2] = CRect(keyCenterX - keySize * 1.5 - 10, keyTopY + keySize + 10, keyCenterX - keySize / 2 - 10, keyTopY + keySize * 2 + 10); // ←
	keys[3] = CRect(keyCenterX + keySize / 2 + 10, keyTopY + keySize + 10, keyCenterX + keySize * 1.5 + 10, keyTopY + keySize * 2 + 10); // →

	CString arrows[4] = { _T("▲"), _T("▽"), _T("◀"), _T("▶") };

	// 방향키 폰트 설정
	LOGFONT keyFontDef = { 0 };
	keyFontDef.lfHeight = 40;
	_tcscpy_s(keyFontDef.lfFaceName, _T("맑은 고딕"));

	CFont keyFont;
	keyFont.CreateFontIndirect(&keyFontDef);
	CFont* pOldKeyFont = pDC->SelectObject(&keyFont);

	// 방향키 박스 및 텍스트 출력
	for (int i = 0; i < 4; ++i)
	{
		COLORREF brushColor = RGB(255, 255, 255); // 기본 흰색
		COLORREF textColor = RGB(0, 0, 0);        // 기본 검정

		switch (i) {
		case 0: // ↑ 시작 키
			brushColor = RGB(60, 60, 60);         // 진한 회색
			textColor = RGB(255, 255, 255);       // 흰 글씨
			break;
		case 1: // ↓ 중립
			break;
		case 2: // ← 조작
		case 3: // → 조작
			brushColor = RGB(255, 240, 150);      // 연한 노랑
			textColor = RGB(0, 0, 0);
			break;
		}

		// 색상 적용
		CBrush keyBrush(brushColor);
		CBrush* pOldKeyBrush = pDC->SelectObject(&keyBrush);
		pDC->RoundRect(&keys[i], CPoint(20, 20));
		pDC->SelectObject(pOldKeyBrush);

		// 텍스트 색상 적용
		pDC->SetTextColor(textColor);
		CSize textSize = pDC->GetTextExtent(arrows[i]);
		int textX = keys[i].left + (keySize - textSize.cx) / 2;
		int textY = keys[i].top + (keySize - textSize.cy) / 2;
		pDC->TextOutW(textX, textY, arrows[i]);

	}

	// 폰트 원상복구
	pDC->SelectObject(pOldKeyFont);
	pDC->SelectObject(pOldFont);
}

void CChildView::DrawGameResultMessage(CDC* pDC, const CRect& rect)
{
	if (m_gameState != GAME_WIN && m_gameState != GAME_LOSE)
		return;

	CString message = (m_gameState == GAME_WIN) ? _T("🎉 레벨 클리어!") : _T("😢 Game Over...");

	LOGFONT logFont = { 0 };
	logFont.lfHeight = 48;
	_tcscpy_s(logFont.lfFaceName, _T("맑은 고딕"));
	logFont.lfWeight = FW_BOLD;
	CFont font;
	font.CreateFontIndirect(&logFont);
	CFont* pOldFont = pDC->SelectObject(&font);

	COLORREF color = (m_gameState == GAME_WIN)
		? RGB(0, 200, 100)
		: RGB(250, 0, 0);

	pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);

	CSize textSize = pDC->GetTextExtent(message);
	int x = rect.left + (rect.Width() - textSize.cx) / 2;
	int y = rect.top + (rect.Height() - textSize.cy) / 2;

	pDC->TextOutW(x, y, message);

	pDC->SelectObject(pOldFont);

	if (m_bShowContinueMsg)
	{
		CString msg = (m_gameState == GAME_WIN) ? _T("다음 레벨을 시작하려면 아무 키나 누르세요...") : _T("다시 시작하려면 아무 키나 누르세요...");
		pDC->SetTextColor(RGB(0, 0, 0));
		CSize msgSize = pDC->GetTextExtent(msg);
		int msgX = rect.left + (rect.Width() - msgSize.cx) / 2;
		int msgY = y + textSize.cy + 30;
		pDC->TextOutW(msgX, msgY, msg);
	}
}

void CChildView::DrawItemMessage(CDC* pDC, const CRect& rect)
{
	for (const auto& message : m_gameManager.effectMessages) {
		CFont font;
		font.CreatePointFont(240, _T("맑은 고딕"));
		CFont* pOldFont = pDC->SelectObject(&font);

		pDC->SetBkMode(TRANSPARENT);

		CSize textSize = pDC->GetTextExtent(message.text);
		int x = message.position.x - textSize.cx / 2;
		int y = message.position.y - textSize.cy / 2;

		// 아이템 타입에 따라 색상 지정
		COLORREF textColor;
		switch (message.type) {
		case ItemType::PaddleEnlarge:
			textColor = RGB(0, 128, 255); // 파랑
			break;
		case ItemType::Heal:
			textColor = RGB(0, 200, 0);   // 초록
			break;
		case ItemType::BallIncrease:
			textColor = RGB(255, 64, 64); // 빨강
			break;
		default:
			textColor = RGB(0, 0, 0); // 기본
			break;
		}

		// 그림자 먼저 그리기
		pDC->SetTextColor(RGB(50, 50, 50));
		pDC->TextOutW(x + 2, y + 2, message.text);

		// 실제 텍스트
		pDC->SetTextColor(textColor);
		pDC->TextOutW(x, y, message.text);

		pDC->SelectObject(pOldFont);
	}
}
