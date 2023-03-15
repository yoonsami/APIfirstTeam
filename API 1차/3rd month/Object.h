#pragma once
#include "Include.h"
class CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	virtual void		Init() PURE;
	virtual int			Update()PURE;
	virtual void		Late_Update()PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release()PURE;

public:
	RECT&		Get_Rect() { return m_tRect; }
	INFO&		Get_Info() { return m_tInfo; }
	void		Set_Pos(float _fX, float _fY);
	void		Draw_Figure(HDC hDC);
	FIGURETYPE	Get_Figure() { return m_eFigure; }
	void		Set_Dead() { m_bDead = true; }
	void		Set_Velocity(const Vec2 _vel) { m_tVel = _vel; }
	void		Accelerated();
	FLOAT		Get_AimRadian() { return m_fAimRadian; }
	BOOL		Is_Invincible() { return m_bInvincible; }
	BOOL		Is_OnGround() { return m_bOnGround; }
	STAT&		Get_Stat() { return m_tStat; }
	void		On_Attacked(CObject* _Attacker);
	BOOL		Is_Dead() { if (m_tStat.m_fHp <= 0) m_bDead = true; return m_bDead; }


protected:
	void		Update_Rect();
	
protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	STAT		m_tStat;
	FIGURETYPE	m_eFigure;
	Vec2		m_tVel;
	Vec2		m_tAcc;
	FLOAT		m_fSpeed;
	BOOL		m_bDead;
	BOOL		m_bInvincible;
	BOOL		m_bOnGround;
	Vec2		m_tDirection;
	FLOAT		m_fAimRadian;
};

