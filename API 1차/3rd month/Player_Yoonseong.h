#pragma once
#include "Player.h"

class CPlayer_Yoonseong :public CPlayer
{
public:
	CPlayer_Yoonseong();
	virtual ~CPlayer_Yoonseong();
public:
	virtual void		Init() override;
	virtual int			Update() override;
	virtual void		Late_Update() override;
	virtual void		Render(HDC hDC) override;
	virtual void		Release() override;
	virtual void		Key_Input() override;

public:
	void Set_Bullet(list<CObject*>* _bulletList) { m_pBulletList = _bulletList; }
	void Set_Shield(list<CObject*>* _shieldList) { m_pShieldList = _shieldList; }
	void Draw_Arrow(HDC hDC, FLOAT _size);
	Vec2& Get_ArrowTail() { return m_tArrowTail; }
	void Set_AimPos();
private:
	CBow*			m_pBow;
	FLOAT			m_fBowPower;
	Vec2			m_tArrowTail;
	Vec2			m_tArrowHead;
	FLOAT			m_fArrowSize;
};

