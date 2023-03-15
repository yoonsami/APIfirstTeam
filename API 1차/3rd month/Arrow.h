#pragma once
#include "Bullet.h"

class CArrow : public CBullet
{
public:
	CArrow(Vec2& ArrowHead, Vec2& _Vel, FLOAT Speed);
	virtual ~CArrow();

public:
	virtual void		Init() override;
	virtual int			Update() override;
	virtual void		Late_Update() override;
	virtual void		Render(HDC hDC) override;
	virtual void		Release() override;
	virtual void		Accelerated() override;
	ARROWPOINT			Get_HeadTailPos() { return ARROWPOINT{ m_tArrowHead,m_tArrowTail }; }
protected:
	void Set_TailPos();
protected:
	Vec2			m_tArrowTail;
	Vec2			m_tArrowHead;
	FLOAT			m_fArrowSize;
};

