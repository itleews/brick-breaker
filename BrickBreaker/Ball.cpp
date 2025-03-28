#include "pch.h"
#include "Ball.h"

void Ball::Draw(CDC* pDC) {
    // �귯�� ���� �� ����
    CBrush brush(m_ballColor);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // �� �׸���
    pDC->Ellipse(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);

    // ���� �귯�� ����
    pDC->SelectObject(pOldBrush);
}


bool Ball::Update(CRect boundary, CWnd* pWnd) {
    // ���� ���� ��ġ�� ����
    CRect oldRect(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);

    // ��ġ �̵�
    m_x += m_dx;
    m_y += m_dy;

    // ��� üũ �� �̵� �ݿ�
    if (m_x + m_radius + m_dx >= boundary.right) {
        m_dx = -m_dx;
        m_x = boundary.right - m_radius;
    }
    else if (m_x - m_radius + m_dx <= boundary.left) {
        m_dx = -m_dx;
        m_x = boundary.left + m_radius;
    }
    if (m_y + m_radius >= boundary.bottom) {
        return FALSE;
    }
    else if (m_y - m_radius + m_dy <= boundary.top) {
        m_dy = -m_dy;
        m_y = boundary.top + m_radius;
    }

    // ���ο� ��ġ�� ���
    CRect newRect(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);

    // ���� ��ġ�� ���ο� ��ġ�� �����ϴ� �������� ��ħ
    oldRect.UnionRect(&oldRect, &newRect);

    // pWnd�� nullptr���� Ȯ���� �� ��ȿȭ
    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);  // CWnd�� ���� ��ȿȭ ��û
    }
}