#include "pch.h"
#include "Boss.h"
#include "AbstractFactory.h"

CBoss::CBoss() :m_pMonsterList(nullptr)
{

}

CBoss::~CBoss()
{

}

void CBoss::Init()
{
	//���� ó���� ����
	m_bInvincible = true;
	m_iMonsterType = MON_BOSS;
	m_tInfo.fX = WINCX / 2.f;
	m_tInfo.fY = WINCY / 3.f;
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 100.f;
	m_fSpeed = 3.f;
	m_tStat.m_fMaxHp = 100;
	m_tStat.m_fHp = m_tStat.m_fMaxHp;
	__super::Update_Rect();
}


int CBoss::Update()
{
	if (Is_Dead())
		return OBJ_DEAD;

	//���� ����Ʈ�� ������ ������ ����Ǯ��
	if (1 == m_pMonsterList->size())
		m_bInvincible = false;

	m_tInfo.fX += m_fSpeed;
	__super::Update_Rect();
	Update_MonsterList();

	if (0 >= m_tRect.left || WINCX <= m_tRect.right)
	{
		m_fSpeed *= -1;
	}

	return OBJ_NOEVENT;
}

//Ÿ�Ժ� ���� ����
CObject* CBoss::Create_Monster(float _fX, float _fY, INT _eType)
{
	CObject* temp = CAbstractFactory<CMonster>::Create(_fX, _fY);
	dynamic_cast<CMonster*>(temp)->Set_Type(_eType);
	return temp;
}

void CBoss::Create_Parts()
{
	m_pMonsterList->push_back(Create_Monster(m_tRect.left, m_tRect.top, MON_MONSTER));
	m_pMonsterList->push_back(Create_Monster(m_tRect.right, m_tRect.top, MON_RANDOMBULLET));
	m_pMonsterList->push_back(Create_Monster(m_tRect.left, m_tRect.bottom, MON_SHEILD));
	m_pMonsterList->push_back(Create_Monster(m_tRect.right, m_tRect.bottom, MON_SCREWBULLET));
	for (auto& iter : *m_pMonsterList)
		dynamic_cast<CMonster*>(iter)->Set_Bullet(m_pBulletList);
}

// ���������� ���͵� ��ǥ ����
void CBoss::Update_MonsterList()
{

}

