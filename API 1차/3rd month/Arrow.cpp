#include "pch.h"
#include "Arrow.h"
#include "MainGame.h"

CArrow::CArrow(Vec2& ArrowHead, Vec2& _Vel, FLOAT Speed) :m_tArrowHead(ArrowHead), m_tArrowTail{} , m_fArrowSize(ARROWSIZE)
{
	m_tVel = _Vel.Get_DirVec() * Speed;
	m_tInfo.fX = ArrowHead.vX;
	m_tInfo.fY = ArrowHead.vY;
}

CArrow::~CArrow()
{

}

void CArrow::Init()
{
	m_eFigure = FIGURETYPE::FT_ARROW;
	m_tInfo.fCX = 5.f;
	m_tInfo.fCY= 5.f;
	m_tStat.m_fAttack = 50.f;
}

int CArrow::Update()
{
	Accelerated();
	m_tArrowHead.vX = m_tInfo.fX;
	m_tArrowHead.vY = m_tInfo.fY;
	Set_TailPos();

	//
	if (Is_Dead())
		return OBJ_DEAD;

	__super::Update_Rect();
	return 0;
}

void CArrow::Late_Update()
{

}

void CArrow::Render(HDC hDC)
{
	Draw_Figure(hDC);
	MoveToEx(hDC, static_cast<INT>(m_tArrowHead.vX), static_cast<INT>(m_tArrowHead.vY), nullptr);
	LineTo(hDC, static_cast<INT>(m_tArrowTail.vX), static_cast<INT>(m_tArrowTail.vY));
}

void CArrow::Release()
{
}

void CArrow::Accelerated()
{
	if (CMainGame::iStageNum == STAGE_THREE)
		CMainGame::Gravity = GRAVITY;

	m_tAcc = { 0.f, CMainGame::Gravity };


	// 가속도/ 속도/ 위치 변화
	{
		m_tVel += m_tAcc * CMainGame::GameSpeed;
		m_tInfo += m_tVel * CMainGame::GameSpeed;
	}

	// 충돌, 착지 후 속도, 위치 보정
	{
		if (m_tInfo.fY + m_tInfo.fCY * 0.5f > PLAYZONEBOTTOM+100) // 아래 넘어갔을 때
		{
			m_tStat.m_fHp = 0;
		}

		else if (m_tInfo.fY - m_tInfo.fCY * 0.5f < PLAYZONETOP) // 위로 넘어갔을 때
		{
			
		}

		if (m_tInfo.fX + m_tInfo.fCX * 0.5f > PLAYZONERIGHT+100) // 오른쪽 넘어갔을 때
		{
			m_tStat.m_fHp = 0;
		}

		if (m_tInfo.fX - m_tInfo.fCX * 0.5f < PLAYZONELEFT-100) // 왼쪽 넘어갔을 때
		{
			m_tStat.m_fHp = 0;
		}
	}

}

void CArrow::Set_TailPos()
{

	m_tArrowTail = m_tArrowHead - m_tVel.Get_DirVec() * ARROWSIZE;

}
