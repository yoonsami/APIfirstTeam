#pragma once
#include "Monster.h"
class CBoss :   public CMonster
{
public:
	CBoss();
	virtual ~CBoss();

public:
	virtual void Init() override;
	virtual int  Update() override;

public:
	CObject* Create_Monster(float _fX, float _fY, INT _eType);
	void Create_Parts();
	void Distroy_Parts();
	void Set_MonsterList(list<CObject*>* _pMonsterList) { m_pMonsterList = _pMonsterList; }
	void Update_MonsterList();

private:
	list<CObject*>* m_pMonsterList;
};


