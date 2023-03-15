#pragma once
#include "Object.h"

class CBullet : public CObject
{
public:
	CBullet();
	virtual ~CBullet();

public:
	virtual void		Init() override;
	virtual int			Update() override;
	virtual void		Late_Update() override;
	virtual void		Render(HDC hDC) override;
	virtual void		Release() override;

public:
	void Set_Dead() { m_bDead = true; }

private:
	Vec2			m_tArrowTail;
	Vec2			m_tArrowHead;
	FLOAT			m_fArrowSize;
};

