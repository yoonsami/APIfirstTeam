#include "pch.h"
#include "Monster.h"
#include "Bullet.h"
#include "MainGame.h"
#include "AbstractFactory.h"
#include "Shield.h"

CMonster::CMonster() 
	: CObject(OT_MONSTER),
	m_dwInvincibleTime(GetTickCount64()), 
	m_dwBulletCreTime(GetTickCount64()),
	m_pBulletList(nullptr),
	m_fRadius(0.f),
	m_tTarget{},
	m_iPattern(STRAIGHT)
{

}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Init()
{
	m_fSpeed = 3.f;
	m_eFigure = FIGURETYPE::FT_CIRCLE;
	m_bInvincible = true;
	m_tVel = { 0.f, m_fSpeed };
	m_tStat.m_fMaxHp = 100;
	m_tStat.m_fHp = m_tStat.m_fMaxHp;
	m_tStat.m_fAttack = 10;
	if (CMainGame::iStageNum == STAGE_ONE)
	{
		m_tInfo = { ((rand() % 541) + 180.f) , 20, 60.f, 60.f };
		m_iPattern = rand() % PATTERN_END;
	}
	else if (CMainGame::iStageNum == STAGE_TWO)
	{
		m_tInfo = { WINCX *0.5f, WINCY / 3.f , 100.f, 100.f };
		m_tVel = { m_fSpeed, 0.f };
	}
	else if (CMainGame::iStageNum == STAGE_THREE)
	{
		m_tInfo = { ((rand() % 541) + 180.f) , 200, 60.f, 60.f };
		m_tVel = { 2.f, 0.f };
	}
}

int CMonster::Update()
{
	// 생성 후 0.3초동안 무적
	if (m_dwInvincibleTime + 300 < GetTickCount64())
	{
		m_bInvincible = false;

		m_dwInvincibleTime = GetTickCount64();
	}

	if (Is_Dead())
		return OBJ_DEAD;

	if (m_iPattern == TRACE)
	{
		m_fAimRadian = atan2f(m_tTarget.fY - m_tInfo.fY, m_tTarget.fX- m_tInfo.fX);

		m_tVel.vX = cosf(m_fAimRadian);
		m_tVel.vY = sinf(m_fAimRadian);
	}

	Move();
	Acting();



	__super::Update_Rect();
	return OBJ_NOEVENT;;
}

void CMonster::Late_Update()
{
	if (PLAYZONEBOTTOM + m_tInfo.fCY * 0.5f <= m_tInfo.fY || PLAYZONELEFT - 50 >= m_tInfo.fX - m_tInfo.fCX * 0.5f || PLAYZONERIGHT <= m_tInfo.fX - m_tInfo.fCX * 0.5f)
		m_tStat.m_fHp = 0;
}

