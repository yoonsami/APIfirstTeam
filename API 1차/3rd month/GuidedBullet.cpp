#include "pch.h"
#include "GuidedBullet.h"

CGuidedBullet::CGuidedBullet() :m_pTargetList(nullptr),m_fDetectRange(150.f),m_pTarget(nullptr)
{
	m_tDetect_Rect = { static_cast<INT>(m_tInfo.fX - 150),static_cast<INT>(m_tInfo.fY - 150),static_cast<INT>(m_tInfo.fX + 150),static_cast<INT>(m_tInfo.fY + 150) };
}

CGuidedBullet::~CGuidedBullet()
{

}

void CGuidedBullet::Init()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 20.f;
	m_fSpeed = 5.f;

	m_eFigure = FIGURETYPE::FT_CIRCLE;
}

int CGuidedBullet::Update()
{
	if (Is_Dead())
		return OBJ_DEAD;
	Find_ClosestTarget();
	if(m_pTarget)
		m_tDirection = (m_pTarget->Get_Info().Get_PosVec() - m_tInfo.Get_PosVec()).Get_DirVec();

	m_tVel = m_tDirection * m_fSpeed;
	m_tInfo += m_tVel;
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGuidedBullet::Find_ClosestTarget()
{
	FLOAT Min = 999;
	if(m_pTargetList)
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