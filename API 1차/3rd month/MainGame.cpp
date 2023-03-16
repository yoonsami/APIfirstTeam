#include "pch.h"
#include "MainGame.h"
#include "Player.h"
#include "Player_Yoonseong.h"
#include "Monster.h"
#include "AbstractFactory.h"
#include "CollisionMgr.h"
#include "Mouse.h"
#include "GuidedBullet.h"
#include "ExtremeArrow.h"
#include "Boss.h"
#include "Block.h"

CMainGame::CMainGame() 
	: m_dwFPSTime(GetTickCount64()),
	m_dwBLOCKCreTime(GetTickCount64()),
	m_dwMonsterCreTime(0), 
	m_DC(nullptr), 
	m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(TCHAR) * 32);
	ZeroMemory(m_szScore, sizeof(TCHAR) * 32);
	ZeroMemory(m_szResult, sizeof(TCHAR) * 32);
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Init()
{
	m_DC = GetDC(g_hWnd);
	if(iStageNum == STAGE_THREE)
		m_objList[OT_PLAYER].push_back(CAbstractFactory<CPlayer_Yoonseong>::Create());
	else
		m_objList[OT_PLAYER].push_back(CAbstractFactory<CPlayer>::Create());

	dynamic_cast<CPlayer*>(m_objList[OT_PLAYER].front())->Set_Bullet(&m_objList[OT_BULLET]);
	dynamic_cast<CPlayer*>(m_objList[OT_PLAYER].front())->Set_GuidedBullet(&m_objList[OT_GUIDEDBULLET]);
	m_objList[OT_MOUSE].push_back(CAbstractFactory<CMouse>::Create());
	if (iStageNum == STAGE_TWO)
	{
		m_objList[OT_MONSTER].push_back(CAbstractFactory<CBoss>::Create());
		dynamic_cast<CBoss*>(m_objList[OT_MONSTER].front())->Set_Bullet(&m_objList[OT_MONSTERBULLET]);
		dynamic_cast<CBoss*>(m_objList[OT_MONSTER].front())->Set_MonsterList(&m_objList[OT_MONSTER]);
		dynamic_cast<CBoss*>(m_objList[OT_MONSTER].front())->Create_Parts();
	}
	if (iStageNum == STAGE_THREE)
	{
		m_objList[OT_MONSTER].push_back(CAbstractFactory<CMonster>::Create());
		dynamic_cast<CMonster*>(m_objList[OT_MONSTER].back())->Set_Bullet(&m_objList[OT_MONSTERBULLET]);
	}
}

void CMainGame::Update()
{
	++m_iFPS;


	// 몬스터 없을 때 생성
	if (m_dwMonsterCreTime + 1000 < GetTickCount64())
	{
		if (m_objList[OT_MONSTER].size() == 0)
		{
			if(CMainGame::iStageNum!=STAGE_THREE)
			{
				size_t iSize_Max = (rand() * rand()) % 5 + 3;
				for (size_t i = 0; i < iSize_Max; ++i)
				{
					m_objList[OT_MONSTER].push_back(CAbstractFactory<CMonster>::Create());
					dynamic_cast<CMonster*>(m_objList[OT_MONSTER].back())->Set_Bullet(&m_objList[OT_MONSTERBULLET]);
				}
			}
		}
	}
	//블럭 없을 때 생성
	if (m_dwBLOCKCreTime + 1000 < GetTickCount64())
	{
		if (m_objList[OT_BLOCK].size() == 0)
		{
			m_objList[OT_BLOCK].push_back(CAbstractFactory<CBlock>::Create());
		}
	}

	for (size_t i = 0; i < OT_END; ++i)
	{
		for (auto it = m_objList[i].begin(); it != m_objList[i].end(); )
		{

			// 추적 정보 전달
			if (i == OT_MONSTER && dynamic_cast<CMonster*>(*it)->Get_Pattern() == TRACE)
			{
				static_cast<CMonster*>(*it)->Set_Target(m_objList[OT_PLAYER].front()->Get_Info());
			}
			if (i == OT_GUIDEDBULLET && iStageNum != STAGE_THREE) dynamic_cast<CGuidedBullet*>(*it)->Set_TargetList(&m_objList[OT_MONSTER]);
			else if(i == OT_GUIDEDBULLET && iStageNum == STAGE_THREE)  
				dynamic_cast<CExtremeArrow*>(*it)->Set_TargetList(&m_objList[OT_MONSTER]);

			int Result = (*it)->Update();
			
			if (Result == OBJ_DEAD)
			{
				if(i == OT_MONSTER)
				{
					iScore += 1;
					m_dwMonsterCreTime = GetTickCount64();

					// 몬스터 분열
					if (iStageNum == STAGE_THREE)
					{
						INFO tmp = (*it)->Get_Info();
						if (tmp.fCX > 35.f)
						{
							m_objList[OT_MONSTER].push_back(CAbstractFactory<CMonster>::Create
							(tmp.fX, tmp.fY, tmp.fCX, tmp.fCY, (*it)->Get_Velocity(), 0.8f, -1.f));
							dynamic_cast<CMonster*>(m_objList[OT_MONSTER].back())->Set_Bullet(&m_objList[OT_MONSTERBULLET]);
							m_objList[OT_MONSTER].push_back(CAbstractFactory<CMonster>::Create
							(tmp.fX, tmp.fY, tmp.fCX, tmp.fCY, (*it)->Get_Velocity(), 0.8f, 1.f));
							dynamic_cast<CMonster*>(m_objList[OT_MONSTER].back())->Set_Bullet(&m_objList[OT_MONSTERBULLET]);
						}
					}
				}
				Safe_Delete<CObject*>(*it);
				it = m_objList[i].erase(it);
			}
			else ++it;
		}
	}
	if (iScore >= 20)
	{
		Change_Stage();
	}
}

