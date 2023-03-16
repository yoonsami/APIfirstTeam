#include "pch.h"
#include "Block.h"

CBlock::CBlock() :CObject(OT_BLOCK)
{
}

CBlock::~CBlock()
{
}

void CBlock::Init()
{
	m_tInfo = { ((rand() % 541) + 180.f) , 20, 70.f, 70.f };
	m_fSpeed = 2.f;
	m_eFigure = FIGURETYPE::FT_RECT;
	m_bInvincible = true;
	m_tVel = { 0.f, m_fSpeed };

	m_tStat.m_fMaxHp = 100;
	m_tStat.m_fHp = m_tStat.m_fMaxHp;

}

int CBlock::Update()
{
	if (Is_Dead())
		return OBJ_DEAD;

	m_tStat.m_fHp = m_tStat.m_fMaxHp;
	m_tInfo.fX += m_tVel.vX;
	m_tInfo.fY += m_tVel.vY;

	__super::Update_Rect();
	return OBJ_NOEVENT;;
}

void CBlock::Late_Update()
{
	if (PLAYZONEBOTTOM + m_tInfo.fCY * 0.5f <= m_tInfo.fY ||
		PLAYZONELEFT - 50 >= m_tInfo.fX - m_tInfo.fCX * 0.5f ||
		PLAYZONERIGHT <= m_tInfo.fX - m_tInfo.fCX * 0.5f)
		m_tStat.m_fHp = 0;
}

void CBlock::Render(HDC hDC)
{
	HPEN hpen;
	HPEN hpenOld;
	hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	hpenOld = (HPEN)::SelectObject(hDC, (HGDIOBJ)hpen);
	Draw_Figure(hDC);
	hpen = (HPEN)::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
}

void CBlock::Release()
{
}