void CMonster::Render(HDC hDC)
{
	// 무적이면 빨간색으로 render
	for (auto& i : m_tHitArrow)
	{
		MoveToEx(hDC, static_cast<INT>(i.Head.vX), static_cast<INT>(i.Head.vY), nullptr);
		LineTo(hDC, static_cast<INT>(i.Tail.vX), static_cast<INT>(i.Tail.vY));
	}
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

void CMonster::Accelerated()
{
	if (CMainGame::iStageNum == STAGE_THREE)
		CMainGame::Gravity = GRAVITY;
	else
	{
		CMainGame::Gravity = 0.f;
	}
	// 알짜힘 계산
	{
		// 중력
		m_tAcc = { 0.f, CMainGame::Gravity };

		// 지상에서 반작용
		if (Is_OnGround())
			m_tAcc += {0.f, -CMainGame::Gravity };
	}

	// 가속도/ 속도/ 위치 변화
	{
		m_tVel += m_tAcc * CMainGame::GameSpeed;
		m_tInfo += m_tVel * CMainGame::GameSpeed;
		Update_Hit_ArrowPos(1.f);
	}

	// 충돌, 착지 후 속도, 위치 보정
	{
		if (m_tInfo.fY + m_tInfo.fCY * 0.5f > PLAYZONEBOTTOM) // 아래 넘어갔을 때
		{
			// y방향 변화 전으로 돌림
			m_tVel.vY += -m_tAcc.vY * CMainGame::GameSpeed;
			m_tInfo.fY += -m_tVel.vY * CMainGame::GameSpeed;
			for (auto& i : m_tHitArrow)
			{
				i.Head.vY += -m_tVel.vY * CMainGame::GameSpeed;
				i.Tail.vY += -m_tVel.vY * CMainGame::GameSpeed;
			}
			FLOAT a = m_tInfo.fY;
			// 충돌 직전 속도 비율
			FLOAT Ratio = CMainGame::quad_eqn(m_tAcc.vY * CMainGame::GameSpeed * CMainGame::GameSpeed, m_tVel.vY * CMainGame::GameSpeed, m_tInfo.fY - PLAYZONEBOTTOM + m_tInfo.fCY * 0.5f);

			m_tVel.vY += m_tAcc.vY * Ratio * CMainGame::GameSpeed;
			m_tInfo.fY = PLAYZONEBOTTOM - m_tInfo.fCY * 0.5f;

			for (auto& i : m_tHitArrow)
			{
				i.Head.vY += m_tInfo.fY - a;
				i.Tail.vY += m_tInfo.fY - a;
			}
			// 반발계수 적용
			m_tVel.vY *= -1.f;
			// 충돌 후 Y좌표 보정
			m_tVel.vY -= m_tAcc.vY * (1 - Ratio) * CMainGame::GameSpeed;
			m_tInfo.fY += m_tVel.vY * (1 - Ratio) * CMainGame::GameSpeed;
			for (auto& i : m_tHitArrow)
			{
				i.Head.vY += m_tVel.vY * (1 - Ratio) * CMainGame::GameSpeed;
				i.Tail.vY += m_tVel.vY * (1 - Ratio) * CMainGame::GameSpeed;
			}
		}

		else if (m_tInfo.fY - m_tInfo.fCY * 0.5f < PLAYZONETOP) // 위로 넘어갔을 때
		{

			m_tInfo.fY = PLAYZONETOP + m_tInfo.fCY * 0.5f;

			m_tVel.vY *= -1.f;

		}

		if (m_tInfo.fX + m_tInfo.fCX * 0.5f > PLAYZONERIGHT) // 오른쪽 넘어갔을 때
		{
			// 위치 보정 
			m_tInfo.fX = PLAYZONERIGHT - m_tInfo.fCX * 0.5f;
			//반발계수 적용
			m_tVel.vX *= -1.f;
		}

		if (m_tInfo.fX - m_tInfo.fCX * 0.5f < PLAYZONELEFT) // 왼쪽 넘어갔을 때
		{
			// 위치 보정 
			m_tInfo.fX = PLAYZONELEFT + m_tInfo.fCX * 0.5f;
			//반발계수 적용
			m_tVel.vX *= -1.f;
		}
	}

	if (m_tVel.vY == 0 && m_tInfo.fY == PLAYZONEBOTTOM - (m_tInfo.fCY * 0.5f))
		m_bOnGround = true;
	else	m_bOnGround = false;
}

void CMonster::Update_Hit_ArrowPos(FLOAT A)
{
	for (auto& i : m_tHitArrow)
	{
		i.Head.vX += m_tVel.vX * CMainGame::GameSpeed * A;
		i.Head.vY += m_tVel.vY * CMainGame::GameSpeed * A;
		i.Tail.vX += m_tVel.vX * CMainGame::GameSpeed * A;
		i.Tail.vY += m_tVel.vY * CMainGame::GameSpeed * A;
	}
}

void CMonster::Move()
{
	if(CMainGame::iStageNum == STAGE_ONE)
	{
		if (STRAIGHT == m_iPattern)
		{
			m_tInfo.fX += m_tVel.vX;
			m_tInfo.fY += m_tVel.vY;
		}
		else if (LEFT_ANGLE == m_iPattern)
		{
			if (300 >= m_tInfo.fY)
				m_tInfo.fY += m_tVel.vY;
			else if (300 < m_tInfo.fY)
			{
				m_tVel.vX = -m_fSpeed;
				m_tVel.vY = 0.f;
				m_tInfo.fX += m_tVel.vX;
			}
		}
		else if (RIGHT_ANGLE == m_iPattern)
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
		else if (LEFT_CIRCLE == m_iPattern)
		{
			if (150 >= m_tInfo.fY && 0 == m_fAimRadian)
				m_tInfo.fY += m_tVel.vY;
			else if (150 < m_tInfo.fY)
			{
				--m_fAimRadian;
				float fRadian = m_fAimRadian * (PI / 180.f);
				m_tVel.vX = -1.1f * (float)cos(fRadian);
				m_tVel.vY = -1.1f * (float)sin(fRadian);
				m_tInfo.fX += m_tVel.vX;
				m_tInfo.fY += m_tVel.vY;
			}
		}
		else if (RIGHT_CIRCLE == m_iPattern)
		{
			if (150 >= m_tInfo.fY && 0 == m_fAimRadian)
				m_tInfo.fY += m_tVel.vY;
			else if (150 < m_tInfo.fY)
			{
				++m_fAimRadian;
				float fRadian = m_fAimRadian * (PI / 180.f);
				m_tVel.vX = 1.1f * (float)cos(fRadian);
				m_tVel.vY = 1.1f * (float)sin(fRadian);

				m_tInfo.fX += m_tVel.vX;
				m_tInfo.fY += m_tVel.vY;
			}
		}
		else if (TRACE == m_iPattern)
		{
			m_tInfo.fX += m_tVel.vX;
			m_tInfo.fY += m_tVel.vY;
		}
	}
	else if (CMainGame::iStageNum == STAGE_TWO)
	{
		m_tInfo += m_tVel * m_fSpeed;
	}
	else if(CMainGame::iStageNum == STAGE_THREE)
		Accelerated();
}

void CMonster::Acting()
{
	if (CMainGame::iStageNum == STAGE_ONE)
	{
		
			Random_Shooting();
	}
	else if (CMainGame::iStageNum == STAGE_TWO)
	{
		switch (m_iMonsterType)
		{
		case MON_MONSTER:
			if (m_dwBulletCreTime + 1000 < GetTickCount64())
			{
				m_pBulletList->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, Vec2{ 0.f, 1.f }));
				m_dwBulletCreTime = GetTickCount64();
			}
			break;
		case MON_RANDOMBULLET:
			if (m_dwBulletCreTime + (rand() % 2000) + 500 < GetTickCount64())
			{
				Random_Shooting();
			}
			break;
		case MON_SHEILD:
			if (m_dwBulletCreTime + 1000 < GetTickCount64())
			{
				Create_Shield();
				m_dwBulletCreTime = GetTickCount64();
			}
			break;
		case MON_SCREWBULLET:
			break;
		case MON_BOSS:
			break;
		case MON_END:
			break;
		default:
			break;
		}
	}
	else if (CMainGame::iStageNum == STAGE_THREE)
	{
			Random_Shooting();
	}
}

void CMonster::Random_Shooting()
{
	if (m_dwBulletCreTime + (rand() % 2000) + 2000 < GetTickCount64())
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

}


void CMonster::Create_Shield()
{
	m_pBulletList->push_back(new CShield(this));
	m_pBulletList->back()->Init();
}