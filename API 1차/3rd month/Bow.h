#pragma once
#include "Object.h"

class CBow :  public CObject
{
public:
	CBow(CObject* _Owner);
public:
	virtual void	Init() override;
	virtual int		Update() override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void	Release() override;

public:
	void	Set_Radius(FLOAT _radius) { m_fRadius = _radius; }
	void	Set_Radian();
	void	Set_PosInfo();
	void	Set_Owner(CObject* _pOwner) { m_pOwner = _pOwner; }
	void	Update_ArrowTail();
private:
	CObject*	m_pOwner;
	FLOAT		m_fRadius;
	Vec2		m_tArrowTail;
};

