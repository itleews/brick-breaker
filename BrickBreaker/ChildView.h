
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include <vector>
#include "GameManager.h"


// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	GameManager m_gameManager;
	CRect m_boundary;
	CString m_strTime;
	ULONGLONG m_startTick;
	int highScore = 0;
	bool m_bShowContinueMsg = true;   // 깜빡임 상태

// 작업입니다.
public:
	void DrawBackground(CDC* pDC, const CRect& rect);
	void DrawGameScene(CDC* pDC);
	void DrawStatus(CDC* pDC, const CRect& rect);
	void DrawStartScreen(CDC* pDC, const CRect& rect);
	void DrawGameResultMessage(CDC* pDC, const CRect& rect);
	void DrawItemMessage(CDC* pDC, const CRect& rect);

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 구현입니다.
public:
	virtual ~CChildView();

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

