#include "pch.h"
#include "Object.h"
#include "MainGame.h"

CObject::CObject() : m_fSpeed(0.f), m_bDead(false), m_bInvincible(false), m_bOnGround(false),m_eFigure(FIGURETYPE::FT_RECT),m_tDirection{}, m_fAimRadian(0.f), m_tStat{ 10.f,10.f,10.f}
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tVel, sizeof(Vec2));
	ZeroMemory(&m_tAcc, sizeof(Vec2));
}

CObject::~CObject()
{

}

void CObject::Update_Rect(void)
{
	m_tRect.left = static_cast<LONG>(m_tInfo.fX - (0.5 + 0.5 * (m_tStat.m_fHp / m_tStat.m_fMaxHp)) * (m_tInfo.fCX * 0.5f));
	m_tRect.top = static_cast<LONG>(m_tInfo.fY - (0.5 + 0.5 * (m_tStat.m_fHp / m_tStat.m_fMaxHp)) * (m_tInfo.fCY * 0.5f));
	m_tRect.right = static_cast<LONG>(m_tInfo.fX + (0.5 + 0.5 * (m_tStat.m_fHp / m_tStat.m_fMaxHp)) * (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = static_cast<LONG>(m_tInfo.fY + (0.5 + 0.5 * (m_tStat.m_fHp / m_tStat.m_fMaxHp)) * (m_tInfo.fCY * 0.5f));
}

void CObject::Set_Pos(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

void CObject::Draw_Figure(HDC hDC)
{
	switch (m_eFigure)
	{
	case FIGURETYPE::FT_RECT:
		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		break;
	case FIGURETYPE::FT_CIRCLE:
		Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		break;
	}
}

void CObject::Accelerated()
{
	if (CMainGame::iStageNum == STAGE_TWO)
		CMainGame::Gravity = 1.f;
	else 
	{
		CMainGame::Gravity = 0.f;
	}
	// ��¥�� ���
	{
		// �߷�
		m_tAcc = { 0.f, CMainGame::Gravity };

		// ���󿡼� ���ۿ�
		if (Is_OnGround())
			m_tAcc += {0.f, -CMainGame::Gravity };
	}

	// ���ӵ�/ �ӵ�/ ��ġ ��ȭ
	{
		m_tVel += m_tAcc * CMainGame::GameSpeed;
		m_tInfo += m_tVel * CMainGame::GameSpeed;
	}
	
	// �浹, ���� �� �ӵ�, ��ġ ����
	{
		if (m_tInfo.fY + m_tInfo.fCY*0.5f > PLAYZONEBOTTOM) // �Ʒ� �Ѿ�� ��
		{
			// y���� ��ȭ ������ ����
			m_tVel.vY -= m_tAcc.vY * CMainGame::GameSpeed;
			m_tInfo.fY -= m_tVel.vY * CMainGame::GameSpeed;

			// �浹 ���� �ӵ� ����
			FLOAT Ratio = CMainGame::quad_eqn(m_tAcc.vY * CMainGame::GameSpeed * CMainGame::GameSpeed, m_tVel.vY * CMainGame::GameSpeed, m_tInfo.fY - PLAYZONEBOTTOM + m_tInfo.fCY * 0.5f);
			
			m_tVel.vY += m_tAcc.vY * Ratio * CMainGame::GameSpeed;
			m_tInfo.fY = PLAYZONEBOTTOM - m_tInfo.fCY * 0.5f;
			
			// �ݹ߰�� ����
			m_tVel.vY *= -CMainGame::COR;
			// �浹 �� Y��ǥ ����
			m_tVel.vY -= m_tAcc.vY * (1 - Ratio) * CMainGame::GameSpeed;
			m_tInfo.fY += m_tVel.vY * (1 - Ratio) * CMainGame::GameSpeed;
		}
		
		else if (m_tInfo.fY - m_tInfo.fCY * 0.5f < PLAYZONETOP) // ���� �Ѿ�� ��
		{
			
			m_tInfo.fY = PLAYZONETOP + m_tInfo.fCY * 0.5f;

			m_tVel.vY *= -CMainGame::COR;
	
		}

		if (m_tInfo.fX + m_tInfo.fCX * 0.5f > PLAYZONERIGHT) // ������ �Ѿ�� ��
		{
			// ��ġ ���� 
			m_tInfo.fX = PLAYZONERIGHT - m_tInfo.fCX * 0.5f;
			//�ݹ߰�� ����
			m_tVel.vX *= -CMainGame::COR;
		}

		if (m_tInfo.fX - m_tInfo.fCX * 0.5f < PLAYZONELEFT) // ���� �Ѿ�� ��
		{
			// ��ġ ���� 
			m_tInfo.fX = PLAYZONELEFT + m_tInfo.fCX * 0.5f;
			//�ݹ߰�� ����
			m_tVel.vX *= -CMainGame::COR;
		}
	}
	
	if (m_tVel.vY == 0 && m_tInfo.fY == PLAYZONEBOTTOM - (m_tInfo.fCY * 0.5f))
		m_bOnGround = true;
	else	m_bOnGround = false;

}

void CObject::On_Attacked(CObject* _Attacker)
{
	m_tStat.m_fHp -= _Attacker->Get_Stat().m_fAttack;
	if (m_tStat.m_fHp < 0)
		m_tStat.m_fHp = 0;
}
