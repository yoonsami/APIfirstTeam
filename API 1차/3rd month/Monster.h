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

public:
	void			Set_Pattern(int _Pattern) { m_iPattern = _Pattern; }
	void			Set_Bullet(list<CObject*>* _bulletList) { m_pBulletList = _bulletList; }

private:
	void Move(int _iPattern);
	void Random_Shooting();

private:
	DWORD64			m_dwInvincibleTime;
	DWORD64			m_dwBulletCreTime;
	FLOAT			m_fAngle;
	INT				m_iPattern;
	list<CObject*>* m_pBulletList;
};

