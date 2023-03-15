#include "pch.h"
#include "Player.h"
#include "Bullet.h"
#include "AbstractFactory.h"
#include "KeyBoardMgr.h"
#include "Shield.h"
#include "Bow.h"
#include "MainGame.h"
#include "GuidedBullet.h"
#include "RotationBullet.h"

CPlayer::CPlayer() : m_pBulletList(nullptr),
m_pShieldList(nullptr),
m_dwBulletCreTime(GetTickCount64()),
m_dwShieldCreTime(GetTickCount64()),
m_tKeyInputVec{}
{

}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Init()
{
	m_tStat = { 10.f,100.f,100.f };

	if (CMainGame::iStageNum != STAGE_THREE)
	{
		m_tInfo = { WINCX / 2.f, 400, 40.f, 40.f };
		m_fSpeed = 10.f;
		// 모양 설정
		m_eFigure = FIGURETYPE::FT_RECT;
	}
	else if (CMainGame::iStageNum == STAGE_THREE)
	{
		m_tInfo.fX = WINCX / 2.f;
		m_tInfo.fY = WINCY / 2.f;
		m_tInfo.fCX = 100.f;
		m_tInfo.fCY = 100.f;
		m_fSpeed = 10.f;
		m_fAimRadian = 0;
	}
}

int CPlayer::Update()
{
	if (Is_Dead())
	{
		m_tStat.m_fHp = m_tStat.m_fMaxHp;
	}

	Key_Input();
	Accelerated();
	__super::Update_Rect();
	
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{

}

void CPlayer::Render(HDC hDC)
{
	Draw_Figure(hDC);
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{	
	if (GetAsyncKeyState('W'))
	{
		m_tKeyInputVec.vY -= 1.f;
	}
	if (GetAsyncKeyState('S'))
	{
		m_tKeyInputVec.vY += 1.f;
	}

	if (GetAsyncKeyState('D'))
	{
		m_tKeyInputVec.vX += 1.f;
	}

	if (GetAsyncKeyState('A'))
	{

		m_tKeyInputVec.vX -= 1.f;
	}
	if (GetAsyncKeyState('Z') && m_dwBulletCreTime + 200 < GetTickCount64())
	{
		// 그냥 bullet
		m_pBulletList->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, Vec2{ 0.f,-1.f }));
		m_dwBulletCreTime = GetTickCount64();
	}
	if (GetAsyncKeyState('X') && m_dwBulletCreTime + 200 < GetTickCount64())
	{
		// 유도탄
		m_pGuidedBulletList->push_back(CAbstractFactory<CGuidedBullet>::Create(m_tInfo.fX, m_tInfo.fY, Vec2{ 0.f,-1.f }));
		m_dwBulletCreTime = GetTickCount64();
	}
	if (GetAsyncKeyState('C') && m_dwBulletCreTime + 200 < GetTickCount64())
	{
		// 회전탄
		m_pBulletList->push_back(CAbstractFactory<CRotationBullet>::Create(m_tInfo.fX, m_tInfo.fY, Vec2{ 0.f,-1.f }));
		m_dwBulletCreTime = GetTickCount64();
	}
	if (GetAsyncKeyState('V') && m_dwShieldCreTime + 200 < GetTickCount64())
	{
		Create_Shield();
		m_dwShieldCreTime = GetTickCount64();
	}

	m_tVel = m_tKeyInputVec.Get_DirVec() * m_fSpeed;
	m_tKeyInputVec = {};
}

void CPlayer::Create_Shield()
{
	m_pBulletList->push_back(new CShield(this));
	m_pBulletList->back()->Init();
}
