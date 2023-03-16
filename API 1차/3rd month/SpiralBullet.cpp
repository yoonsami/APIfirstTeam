#include "pch.h"
#include "SpiralBullet.h"

CSpiralBullet::CSpiralBullet() 
	: m_pCenterObj(nullptr), 
	m_tCenterPos{},
	m_fRadius(30.f),
	m_fRadian(0.f),
	m_iSide(1)
{

}

CSpiralBullet::~CSpiralBullet()
{

}

void CSpiralBullet::Init()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fSpeed = PI / 10;

	m_eFigure = FIGURETYPE::FT_CIRCLE;

}

int CSpiralBullet::Update()
{
	if (Is_Dead() || m_pCenterObj->Is_Dead())
		return OBJ_DEAD;
	m_tCenterPos.vX = m_pCenterObj->Get_Info().fX;
	m_tCenterPos.vY = m_pCenterObj->Get_Info().fY;

	m_tDirection = Vec2{ -m_pCenterObj->Get_Velocity().vY, m_pCenterObj->Get_Velocity().vX }.Get_DirVec();

	m_fRadian += m_fSpeed;
	if (m_fRadian > 2.f * PI) m_fRadian -= 2.f * PI;

	m_tInfo.fX = m_tCenterPos.vX + m_tDirection.vX * m_fRadius * sinf(m_fRadian) * m_iSide;
	m_tInfo.fY = m_tCenterPos.vY + m_tDirection.vY * m_fRadius * sinf(m_fRadian) * m_iSide;
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CSpiralBullet::Render(HDC hDC)
{
	Draw_Figure(hDC);
}

void CSpiralBullet::Late_Update()
{

}

