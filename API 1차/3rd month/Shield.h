#pragma once
#include "Bullet.h"

class CShield : public CBullet
{
public:
	CShield(CObject* pOwner);
	virtual ~CShield();

public:
	virtual void	Init() override;
	virtual int		Update() override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void	Release() override;

public:
	void	Set_Radian();
	void	Set_PosInfo();
	void	Set_Owner(CObject* _pOwner) { m_pOwner = _pOwner; }

private:
	CObject*	m_pOwner;
	FLOAT		m_fRadius = 0.f;

};