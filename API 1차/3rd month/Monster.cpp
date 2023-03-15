#include "pch.h"
#include "Monster.h"
#include "Bullet.h"
#include "MainGame.h"
#include "AbstractFactory.h"

CMonster::CMonster() :m_dwInvincibleTime(GetTickCount64()), m_fAngle(0), m_iPattern(0),m_dwBulletCreTime(GetTickCount64()) ,m_pBulletList(nullptr)
{

}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Init()
{
	if (CMainGame::iStageNum == STAGE_ONE)
	{
		m_tInfo = { ((rand() % 541) + 180.f) , 20, 60.f, 60.f };
		m_fSpeed = 3.f;
		m_eFigure = FIGURETYPE::FT_CIRCLE;
		m_bInvincible = false;
		m_tVel = { 0.f, m_fSpeed };
	}
	else if(CMainGame::iStageNum == STAGE_TWO)
	{
		m_tInfo = { WINCX / 2.f, 200, 60.f, 60.f };
		m_fSpeed = 5.f;
		m_eFigure = FIGURETYPE::FT_CIRCLE;
		m_bInvincible = true;
		m_tVel = { m_fSpeed,0.f };
	}
	else if (CMainGame::iStageNum == STAGE_THREE)
	{
		m_tInfo.fX = WINCX / 2.f;
		m_tInfo.fY = WINCY / 3.f;
		m_tInfo.fCX = 100.f;
		m_tInfo.fCY = 100.f;
		m_fSpeed = 3.f;
		m_fAngle = 0;
		m_tStat.m_fMaxHp = 100;
		m_tStat.m_fHp = m_tStat.m_fMaxHp;
		m_tStat.m_fAttack = 10;
	}
}

int CMonster::Update()
{
	// 생성 후 1초동안 무적
	if (m_dwInvincibleTime + 1000 < GetTickCount64())
	{
		m_bInvincible = false;

		m_dwInvincibleTime = GetTickCount64();
	}

	if (m_dwBulletCreTime + (rand() % 2000) + 500 < GetTickCount64())
	{
		Random_Shooting();
	}

	if (CMainGame::iStageNum == STAGE_TWO)
	{
		
	}
	else
	{
		if (PLAYZONEBOTTOM + m_tInfo.fCY * 0.5f <= m_tInfo.fY || PLAYZONELEFT - 50 >= m_tInfo.fX - m_tInfo.fCX * 0.5f || PLAYZONERIGHT <= m_tInfo.fX - m_tInfo.fCX * 0.5f)
			m_bDead = true;
	}

	if (Is_Dead())
		return OBJ_DEAD;

	Move(m_iPattern);
	__super::Update_Rect();
	return OBJ_NOEVENT;;
}

void CMonster::Late_Update()
{
	
}

void CMonster::Render(HDC hDC)
{
	// 무적이면 빨간색으로 render
	if(m_bInvincible == false)
		Draw_Figure(hDC);
	else
	{
		HPEN hpen;
		HPEN hpenOld;
		hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);
		Draw_Figure(hDC);
		hpen = (HPEN)::SelectObject(hDC, hpenOld);
		::DeleteObject(hpen);
	}
}

void CMonster::Release()
{

}

void CMonster::Move(int _iPattern)
{
	if (STRAIGHT == _iPattern)
	{
		m_tInfo.fX += m_tVel.vX;
		m_tInfo.fY += m_tVel.vY;
	}
	else if (LEFT_ANGLE == _iPattern)
	{
		if (300 >= m_tInfo.fY)
			m_tInfo.fY += m_tVel.vY;
		else if (300 < m_tInfo.fY)
		{
			m_tVel.vX = m_fSpeed;
			m_tVel.vY = 0.f;
			m_tInfo.fX -= m_tVel.vX;
		}
	}
	else if (RIGHT_ANGLE == _iPattern)
	{
		if (300 >= m_tInfo.fY)
			m_tInfo.fY += m_tVel.vY;
		else if (300 < m_tInfo.fY)
		{

			m_tVel.vX = m_fSpeed;
			m_tVel.vY = 0.f;
			m_tInfo.fX += m_tVel.vX;
		}
	}
	else if (LEFT_CIRCLE == _iPattern)
	{
		if (150 >= m_tInfo.fY && 0 == m_fAngle)
			m_tInfo.fY += m_tVel.vY;
		else if (150 < m_tInfo.fY)
		{
			--m_fAngle;
			float fRadian = m_fAngle * (PI / 180.f);

			m_tInfo.fX -= 1.1f * (float)cos(fRadian);
			m_tInfo.fY -= 1.1f * (float)sin(fRadian);
		}
	}
	else if (RIGHT_CIRCLE == _iPattern)
	{
		if (150 >= m_tInfo.fY && 0 == m_fAngle)
			m_tInfo.fY += m_tVel.vY;
		else if (150 < m_tInfo.fY)
		{
			++m_fAngle;
			float fRadian = m_fAngle * (PI / 180.f);

			m_tInfo.fX += 1.1f * (float)cos(fRadian);
			m_tInfo.fY += 1.1f * (float)sin(fRadian);
		}
	}
	else if (TRACE == _iPattern)
	{

	}
}

void CMonster::Random_Shooting()
{
	for (int i = 0; i < rand() % 3 + 2; ++i)
	{
		Vec2 vTemp{ (float)(rand() % 7) - 3 , (float)(rand() % 4) + 1 };
		if (vTemp.vX == 0)
			++vTemp.vX;
		m_pBulletList->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, vTemp.Get_DirVec()));
	}
	m_dwBulletCreTime = GetTickCount64();

}
