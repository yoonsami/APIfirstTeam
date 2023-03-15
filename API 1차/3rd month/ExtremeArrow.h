#pragma once
#include "Arrow.h"
class CExtremeArrow :public CArrow
{
public:
	CExtremeArrow(Vec2& ArrowHead, Vec2& _Vel, FLOAT Speed);
	virtual ~CExtremeArrow();

public:
	virtual void Init() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

public:
	void Set_TargetList(list<CObject*>* TargetList) { m_pTargetList = TargetList; }

private:
	void Find_ClosestTarget();

private:
	list<CObject*>* m_pTargetList;
	FLOAT			m_fDetectRange;
	CObject*		m_pTarget;
};

