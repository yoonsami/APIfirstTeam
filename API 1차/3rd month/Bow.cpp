#include "pch.h"
#include "Bow.h"
#include "Player.h"

CBow::CBow(CObject* _Owner) : m_pOwner(_Owner), m_fRadius(BOWRADIUS),m_tArrowTail{}
{

}

void CBow::Init()
{
	m_tInfo.fCX = BOWRADIUS*2.f;
	m_tInfo.fCY = BOWRADIUS*2.f;
}

int CBow::Update()
{
    Set_Radian();
	Set_PosInfo();
	Update_ArrowTail();
	__super::Update_Rect();
    return 0;
}

void CBow::Late_Update()
{

}

void CBow::Render(HDC hDC)
{
	Arc(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom,
		static_cast<INT>(m_tInfo.fX + m_fRadius * cosf(m_fAimRadian + BOWRADIAN)),
		static_cast<INT>(m_tInfo.fY + m_fRadius * sinf(m_fAimRadian + BOWRADIAN)),
		static_cast<INT>(m_tInfo.fX + m_fRadius * cosf(m_fAimRadian - BOWRADIAN)),
		static_cast<INT>(m_tInfo.fY + m_fRadius * sinf(m_fAimRadian - BOWRADIAN)));
	
	MoveToEx(hDC, 
		static_cast<INT>(m_tInfo.fX + m_fRadius * cosf(m_fAimRadian + BOWRADIAN)), 
		static_cast<INT>(m_tInfo.fY + m_fRadius * sinf(m_fAimRadian + BOWRADIAN)), nullptr);
	LineTo(hDC, static_cast<INT>(m_tArrowTail.vX), static_cast<INT>(m_tArrowTail.vY));
	LineTo(hDC, 
		static_cast<INT>(m_tInfo.fX + m_fRadius * cosf(m_fAimRadian - BOWRADIAN)), 
		static_cast<INT>(m_tInfo.fY + m_fRadius * sinf(m_fAimRadian - BOWRADIAN)));



	TCHAR		szBuff1[100] = L"";
	TCHAR		szBuff2[100] = L"";

	// wsprintf : �Ҽ��� �ڸ� ������ ����� �Ұ���(WinApi���� �����ϴ� �Լ�, ��� ������ �������� ����)
	// wsprintf(szBuff, L"Bullet : %d", m_ObjList[OBJ_BULLET].size());

	// swprintf_s : visual c++���� �����ϴ� �Լ�, ��� ������ ����
	swprintf_s(szBuff1, L"Radian : %f", m_fAimRadian * 180 / PI);
	TextOut(hDC, 50, 50, szBuff1, lstrlen(szBuff1));
}

void CBow::Release()
{
}

void CBow::Set_Radian()
{
    m_fAimRadian = m_pOwner->Get_AimRadian();

}

void CBow::Set_PosInfo()
{
	m_tDirection = { cosf(m_fAimRadian), sinf(m_fAimRadian) };
	m_tInfo.fX = m_pOwner->Get_Info().fX;
	m_tInfo.fY = m_pOwner->Get_Info().fY;
}

void CBow::Update_ArrowTail()
{
	m_tArrowTail = dynamic_cast<CPlayer*>(m_pOwner)->Get_ArrowTail();
}
