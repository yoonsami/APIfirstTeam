#pragma once
#include "Include.h"

class CArrow;
class CObject
{
public:
	CObject(int objectType);
	virtual ~CObject();

public:
	virtual void		Init() PURE;
	virtual int			Update()PURE;
	virtual void		Late_Update()PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release()PURE;
	virtual void		Accelerated();
public:
	RECT&		Get_Rect() { return m_tRect; }
	INFO&		Get_Info() { return m_tInfo; }
	STAT&		Get_Stat() { return m_tStat; }
	void		Set_Pos(float _fX, float _fY);
	void		Draw_Figure(HDC hDC);
	void		Set_Dead() { m_bDead = true; }
	void		Set_DirVec(const Vec2& _vel) { m_tDirection = _vel; }
	Vec2&		Get_Velocity() {return m_tVel; }
	FIGURETYPE	Get_Figure() { return m_eFigure; }
	FLOAT		Get_AimRadian() { return m_fAimRadian; }
	BOOL		Is_Invincible() { return m_bInvincible; }
	BOOL		Is_OnGround() { return m_bOnGround; }
	void		On_Attacked(CObject* _Attacker);
	BOOL		Is_Dead() { if (m_tStat.m_fHp <= 0) m_bDead = true; else m_bDead = false; return m_bDead; }
	void		Hit_By_Arrow(CArrow* Arrow);
	void		Shirink_Size(FLOAT Ratio) { m_tInfo.fCX *= Ratio; m_tInfo.fCY *= Ratio; }
	void		Set_VelX(FLOAT velX) { m_tVel.vX = velX; }
	void		Set_VelY(FLOAT velY) { m_tVel.vY = velY; }
	void		Set_Size(float _fCX, float _fCY);
	INT			Get_ObjectType() { return m_iObjectType; }
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
	list<ARROWPOINT> m_tHitArrow;
	INT			m_iObjectType;
};

