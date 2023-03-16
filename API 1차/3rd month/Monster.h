#pragma once
#include "Object.h"
class CMonster :  public CObject
{
public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void		Init() override;
	virtual int			Update() override;
	virtual void		Late_Update() override;
	virtual void		Render(HDC hDC) override;
	virtual void		Release() override;
	virtual void		Accelerated() override;

public:
	void			Set_Bullet(list<CObject*>* _bulletList) { m_pBulletList = _bulletList; }
	INT				Get_Pattern() { return m_iPattern; }
	void			Set_Target(INFO _tTarget) { m_tTarget = _tTarget; }
	void			Update_Hit_ArrowPos(FLOAT A);
	INT				Get_Type() { return m_iMonsterType; }
	void			Set_Type(INT _eType) { m_iMonsterType = _eType; }
private:
	void Move();
	void Acting();
	void Random_Shooting();
	void Create_Shield();

protected:
	DWORD64			m_dwInvincibleTime;
	DWORD64			m_dwBulletCreTime;
	INT				m_iPattern;
	list<CObject*>* m_pBulletList;
	FLOAT			m_fRadius;
	INFO			m_tTarget;
	INT				m_iMonsterType;
};