void CMainGame::Late_Update()
{
	for (size_t i = 0; i < OT_END; ++i)
	{
		for (auto it = m_objList[i].begin(); it != m_objList[i].end(); ++it)
		{
			(*it)->Late_Update();
		}
	}
	CCollisionMgr::CheckCollide(m_objList[OT_MONSTER], m_objList[OT_BULLET]);
	CCollisionMgr::CheckCollide(m_objList[OT_MONSTER], m_objList[OT_GUIDEDBULLET]);
	CCollisionMgr::CheckCollide(m_objList[OT_PLAYER], m_objList[OT_MONSTERBULLET]);
	CCollisionMgr::CheckCollide(m_objList[OT_BLOCK], m_objList[OT_BULLET]);
	CCollisionMgr::CheckCollide(m_objList[OT_BLOCK], m_objList[OT_GUIDEDBULLET]);
	
}

void CMainGame::Render()
{
	//FPS 표시
	if (m_dwFPSTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);
		m_iFPS = 0;
		m_dwFPSTime = GetTickCount64();
	}

	HBRUSH hpen;
	HBRUSH hpenOld;
	hpen = CreateSolidBrush(RGB(0, 0, 0));
	hpenOld = (HBRUSH)::SelectObject(m_DC, (HGDIOBJ)hpen);
	Rectangle(m_DC, 0, 0, WINCX, WINCY);
	hpen = (HBRUSH)::SelectObject(m_DC, hpenOld);
	::DeleteObject(hpen);
	
	Rectangle(m_DC, PLAYZONELEFT, PLAYZONETOP, PLAYZONERIGHT, PLAYZONEBOTTOM);
	
	for (size_t i = 0; i < OT_END; ++i)
	{
		for (auto it = m_objList[i].begin(); it != m_objList[i].end(); ++it)
		{
			(*it)->Render(m_DC);
		}
	}
	
	swprintf_s(m_szScore, L"STAGE : %d   SCORE : %d",iStageNum+1, iScore);
	TextOut(m_DC, 25, 25, m_szScore, lstrlen(m_szScore));
	
	if (iStageNum == STAGE_THREE && m_objList[OT_MONSTER].size() == 0)
	{
		swprintf_s(m_szResult, L"끗");
		TextOut(m_DC, WINCX/2, WINCY/2, m_szResult, lstrlen(m_szResult));
	}
}

void CMainGame::Release()
{
	for (size_t i = 0; i < OT_END; ++i)
	{
		for_each(m_objList[i].begin(), m_objList[i].end(), Safe_Delete<CObject*>);
	}

	ReleaseDC(g_hWnd, m_DC);
}

void CMainGame::Change_Stage()
{
	if (iStageNum == STAGE_THREE);
	else
	{
		iStageNum++;
		Release();
		for (int i = 0; i<OT_END;++i)
			m_objList[i].clear();
		Init();
		iScore = 0;
	}
}

FLOAT CMainGame::Gravity = 1.f;

FLOAT CMainGame::GameSpeed = 1.f;

// 반발계수 : 완전비탄성 = 0, 완전탄성 1
FLOAT CMainGame::COR = 0.f;

INT CMainGame::iStageNum = STAGE_ONE;

INT CMainGame::iScore = 0;
