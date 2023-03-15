#include "pch.h"
#include "Bullet.h"

CBullet::CBullet()
{
	m_tDirection = { 0.f,-1.f };
}

CBullet::~CBullet()
{

}

void CBullet::Init()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fSpeed = 5.f;
	
	m_eFigure = FIGURETYPE::FT_CIRCLE;
}

int CBullet::Update()
{
	if (Is_Dead())
		return OBJ_DEAD;

	m_tInfo.fX += m_tVel.vX * m_fSpeed;
	m_tInfo.fY += m_tVel.vY * m_fSpeed;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBullet::Late_Update()
{
	if (m_tRect.left	<= PLAYZONELEFT		|| m_tRect.top		<= PLAYZONETOP ||
		m_tRect.right	>= PLAYZONERIGHT	|| m_tRect.bottom	>= PLAYZONEBOTTOM)
		m_tStat.m_fHp = 0;
}

void CBullet::Render(HDC hDC)
{
		Draw_Figure(hDC);
}

void CBullet::Release()
{

}

