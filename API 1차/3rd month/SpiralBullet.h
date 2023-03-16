#pragma once
#include "Bullet.h"
class CSpiralBullet :    public CBullet
{
public:
	CSpiralBullet();
	virtual ~CSpiralBullet();

public:
	virtual void		Init() override;
	virtual int			Update() override;
	virtual void		Render(HDC hDC) override;
	virtual void		Late_Update() override;
public:
	void	Set_Center(CObject* CenterObj) { m_pCenterObj = CenterObj; }
	void	Set_Side(INT OneorminusOne) { m_iSide = OneorminusOne; }
private:
	CObject*	m_pCenterObj;
	Vec2		m_tCenterPos;
	FLOAT		m_fRadian;
	FLOAT		m_fRadius;
	INT			m_iSide;
};


