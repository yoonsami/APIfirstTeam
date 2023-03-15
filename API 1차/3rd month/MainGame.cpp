#include "pch.h"
#include "MainGame.h"
#include "Player.h"
#include "Monster.h"
#include "AbstractFactory.h"
#include "CollisionMgr.h"
#include "Mouse.h"

CMainGame::CMainGame() : m_dwFPSTime(GetTickCount64()), m_dwMonsterCreTime(0), m_DC(nullptr), m_iFPS(0), m_iStageNum(0)
{
	ZeroMemory(m_szFPS, sizeof(TCHAR) * 32);
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Init()
{
	srand(static_cast<UINT>(time(nullptr)));
	m_DC = GetDC(g_hWnd);

	m_objList[OT_PLAYER].push_back(CAbstractFactory<CPlayer>::Create());
	dynamic_cast<CPlayer*>(m_objList[OT_PLAYER].front())->Set_Bullet(&m_objList[OT_BULLET]);
	dynamic_cast<CPlayer*>(m_objList[OT_PLAYER].front())->Set_Shield(&m_objList[OT_SHIELD]);
// 	m_objList[OT_MONSTER].push_back(CAbstractFactory<CMonster>::Create());
	m_objList[OT_MOUSE].push_back(CAbstractFactory<CMouse>::Create());
}

void CMainGame::Update()
{
	++m_iFPS;

	if (m_dwMonsterCreTime + 1000 < GetTickCount64())
	{
		if (m_objList[OT_MONSTER].size() == 0)
		{
			//int iPattern = 3; // rand() % 4;
			size_t iSize_Max = (rand() * rand()) % 5 + 3;
			for (size_t i = 0; i < iSize_Max; ++i)
			{
				int iPattern = rand() % 5;
				m_objList[OT_MONSTER].push_back(CAbstractFactory<CMonster>::Create());
				dynamic_cast<CMonster*>(m_objList[OT_MONSTER].back())->Set_Pattern(iPattern);
			}
		}
	}
	
	for (size_t i = 0; i < OT_END; ++i)
	{
		for (auto it = m_objList[i].begin(); it != m_objList[i].end(); )
		{
			int Result = (*it)->Update();
			if (Result == OBJ_DEAD)
			{
				if(i == OT_MONSTER)
					m_dwMonsterCreTime = GetTickCount64();
				Safe_Delete<CObject*>(*it);
				it = m_objList[i].erase(it);
			}
			else ++it;
		}
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

	Rectangle(m_DC, 0, 0, WINCX, WINCY);
	
	Rectangle(m_DC, PLAYZONELEFT, PLAYZONETOP, PLAYZONERIGHT, PLAYZONEBOTTOM);
	
	for (size_t i = 0; i < OT_END; ++i)
	{
		for (auto it = m_objList[i].begin(); it != m_objList[i].end(); ++it)
		{
			(*it)->Render(m_DC);
		}
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

FLOAT CMainGame::Gravity = 1.f;

FLOAT CMainGame::GameSpeed = 1.f;

// 반발계수 : 완전비탄성 = 0, 완전탄성 1
FLOAT CMainGame::COR = 0.f;

INT CMainGame::iStageNum = STAGE_THREE;
