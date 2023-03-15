#include "pch.h"
#include "Shield.h"

CShield::~CShield()
{
	Release();
}

void CShield::Init()
{
	m_fRadius = 100.f;
	m_eFigure = FIGURETYPE::FT_CIRCLE;
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;
}

int CShield::Update()
{
	if (Is_Dead())
		return OBJ_DEAD;
	Set_Radian();
	Set_PosInfo();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CShield::Late_Update()
{

}

void CShield::Render(HDC hDC)
{
	Draw_Figure(hDC);
}

void CShield::Release()
{
}

void CShield::Set_Radian()
{
	m_fAimRadian += 0.1f;
}

void CShield::Set_PosInfo()
{
	m_tDirection = { cosf(m_fAimRadian), sinf(m_fAimRadian) };
	m_tInfo.fX = m_pOwner->Get_Info().fX + m_tDirection.vX * m_fRadius;
	m_tInfo.fY = m_pOwner->Get_Info().fY + m_tDirection.vY * m_fRadius;
}
