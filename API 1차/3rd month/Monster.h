#pragma once
#include "Object.h"
class CMonster :  public CObject
{
public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void		Init() override;
	virtual int			Update() override;
	virtual void		Late_Update() override;
	virtual void		Render(HDC hDC) override;
	virtual void		Release() override;
	void				Set_Pattern(int _Pattern) { m_iPattern = _Pattern; }

private:
	void Move(int _iPattern);
	void Turn();

private:
	DWORD64			m_dwInvincibleTime;
	FLOAT			m_fAngle;
	INT				m_iPattern;
};

