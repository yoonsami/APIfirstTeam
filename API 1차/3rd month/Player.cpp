#include "pch.h"
#include "Player.h"
#include "Bullet.h"
#include "AbstractFactory.h"
#include "KeyBoardMgr.h"
#include "Shield.h"
#include "Bow.h"
#include "MainGame.h"

CPlayer::CPlayer() :m_pBulletList(nullptr), m_pShieldList(nullptr), m_dwBulletCreTime(GetTickCount64()),
m_pBow(nullptr), m_fBowPower(0.f), m_tArrowTail{}, m_tArrowHead{},m_fArrowSize(ARROWSIZE)
{

}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Init()
{
	m_tStat = { 10.f,100.f,100.f };

	if (CMainGame::iStageNum == STAGE_ONE)
	{
		m_tInfo = { WINCX / 2.f, 400, 40.f, 40.f };
		m_fSpeed = 10.f;
		// 모양 설정
		m_eFigure = FIGURETYPE::FT_RECT;
	}
	else if (CMainGame::iStageNum == STAGE_TWO)
	{
		m_tInfo = { WINCX / 2.f, WINCY / 2.f, 50.f, 50.f };
		m_fSpeed = 10.f;
		// 모양 설정
		m_eFigure = FIGURETYPE::FT_RECT;
		m_pBow = new CBow(this);
		m_pBow->Init();
	}
	
}

int CPlayer::Update()
{
	if (Is_Dead())
	{
		m_tStat.m_fHp = m_tStat.m_fMaxHp;
	}

	Key_Input();
	Set_AimPos();
	Accelerated();
	__super::Update_Rect();
	
	if(m_pBow)
		m_pBow->Update();
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{

}

void CPlayer::Render(HDC hDC)
{
	if (CMainGame::iStageNum == STAGE_ONE)
	{
		Draw_Figure(hDC);
	}
	else if(CMainGame::iStageNum == STAGE_TWO)
	{
		Draw_Figure(hDC);
		Draw_Arrow(hDC, 100.f);
		m_pBow->Render(hDC);
	}
}

void CPlayer::Release()
{
	Safe_Delete(m_pBow);
}

void CPlayer::Set_AimPos()
{
	if(CMainGame::iStageNum == STAGE_TWO)
	{
		// 포신 마우스좌표 방향으로
		POINT ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		m_fAimRadian = atan2f(ptMouse.y - m_tInfo.fY, ptMouse.x - m_tInfo.fX);
	}

	m_tDirection.vX = cosf(m_fAimRadian);
	m_tDirection.vY = sinf(m_fAimRadian);

	if(m_pBow)
	{
		m_tArrowTail = { m_tInfo.fX + BOWRADIUS * cosf(BOWRADIAN) * m_tDirection.vX - m_fBowPower * m_tDirection.vX,
				m_tInfo.fY + BOWRADIUS * cosf(BOWRADIAN) * m_tDirection.vY - m_fBowPower * m_tDirection.vY };
		m_tArrowHead = { m_tArrowTail.vX + m_fArrowSize * m_tDirection.vX,
			m_tArrowTail.vY + m_fArrowSize * m_tDirection.vY };
	}
}

void CPlayer::Draw_Arrow(HDC hDC, FLOAT _size)
{
	if(CKeyBoardMgr::m_bLButtonPressed)
	{
		MoveToEx(hDC, static_cast<INT>(m_tArrowTail.vX), static_cast<INT>(m_tArrowTail.vY), nullptr);
		LineTo(hDC, static_cast<INT>(m_tArrowHead.vX), static_cast<INT>(m_tArrowHead.vY));
	}
}

void CPlayer::Key_Input()
{	
	if (CMainGame::iStageNum == STAGE_ONE)
	{
		if (GetAsyncKeyState('W'))
			m_tVel.vY = -m_fSpeed;

		if (GetAsyncKeyState('S'))
			m_tVel.vY = m_fSpeed;

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

		if (GetAsyncKeyState(VK_SPACE) && m_dwBulletCreTime + 200 < GetTickCount64())
		{
			m_pBulletList->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, Vec2{0.f,-1.f}));
			m_dwBulletCreTime = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_RBUTTON) && m_dwBulletCreTime + 200 < GetTickCount64())
		{
			Create_Shield();
			m_dwBulletCreTime = GetTickCount64();
		}
	}
	else if(CMainGame::iStageNum == STAGE_TWO)
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
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			/*m_pBulletList->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, m_tDirection));*/
			m_fBowPower += 0.1f;
			CKeyBoardMgr::m_bLButtonPressed = true;
		}

		if ((GetAsyncKeyState(VK_LBUTTON) == (SHORT)0x0000) && CKeyBoardMgr::m_bLButtonPressed)
		{
			m_fBowPower = 0.f;
			CKeyBoardMgr::m_bLButtonPressed = false;
		}
		
	}
	else if (CMainGame::iStageNum == STAGE_THREE)
	{
	if (GetAsyncKeyState('O'))
	{
		/*m_pBulletList->push_back(CAbstractFactory<CSpinBullet>::Create());
		for (list<CObject*> ::iterator iter = m_BulletList->begin();
			iter != m_pBulletList->end(); iter++)
			dynamic_cast<CBullet*>(*iter)->Set_PlayerPoint(m_tInfo.fX, m_tInfo.fY);*/
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		m_fAimRadian -= 0.1f;
		if (m_fAimRadian > 2 * PI) m_fAimRadian -= 2 * PI;
	}
	if (GetAsyncKeyState(VK_UP))
	{
		m_tVel.vX = cosf(m_fAimRadian) * m_fSpeed;
		m_tVel.vY = sinf(m_fAimRadian) * m_fSpeed;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_fAimRadian += 0.1f;
		if (m_fAimRadian < 0) m_fAimRadian += 2 * PI;
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		m_tVel.vX = cosf(m_fAimRadian) * m_fSpeed;
		m_tVel.vY = sinf(m_fAimRadian) * m_fSpeed;
	}
	if (GetAsyncKeyState(VK_SPACE))
		m_pBulletList->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, m_tDirection));
	}
}

void CPlayer::Create_Shield()
{
	m_pShieldList->push_back(new CShield(this));
	m_pShieldList->back()->Init();
}
