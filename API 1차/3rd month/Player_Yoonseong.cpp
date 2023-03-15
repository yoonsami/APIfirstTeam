#include "pch.h"
#include "Player_Yoonseong.h"
#include "Bow.h"
#include "KeyBoardMgr.h"
#include "ExtremeArrow.h"

CPlayer_Yoonseong::CPlayer_Yoonseong()
	: m_pBow(nullptr),
	m_fBowPower(0.f),
	m_tArrowTail{},
	m_tArrowHead{},
	m_fArrowSize(ARROWSIZE)
{

}

CPlayer_Yoonseong::~CPlayer_Yoonseong()
{
	Release();
}

void CPlayer_Yoonseong::Init()
{
	m_tStat = { 10.f,100.f,100.f };
	m_tInfo = { WINCX / 2.f, WINCY / 2.f, 50.f, 50.f };
	m_fSpeed = 10.f;
	// 모양 설정
	m_eFigure = FIGURETYPE::FT_RECT;
	m_pBow = new CBow(this);
	m_pBow->Init();
}

int CPlayer_Yoonseong::Update()
{
	if (Is_Dead())
	{
		m_tStat.m_fHp = m_tStat.m_fMaxHp;
	}
	Key_Input();

	Accelerated();
	Set_AimPos();
	__super::Update_Rect();

	if (m_pBow)
		m_pBow->Update();
	return OBJ_NOEVENT;
}

void CPlayer_Yoonseong::Late_Update()
{

}

void CPlayer_Yoonseong::Render(HDC hDC)
{
	Draw_Figure(hDC);
	Draw_Arrow(hDC, 100.f);
	if (m_pBow)
		m_pBow->Render(hDC);
}

void CPlayer_Yoonseong::Release()
{
	Safe_Delete(m_pBow);
}

void CPlayer_Yoonseong::Key_Input()
{
	if ((GetAsyncKeyState('D') == (SHORT)0x8000) && !CKeyBoardMgr::m_bLeftPressed)
	{
		if (!CKeyBoardMgr::m_bRightPressed)
		{
			m_tVel.vX += m_fSpeed;
			CKeyBoardMgr::m_bRightPressed = true;
		}
		else if (CKeyBoardMgr::m_bRightPressed && m_tVel.vX < 0)
		{
			m_tVel.vX += m_fSpeed;
			CKeyBoardMgr::m_bRightPressed = false;
		}
	}
	if ((GetAsyncKeyState('D') == (SHORT)0x0000) && CKeyBoardMgr::m_bRightPressed)
	{
		if (m_tInfo.fX == PLAYZONERIGHT - (m_tInfo.fCX * 0.5f));
		else
		{
			if (m_tVel.vX > 0) m_tVel.vX -= m_fSpeed;
			else if (m_tVel.vX < 0) m_tVel.vX += m_fSpeed;
		}
		CKeyBoardMgr::m_bRightPressed = false;
	}

	if ((GetAsyncKeyState('A') == (SHORT)0x8000) && !CKeyBoardMgr::m_bRightPressed)
	{
		if (!CKeyBoardMgr::m_bLeftPressed)
		{
			m_tVel.vX -= m_fSpeed;
			CKeyBoardMgr::m_bLeftPressed = true;
		}
		else if (CKeyBoardMgr::m_bLeftPressed && m_tVel.vX > 0)
		{
			m_tVel.vX -= m_fSpeed;
			CKeyBoardMgr::m_bLeftPressed = false;
		}
	}
	if ((GetAsyncKeyState('A') == (SHORT)0x0000) && CKeyBoardMgr::m_bLeftPressed)
	{
		if (m_tInfo.fX == PLAYZONELEFT + (m_tInfo.fCX * 0.5f));
		else
		{
			if (m_tVel.vX > 0) m_tVel.vX -= m_fSpeed;
			else if (m_tVel.vX < 0) m_tVel.vX += m_fSpeed;
		}
		CKeyBoardMgr::m_bLeftPressed = false;
	}

	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && m_bOnGround)
	{
		m_tVel.vY -= 15.f;
		CKeyBoardMgr::m_bSpacePressed = true;
	}
	if ((GetAsyncKeyState(VK_SPACE) == (SHORT)0x0000) && CKeyBoardMgr::m_bSpacePressed)
	{
		CKeyBoardMgr::m_bSpacePressed = false;
	}
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && !CKeyBoardMgr::m_bRButtonPressed)
	{
		/*m_pBulletList->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, m_tDirection));*/
		m_fBowPower += 1.f;
		m_fBowPower = clamp(m_fBowPower, MINBOWTENSION, MAXBOWTENSION);
		CKeyBoardMgr::m_bLButtonPressed = true;
	}

	if ((GetAsyncKeyState(VK_LBUTTON) == (SHORT)0x0000) && CKeyBoardMgr::m_bLButtonPressed)
	{
		FLOAT fArrowSpeed = 5.f + MAXARRPOW * (m_fBowPower / 90.f);
		m_pBulletList->push_back(new CArrow(m_tArrowHead, m_tDirection, fArrowSpeed));
		m_pBulletList->back()->Init();
		m_fBowPower = 0.f;
		CKeyBoardMgr::m_bLButtonPressed = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && !CKeyBoardMgr::m_bLButtonPressed)
	{
		/*m_pBulletList->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, m_tDirection));*/
		m_fBowPower += 1.f;
		m_fBowPower = clamp(m_fBowPower, MINBOWTENSION, MAXBOWTENSION);
		CKeyBoardMgr::m_bRButtonPressed = true;
	}

	if ((GetAsyncKeyState(VK_RBUTTON) == (SHORT)0x0000) && CKeyBoardMgr::m_bRButtonPressed)
	{
		FLOAT fArrowSpeed = 5.f + MAXARRPOW * (m_fBowPower / 90.f);
		m_pGuidedBulletList->push_back(new CExtremeArrow(m_tArrowHead, m_tDirection, fArrowSpeed));
		m_pGuidedBulletList->back()->Init();
		m_fBowPower = 0.f;
		CKeyBoardMgr::m_bRButtonPressed = false;
	}
}

void CPlayer_Yoonseong::Set_AimPos()
{
	
	// 포신 마우스좌표 방향으로
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_fAimRadian = atan2f(ptMouse.y - m_tInfo.fY, ptMouse.x - m_tInfo.fX);


	m_tDirection.vX = cosf(m_fAimRadian);
	m_tDirection.vY = sinf(m_fAimRadian);

	if (m_pBow)
	{
		m_tArrowTail = { m_tInfo.fX + BOWRADIUS * cosf(BOWRADIAN) * m_tDirection.vX - m_fBowPower * 0.6f * m_tDirection.vX,
				m_tInfo.fY + BOWRADIUS * cosf(BOWRADIAN) * m_tDirection.vY - m_fBowPower * 0.6f * m_tDirection.vY };
		m_tArrowHead = { m_tArrowTail.vX + m_fArrowSize * m_tDirection.vX,
			m_tArrowTail.vY + m_fArrowSize * m_tDirection.vY };
	}
}

void CPlayer_Yoonseong::Draw_Arrow(HDC hDC, FLOAT _size)
{
	if (CKeyBoardMgr::m_bLButtonPressed || CKeyBoardMgr::m_bRButtonPressed)
	{
		MoveToEx(hDC, static_cast<INT>(m_tArrowTail.vX), static_cast<INT>(m_tArrowTail.vY), nullptr);
		LineTo(hDC, static_cast<INT>(m_tArrowHead.vX), static_cast<INT>(m_tArrowHead.vY));
	}
}
