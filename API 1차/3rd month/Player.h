#pragma once
#include "Object.h"

class CBow;
class CBullet;
class CPlayer : public CObject
{
public:
	CPlayer();
	virtual ~CPlayer();
public:
	virtual void		Init() override;
	virtual int			Update() override;
	virtual void		Late_Update() override;
	virtual void		Render(HDC hDC) override;
	virtual void		Release() override;
	virtual void		Key_Input();

public:
	void Set_Bullet(list<CObject*>* _bulletList) { m_pBulletList = _bulletList; }
	void Set_GuidedBullet(list<CObject*>* _bulletList) { m_pGuidedBulletList = _bulletList; }
	

private:
	void Create_Shield();

protected:
	list<CObject*>* m_pBulletList;
	list<CObject*>* m_pGuidedBulletList;
	DWORD64			m_dwBulletCreTime;
	DWORD64			m_dwShieldCreTime;
	Vec2			m_tKeyInputVec;
};

