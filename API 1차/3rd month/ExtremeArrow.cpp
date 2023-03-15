#include "pch.h"
#include "ExtremeArrow.h"

CExtremeArrow::CExtremeArrow(Vec2& ArrowHead, Vec2& _Vel, FLOAT Speed) 
	: CArrow(ArrowHead,_Vel,Speed),
	m_pTargetList(nullptr),
	m_pTarget(nullptr),
	m_fDetectRange(50.f)
{
	m_fSpeed = 10.f;
}

CExtremeArrow::~CExtremeArrow()
{

}

void CExtremeArrow::Init()
{
	m_eFigure = FIGURETYPE::FT_ARROW;
	m_tInfo.fCX = 5.f;
	m_tInfo.fCY = 5.f;
	m_tStat.m_fAttack = 50.f;
}

int CExtremeArrow::Update()
{
	Find_ClosestTarget();
	if(!m_pTargetList || !m_pTarget)
		Accelerated();
	else
	{
		m_tDirection = (m_pTarget->Get_Info().Get_PosVec() - m_tInfo.Get_PosVec()).Get_DirVec();
		m_tVel = m_tDirection * m_fSpeed;
		m_tInfo += m_tVel;
	}
	m_tArrowHead.vX = m_tInfo.fX;
	m_tArrowHead.vY = m_tInfo.fY;
	Set_TailPos();

	//
	if (Is_Dead())
		return OBJ_DEAD;

	__super::Update_Rect();
	return 0;
}

void CExtremeArrow::Render(HDC hDC)
{
	Draw_Figure(hDC);
	MoveToEx(hDC, static_cast<INT>(m_tArrowHead.vX), static_cast<INT>(m_tArrowHead.vY), nullptr);
	LineTo(hDC, static_cast<INT>(m_tArrowTail.vX), static_cast<INT>(m_tArrowTail.vY));
}

void CExtremeArrow::Find_ClosestTarget()
{
	FLOAT Min = 999;
	if (m_pTargetList)
		for (auto& i : *m_pTargetList)
		{
			FLOAT distance = (i->Get_Info().Get_PosVec() - m_tInfo.Get_PosVec()).Get_Size();
			if (distance < Min)
			{
				Min = distance;
				if (Min < m_fDetectRange)
					m_pTarget = i;
				else
					m_pTarget = nullptr;
			}
		}
}
