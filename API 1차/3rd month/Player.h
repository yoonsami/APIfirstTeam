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

public:
	void Set_Bullet(list<CObject*>* _bulletList) { m_pBulletList = _bulletList; }
	void Set_Shield(list<CObject*>* _shieldList) { m_pShieldList = _shieldList; }
	void Set_AimPos();
	void Draw_Arrow(HDC hDC, FLOAT _size);
	Vec2& Get_ArrowTail() { return m_tArrowTail; }

private:
	void Key_Input();
	void Create_Shield();
private:
	list<CObject*>* m_pBulletList;
	list<CObject*>* m_pShieldList;
	CBow*			m_pBow;
	FLOAT			m_fBowPower;
	Vec2			m_tArrowTail;
	Vec2			m_tArrowHead;
	FLOAT			m_fArrowSize;
	DWORD64			m_dwBulletCreTime;
	DWORD64			m_dwShieldCreTime;
	Vec2			m_tKeyInputVec;
};

